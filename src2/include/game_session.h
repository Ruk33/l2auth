#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "game_server.h"
#include "coroutine.h"

struct crypt_key {
    byte buf[8];
};

struct game_session {
    u32 id;
    int active;
    int closed;
    size_t read;
    struct packet response;
    struct packet request;
    struct username username;
    // struct character *character;
    struct crypt_key encrypt_key;
    struct crypt_key decrypt_key;
    // size_t received;
    // int partial;
    // struct packet response_queue[8];
    // size_t response_queue_count;
    int conn_encrypted;
    struct coroutine state;
};

void game_session_decrypt_packet(struct game_session *session, struct packet *packet);

void game_session_encrypt_packet(struct game_session *session, struct packet *src);

#endif
