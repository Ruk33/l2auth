// #ifndef L2AUTH_SOCKET_STRATEGY_WINDOWS_C
// #define L2AUTH_SOCKET_STRATEGY_WINDOWS_C

// #include <sys/types.h>
// #include <windows.h>
// #include <winsock2.h>
// #include <core/l2_socket.c>

// int socket_strategy_windows_connect(struct L2Socket* l2_socket)
// {
//         WSADATA wsa;

//         if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//                 return L2_SOCKET_WIN_WSA_STARTUP_ERROR;

//         l2_socket->socket = socket(AF_INET, SOCK_STREAM, 0);

//         if ((SOCKET) l2_socket->socket == INVALID_SOCKET)
//                 return L2_SOCKET_WIN_CREATE_SOCKET_ERROR;

//         return 0;
// }

// int socket_strategy_windows_bind
// (
//         struct L2Socket* l2_socket,
//         unsigned short port
// )
// {
//         SOCKADDR_IN addr;

//         addr.sin_family = AF_INET;
//         addr.sin_addr.s_addr = INADDR_ANY;
//         addr.sin_port = htons(port);

//         if (bind((SOCKET) l2_socket->socket, (SOCKADDR*) &addr, sizeof(addr)) == SOCKET_ERROR)
//                 return L2_SOCKET_WIN_BIND_ERROR;

//         return 0;
// }

// int socket_strategy_windows_listen(struct L2Socket* l2_socket)
// {
//         if (listen((SOCKET) l2_socket->socket, 3) != 0)
//                 return L2_SOCKET_WIN_LISTEN_ERROR;

//         return 0;
// }

// int socket_strategy_windows_accept
// (
//         struct L2Socket* server,
//         struct L2Socket* client
// )
// {
//         SOCKADDR_IN addr;
//         int addr_size = sizeof(addr);

//         client->socket = accept(
//                 (SOCKET) server->socket,
//                 (struct sockaddr*) &addr,
//                 &addr_size
//         );

//         if ((SOCKET) client->socket == INVALID_SOCKET)
//                 return L2_SOCKET_WIN_ACCEPT_ERROR;

//         return 0;
// }

// ssize_t socket_strategy_windows_receive
// (
//         struct L2Socket* l2_socket,
//         unsigned char* buffer,
//         size_t buffer_size
// )
// {
//         return recv((SOCKET) l2_socket->socket, buffer, buffer_size, 0);
// }

// ssize_t socket_strategy_windows_send
// (
//         struct L2Socket* l2_socket,
//         unsigned char* buffer,
//         size_t buffer_size
// )
// {
//         return send((SOCKET) l2_socket->socket, buffer, buffer_size, 0);
// }

// void socket_strategy_windows(struct L2SocketStrategy* socket_strategy)
// {
//         socket_strategy->connect = socket_strategy_windows_connect;
//         socket_strategy->bind = socket_strategy_windows_bind;
//         socket_strategy->listen = socket_strategy_windows_listen;
//         socket_strategy->accept = socket_strategy_windows_accept;
//         socket_strategy->receive = socket_strategy_windows_receive;
//         socket_strategy->send = socket_strategy_windows_send;
// }

// #endif //L2AUTH_SOCKET_STRATEGY_WINDOWS_C
