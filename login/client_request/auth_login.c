#include <assert.h>
#include <stdio.h>
#include "../packet.h"
#include "../encrypt_packet.h"
#include "../socket.h"
#include "../rsa.h"
#include "../server_packet/ok.h"
#include "../storage/session.h"
#include "auth_login.h"

void client_request_auth_login(struct StorageSession *session, int fd, unsigned char *request)
{
        assert(session);
        assert(fd > 0);
        assert(request);

        unsigned char response[SERVER_PACKET_OK_FULL_SIZE_ENCRYPTED] = {0};

        // @TODO
        // These should be randomly generated
        // but for the time being, these will do
        session->playOK1 = 42;
        session->playOK2 = 24;

        printf("Handling auth login request.\n");

        // @TODO
        // The content of the decrypted packet
        // will contain the credentials sent
        // by the client. We will have to check
        // if these are valid against some kind
        // of database later...

        // rsa_decrypt(&session->rsa, packet_body(request), packet_body(request));
        // for (size_t i = 0; i < packet_size(request); i++) printf("[%ld %c]", i, request[i]);
        // printf("\n");

        server_packet_ok(response, session->playOK1, session->playOK2);
        encrypt_packet(&session->blowfish, response, response);
        socket_send(fd, response, (size_t) packet_size(response));
}
