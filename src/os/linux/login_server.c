#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "../../server/login/include/session.h"
#include "../../server/login/include/server.h"

#include "../../util.c"
#include "../../packet.c"
#include "../../server/login/session.c"
#include "../../server/login/packet_decoder.c"
#include "../../server/login/packet_encoder.c"
#include "../../server/login/server.c"

#define MAX_SOCKETS 32

struct unix_socket {
	int fd;
	int epoll_fd;
	// Don't think we need more than
	// 1024 of bytes for responses in
	// the login server. If for some reason
	// we need more, just increase it.
	byte buf_write[1024];
	size_t written;
	size_t write_size;
	struct session *session;
};

static void print_errno(void)
{
	printf("%s.\n", strerror(errno));
}

static int unix_socket_init(int *dest, u16 port, int max)
{
	struct sockaddr_in address = { 0 };
	struct sockaddr *address_p = 0;

	int reuse = 0;

	*dest = socket(AF_INET, SOCK_STREAM, 0);

	if (*dest < 0) {
		printf("unable to create socket.\n");
		print_errno();
		goto abort;
	}

	// Make the socket reusable.
	reuse = 1;
	if (setsockopt(*dest, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("unable to mark the socket as reusable.\n");
		print_errno();
		goto abort;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	address_p = (struct sockaddr *) &address;

	if (bind(*dest, address_p, sizeof(address)) < 0) {
		printf("unable to bind socket.\n");
		print_errno();
		goto abort;
	}

	if (listen(*dest, max) < 0) {
		printf("unable to listen socket.\n");
		print_errno();
		goto abort;
	}

	return 1;

abort:
	close(*dest);
	return 0;
}

static int unix_socket_accept(struct unix_socket *dest, int socket_fd)
{
	struct sockaddr_in address = { 0 };
	struct sockaddr *address_p = 0;
	socklen_t addrlen = 0;

	struct epoll_event event = { 0 };

	assert(dest);

	address_p = (struct sockaddr *) &address;

	dest->fd = accept(socket_fd, address_p, &addrlen);

	if (dest->fd < 0) {
		printf("failed to accept new client.\n");
		print_errno();
		goto abort;
	}

	dest->epoll_fd = epoll_create1(0);

	if (dest->epoll_fd == -1) {
		printf("failed to create epoll for new client.\n");
		print_errno();
		goto abort;
	}

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = dest->fd;

	if (epoll_ctl(dest->epoll_fd, EPOLL_CTL_ADD, dest->fd, &event) == -1) {
		printf("failed to add event to client epoll.\n");
		print_errno();
		goto abort;
	}

	return 1;

abort:
	close(dest->fd);
	close(dest->epoll_fd);
	return 0;
}

static void unix_socket_close(struct unix_socket *socket)
{
	shutdown(socket->fd, SHUT_RDWR);
	close(socket->fd);
	close(socket->epoll_fd);
}

static int unix_socket_flush(struct unix_socket *socket)
{
	ssize_t written = 0;

	assert(socket);

	while (socket->written < socket->write_size) {
		written = send(
			socket->fd,
			socket->buf_write + socket->written,
			socket->write_size - socket->written,
			0
		);

		if (written == -1) {
			printf("failed to flush response.\n");
			print_errno();
			return 0;
		}

		socket->written += written;
	}

	// After the packet gets sent, reset the counter.
	socket->write_size = 0;
	socket->written = 0;

	return 1;
}

static void unix_socket_write(struct unix_socket *socket, byte *buf, size_t n)
{
	assert(socket);
	assert(buf);

	// Append buf to the end of the write buffer.
	memcpy(socket->buf_write + socket->write_size, buf, n);
	socket->write_size += n;
}

static void unix_socket_accept_and_fork(int server_fd)
{
	pid_t pid = 0;

	struct unix_socket client = { 0 };
	struct state state = { 0 };

	struct epoll_event event = { 0 };
	int ev_count = 0;

	ssize_t read = 0;

	if (!unix_socket_accept(&client, server_fd)) {
		printf("the client couldn't be accepted.\n");
		return;
	}

	pid = fork();

	switch (pid) {
	// Error while forking.
	case -1:
		printf("unable to fork, the client will be dropped.\n");
		print_errno();
		unix_socket_close(&client);
		return;
	// Child-process.
	case 0:
		close(server_fd);
		break;
	// Parent-process.
	default:
		close(client.fd);
		close(client.epoll_fd);
		return;
	}

	printf("connection accepted. sending initial packet.\n");
	client.session = server_create_connection(&state);
	if (client.session) {
		client.session->play_ok1 = rand();
		client.session->play_ok2 = rand();
		unix_socket_write(
			&client,
			client.session->response.buf,
			packet_size(&client.session->response)
		);
		unix_socket_flush(&client);
	} else {
		printf("no client returned for connection. closing it.\n");
		goto abort;
	}

do_read:
	ev_count = epoll_wait(client.epoll_fd, &event, 1, 5000);

	if (ev_count == -1) {
		printf("failed to listen for requests in epoll.\n");
		print_errno();
		goto abort;
	}
	if (ev_count == 0) {
		printf("dropping client for taking too long.\n");
		goto abort;
	}

	read = recv(
		client.fd,
		client.session->request.packet.buf + client.session->request.received,
		sizeof(client.session->request.packet.buf) - client.session->request.received,
		0
	);

	if (read == 0) {
		printf("closing the connection as requested by client.\n");
		goto success;
	}
	if (read == -1) {
		printf("unable to read. the client will be dropped.\n");
		goto abort;
	}

	client.session->request.received += (size_t) read;
	server_on_request(&state, client.session);

	// Keep reading if the packet isn't completed.
	if (client.session->request.is_partial) {
		goto do_read;
	}

	// Reset the read counter when the entire
	// packet has been read.
	client.session->request.received = 0;

	if (packet_size(&client.session->response)) {
		unix_socket_write(
			&client,
			client.session->response.buf,
			packet_size(&client.session->response)
		);
		unix_socket_flush(&client);
	}

	goto do_read;

success:
	server_on_disconnect(&state, client.session);
	unix_socket_close(&client);
	exit(EXIT_SUCCESS);

abort:
	if (client.session)
		server_on_disconnect(&state, client.session);
	unix_socket_close(&client);
	exit(EXIT_FAILURE);	
}
 
int main(void)
{
	int server_fd = 0;
	u16 port = 2106;

	// Prevent zombie process from fork.
	signal(SIGCHLD, SIG_IGN);
	srand(time(0));

	if (!unix_socket_init(&server_fd, port, MAX_SOCKETS)) {
		printf("unable to initialize socket.\n");
		return 1;
	}

	printf("login server started. listening for connections.\n");
	while (1)
		unix_socket_accept_and_fork(server_fd);

	return 0;
}
