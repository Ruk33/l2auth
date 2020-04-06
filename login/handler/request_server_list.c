#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_SERVER_LIST_C
#define L2AUTH_LOGIN_HANDLER_REQUEST_SERVER_LIST_C

#include <log/log.h>
#include <core/l2_packet.h>
#include <login/packet/server_list.h>
#include <login/handler/request_server_list.h>

l2_packet* login_handler_request_server_list()
{
        log_info("Returning server list");
        return login_packet_server_list();
}

#endif
