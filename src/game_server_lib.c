#include "include/util.h"
#include "include/socket.h"
#include "include/conn.h"
#include "include/log.h"
#include "include/storage.h"
#include "include/game_server_request.h"
#include "include/game_server_lib.h"

void game_server_lib_load(conn_send_response_cb cb)
{
        conn_set_cb(cb);
        storage_open();
        log("Game server initialized.");
}

void game_server_lib_new_conn(socket_t *socket)
{
        log("Game server, accepting new connection!");
        game_server_request_new_conn(socket);
}

void game_server_lib_new_req(socket_t *socket, byte_t *buf, size_t n)
{
        log("Game server, new client request.");
        game_server_request(socket, buf, n);
}

void game_server_lib_disconnect(socket_t *socket)
{
        log("Game server, client disconnected.");
        game_server_request_disconnect(socket);
}
