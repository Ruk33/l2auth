#include <arpa/inet.h>
#include "include/server.h"
#include "include/conn.h"
#include "include/login_server_request.h"
#include "include/login_server_lib.h"

void login_server_lib_load(conn_send_response_cb cb)
{
        // Todo: Get these servers from a database.
        server_t bartz     = { 0 };
        server_t sieghardt = { 0 };

        bartz.id = 1;
        inet_pton(AF_INET, "127.0.0.1", bartz.ip);
        bartz.port        = 7777;
        bartz.age_limit   = 0x0f;
        bartz.pvp         = 0;
        bartz.players     = 0;
        bartz.max_players = 650;
        bartz.status      = 0x01;
        bartz.extra       = 0x00;
        bartz.brackets    = 0x00;

        sieghardt.id = 2;
        inet_pton(AF_INET, "0.0.0.0", sieghardt.ip);
        sieghardt.port        = 7778;
        sieghardt.age_limit   = 0x0f;
        sieghardt.pvp         = 0;
        sieghardt.players     = 0;
        sieghardt.max_players = 650;
        sieghardt.status      = 0x01;
        sieghardt.extra       = 0x00;
        sieghardt.brackets    = 0x00;

        server_add(&bartz);
        server_add(&sieghardt);

        conn_set_cb(cb);
}

void login_server_lib_new_conn(socket_t *socket)
{
        login_server_request_new_conn(socket);
}

void login_server_lib_new_req(socket_t *socket, byte_t *buf, size_t n)
{
        login_server_request(socket, buf, n);
}

void login_server_lib_disconnect(socket_t *socket)
{
        login_server_request_disconnect(socket);
}
