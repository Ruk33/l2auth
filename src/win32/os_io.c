#include <windows.h>
#include <winsock2.h>
#include <assert.h>
#include "../include/util.h"
#include "../include/recycle_id.h"
#include "../include/os_io.h"

#pragma comment(lib, "ws2_32.lib")

static int initialized = 0;

#define MAX_CONN 1024
#define READ_BUF_SIZE 524288

enum io_type {
        INVALID,
        SERVER_SOCKET,
        CLIENT_SOCKET,
        TIMER,
};

struct os_io {
        size_t id;
        SOCKET sock;
        enum io_type type;
};

static size_t io_instances[MAX_CONN] = { 0 };

static struct os_io ios[MAX_CONN] = { 0 };

static size_t io_count = 0;

static os_io_cb *io_cb = 0;

static void initialize()
{
        WORD wVersionRequested = 0;

        WSADATA wsaData = { 0 };

        int err = 0;

        wVersionRequested = MAKEWORD(2, 2);

        err = WSAStartup(wVersionRequested, &wsaData);

        if (err != 0) {
                printf("unable to start wsa %d.\n", err);
                return;
        }

        // Check if the proper version was loaded.
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
                printf("could not find a usable version of winsock.dll.\n");
                WSACleanup();
                return;
        }

        initialized = 1;
}

static struct os_io *io_accept_connection(SOCKET s)
{
        struct os_io io = { 0 };

        io.sock = WSAAccept(s, 0, 0, 0, 0);
        io.type = CLIENT_SOCKET;

        if (io.sock == INVALID_SOCKET) {
                printf("unable to accept connection: %d.\n", WSAGetLastError());
                return 0;
        }

        printf("connection accepted.\n");

        io_count += recycle_id_get(&io.id, io_instances);
        ios[io.id] = io;

        return &ios[io.id];
}

struct os_io *os_io_socket_create(u16_t port, size_t max_conn)
{
        struct os_io io = { 0 };

        struct sockaddr_in service = { 0 };

        int bind_result = 0;

        if (!initialized) {
                initialize();
        }

        if (!initialized) {
                return 0;
        }

        io.sock = WSASocketA(
                AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

        if (io.sock == INVALID_SOCKET) {
                printf("unable to create socket: %d.\n", WSAGetLastError());
                return 0;
        }

        service.sin_family      = AF_INET;
        service.sin_addr.s_addr = inet_addr("127.0.0.1");
        service.sin_port        = htons(port);

        bind_result = bind(io.sock, (SOCKADDR *) &service, sizeof(service));

        if (bind_result == SOCKET_ERROR) {
                printf("unable to bind socket: %d.\n", WSAGetLastError());
                return 0;
        }

        if (listen(io.sock, max_conn) == SOCKET_ERROR) {
                printf("unable to listen: %d.\n", WSAGetLastError());
                return 0;
        }

        io.type = SERVER_SOCKET;
        io_count += recycle_id_get(&io.id, io_instances);
        ios[io.id] = io;

        return &ios[io.id];
}

int os_io_close(struct os_io *io)
{
        assert(io);

        shutdown(io->sock, SD_BOTH);
        closesocket(io->sock);

        printf("closing\n");

        recycle_id(io_instances, io->id);
        *io = (struct os_io){ 0 };

        return 0;
}

long os_io_write(struct os_io *io, void *buf, size_t n)
{
        WSABUF data_buf = { 0 };
        DWORD written   = 0;

        assert(io);

        data_buf.len = n;
        data_buf.buf = buf;

        // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsasend
        if (WSASend(io->sock, &data_buf, 1, &written, 0, 0, 0) ==
            SOCKET_ERROR) {
                printf("unable to write: %d.\n", WSAGetLastError());
                return -1;
        }

        // The fourth parameter of WSASend writes
        // how many bytes were sent only if sent immediately
        // and the overlap parameters aren't used.
        // Todo: properly implement.
        return (long) written;
}

VOID CALLBACK io_on_tick(PVOID io, BOOLEAN timer_or_wait_fired)
{
        io_cb(io, OS_IO_TIMER_TICK, 0, 0);
}

struct os_io *os_io_timer(double timeout)
{
        HANDLE timer       = 0;
        HANDLE timer_queue = 0;

        BOOL timer_created = 0;

        timer_queue = CreateTimerQueue();

        if (!timer_queue) {
                return 0;
        }

        timer_created = CreateTimerQueueTimer(
                &timer,
                timer_queue,
                (WAITORTIMERCALLBACK) io_on_tick,
                0,
                timeout * 1000,
                timeout * 1000,
                0);

        if (!timer_created) {
                return 0;
        }

        return 0;
}

int os_io_listen(os_io_cb *cb)
{
        static byte_t buf[READ_BUF_SIZE] = { 0 };

        int io_ready = 0;

        struct os_io *io = 0;

        WSABUF data_buf               = { 0 };
        WSAOVERLAPPED recv_overlapped = { 0 };
        DWORD read                    = 0;
        DWORD flags                   = 0;

        fd_set readfds = { 0 };

        io_cb = cb;

        data_buf.len = READ_BUF_SIZE;
        data_buf.buf = buf;

        while (1) {
                FD_ZERO(&readfds);

                for (size_t i = 0; i < io_count; i += 1) {
                        if (ios[i].type == INVALID) {
                                continue;
                        }
                        FD_SET(ios[i].sock, &readfds);
                }

                io_ready = select(0, &readfds, 0, 0, 0);

                if (io_ready == SOCKET_ERROR) {
                        continue;
                }

                for (size_t i = 0; i < io_count; i += 1) {
                        io = &ios[i];

                        if (io->type == INVALID) {
                                continue;
                        }

                        if (FD_ISSET(io->sock, &readfds) != TRUE) {
                                continue;
                        }

                        switch (io->type) {
                        case SERVER_SOCKET:
                                io = io_accept_connection(io->sock);
                                if (io) {
                                        cb(io, OS_IO_SOCKET_CONNECTION, 0, 0);
                                }
                                break;
                        case CLIENT_SOCKET:
                                // Don't clear entire buffer, only what we read
                                // previously.
                                if (read > 0) {
                                        SecureZeroMemory(data_buf.buf, read);
                                }

                                switch (
                                        WSARecv(io->sock,
                                                &data_buf,
                                                1,
                                                &read,
                                                &flags,
                                                &recv_overlapped,
                                                0)) {
                                case SOCKET_ERROR:
                                        cb(io, OS_IO_SOCKET_READ_ERROR, 0, 0);
                                        break;
                                default:
                                        if (read) {
                                                cb(io,
                                                   OS_IO_SOCKET_REQUEST,
                                                   buf,
                                                   read);
                                        } else {
                                                cb(io,
                                                   OS_IO_SOCKET_DISCONNECTED,
                                                   0,
                                                   0);
                                                os_io_close(io);
                                        }
                                        break;
                                }

                                break;
                        case TIMER:
                                break;
                        default:
                                break;
                        }
                }
        }

        return 1;
}
