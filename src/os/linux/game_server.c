#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "../../include/util.h"
#include "../../include/packet.h"
#include "../../server/game/include/l2_string.h"
#include "../../server/game/include/session.h"
#include "../../server/game/include/server.h"

#include "../../packet.c"
#include "../../util.c"
#include "../../server/game/character.c"
#include "../../server/game/packet_decoder.c"
#include "../../server/game/session.c"
#include "../../server/game/db_unix.c"
#include "../../server/game/l2_string.c"
#include "../../server/game/packet_reader.c"
#include "../../server/game/player.c"
#include "../../server/game/random_unix.c"
#include "../../server/game/packet_encoder.c"
#include "../../server/game/server.c"

#ifndef MAX_SOCKETS
#define MAX_SOCKETS 512
#endif

struct unix_socket {
	int fd;
	byte buf_write[KB(4)];
	size_t sent;
	size_t write_size;
	struct session *session;
};

static void unix_socket_set_non_block(int fd)
{
	int flags = 0;

	flags = fcntl(fd, F_GETFL, 0);
	flags = flags < 0 ? 0 : flags;

	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static int unix_socket_init(int *dest, u16 port, int max)
{
	struct sockaddr_in address = { 0 };
	struct sockaddr *address_p = 0;

	int reusable_enable = 0;

	*dest = socket(AF_INET, SOCK_STREAM, 0);

	if (*dest < 0)
		goto abort;

	unix_socket_set_non_block(*dest);

	// Make the socket reusable.
	reusable_enable = 1;
	if (setsockopt(*dest, SOL_SOCKET, SO_REUSEADDR, &reusable_enable, sizeof(int)) < 0)
		goto abort;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	address_p = (struct sockaddr *) &address;

	if (bind(*dest, address_p, sizeof(address)) < 0)
		goto abort;

	if (listen(*dest, max) < 0)
		goto abort;

	return 1;

abort:
	close(*dest);
	return 0;
}

static int unix_socket_accept(struct unix_socket *dest, int server_fd)
{
	struct sockaddr_in address = { 0 };
	struct sockaddr *address_p = 0;
	socklen_t addrlen = 0;

	if (!dest) {
		printf("unable to accept new client. too many clients?\n");
		return 0;
	}

	address_p = (struct sockaddr *) &address;
	dest->fd = accept(server_fd, address_p, &addrlen);

	if (dest->fd < 0) {
		*dest = (struct unix_socket){ 0 };
		printf("failed to accept new client.\n");
		return 0;
	}

	unix_socket_set_non_block(dest->fd);
	return 1;
}

static void unix_socket_close(struct unix_socket *socket)
{
	shutdown(socket->fd, SHUT_RDWR);
	close(socket->fd);
	*socket = (struct unix_socket){ 0 };
}

static void unix_socket_flush(struct unix_socket *socket)
{
	ssize_t tmp = 0;

	assert(socket);

	while (tmp != -1 && socket->write_size > socket->sent) {
		tmp = send(
			socket->fd,
			socket->buf_write + socket->sent,
			socket->write_size - socket->sent,
			0
		);
		socket->sent += MAX(0, tmp);
	}

	// If the entire packet has been sent, reset the counter.
	if (socket->write_size == socket->sent) {
		socket->write_size = 0;
		socket->sent = 0;
	}
}

static void unix_socket_write(struct unix_socket *socket, byte *buf, size_t n)
{
	assert(socket);
	assert(buf);

	if (socket->write_size + n > ARR_LEN(socket->buf_write)) {
		printf("warning, prevented buf overflow in unix_socket_write.\n");
		return;
	}

	// Append buf to the end of the write buffer.
	cpy_bytes(socket->buf_write + socket->write_size, buf, n);
	socket->write_size += n;
}

static void on_new_connection(struct unix_socket *dest, struct state *state, int server_fd, int epoll_fd)
{
	struct epoll_event event = { 0 };

	assert(dest);
	assert(state);

	printf("detected new connection.\n");

	if (!unix_socket_accept(dest, server_fd)) {
		printf("unable to accept new client.\n");
		return;
	}

	event.events = EPOLLIN | EPOLLOUT;
	event.data.ptr = dest;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, dest->fd, &event) == -1) {
		printf("failed to epoll add. closing the connection.\n");
		unix_socket_close(dest);
		return;
	}

	dest->session = server_on_new_connection(state);
	if (!dest->session) {
		printf("client couldn't initialize. closing the connection.\n");
		unix_socket_close(dest);
		return;
	}
}

static void on_read(struct state *state, struct unix_socket *client)
{
	ssize_t tmp = 0;

	assert(state);
	assert(client);

	// Read until the call "blocks"
	// This ensures no bytes are left waiting to be consumed.
	while (tmp != -1) {
		tmp = recv(
			client->fd,
			client->session->request.buf + client->session->received,
			sizeof(client->session->request.buf) - client->session->received,
			0
		);
		client->session->received += tmp > 0 ? ((size_t) tmp) : 0;
	}

	// Connection is closed.
	if (client->session->received == 0) {
		printf("closing connection as requested by client.\n");
		server_on_disconnect(state, client->session);
		unix_socket_close(client);
		return;
	}

	if (errno != EAGAIN) {
		printf("failed to read request, closing connection.\n");
		server_on_disconnect(state, client->session);
		unix_socket_close(client);
		return;
	}

	server_on_request(state, client->session);

	if (!packet_size(&client->session->response))
		return;

	unix_socket_write(
		client,
		client->session->response.buf,
		packet_size(&client->session->response)
	);
	unix_socket_flush(client);
}

static void on_write(struct unix_socket *client)
{
	assert(client);
	unix_socket_flush(client);
}

static int unix_socket_listen(int server_fd)
{
	static struct epoll_event events[MAX_SOCKETS] = { 0 };
	static struct unix_socket clients[MAX_SOCKETS] = { 0 };
	static struct state state = { 0 };

	int epoll_fd = 0;
	int timer_fd = 0;

	int can_read = 0;
	int can_write = 0;

	struct itimerspec utmr = { 0 };

	struct epoll_event event = { 0 };
	int ev_count = 0;

	size_t client_count = 0;

	epoll_fd = epoll_create1(0);

	if (epoll_fd < 0) {
		printf("unable to create epoll. server can't start.\n");
		return 0;
	}

	timer_fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);

	if (timer_fd < 0) {
		printf("unable to create timer. server can't start.\n");
		return 0;
	}

	// Start the timer to run every second.
	utmr.it_value.tv_sec = 1;
	timerfd_settime(timer_fd, 0, &utmr, 0);

	// Add timer to epoll.
	event.events = EPOLLIN;
	event.data.fd = timer_fd;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &event) == -1) {
		printf("unable to add timer to epoll. server can't start.\n");
		return 0;
	}

	// Add server socket to epoll.
	event.events = EPOLLIN;
	event.data.fd = server_fd;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
		printf("unable to add server socket to epoll. server can't start.\n");
		return 0;
	}
	printf("listening for new connections.\n");

	while (1) {
		ev_count = epoll_wait(epoll_fd, events, ARR_LEN(events), -1);

		if (ev_count < 0) {
			printf("epoll_wait failed, closing the server.\n");
			break;
		}

		for (int i = 0; i < ev_count; i += 1) {
			can_read = (events[i].events & EPOLLIN) == EPOLLIN;
			can_write = (events[i].events & EPOLLOUT) == EPOLLOUT;

			if (events[i].data.fd == server_fd && can_read) {
				on_new_connection(&clients[client_count], &state, server_fd, epoll_fd);
				client_count += 1;
				continue;
			}
			if (events[i].data.fd == timer_fd && can_read) {
				// Start the timer again.
				timerfd_settime(timer_fd, 0, &utmr, 0);
				server_on_tick(&state, 0.3f);
				
				// Handle queue responses.
				for (size_t n = 0; n < client_count; n += 1) {
					// Ignore clients that are not playing.
					if (!clients[n].session->character)
						continue;
					// for (size_t z = 0; z < clients[n].client->response_queue_count; z += 1)
					// 	unix_socket_write(
					// 		&clients[n],
					// 		clients[n].session->response_queue[z].buf,
					// 		packet_size(&clients[n].session->response_queue[z])
					// 	);
					// clients[n].session->response_queue_count = 0;
					unix_socket_flush(&clients[n]);
				}

				continue;
			}
			if (can_read)
				on_read(&state, events[i].data.ptr);

			if (can_write)
				on_write(events[i].data.ptr);
		}
	}

	close(server_fd);
	close(epoll_fd);

	return 1;
}

int main()
{
	int server_fd = 0;
	u16 port = 7777;

	srand(time(0));

	if (!unix_socket_init(&server_fd, port, MAX_SOCKETS)) {
		printf("unable to initialize socket.\n");
		return 1;
	}

	if (!unix_socket_listen(server_fd)) {
		printf("unable to listen for requests.\n");
		return 1;
	}

	return 0;
}
