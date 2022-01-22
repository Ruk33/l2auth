#ifndef INCLUDE_LS_TYPES_H
#define INCLUDE_LS_TYPES_H

#include "config.h"

struct ls_rsa;

struct ls_blowfish;

struct ls_session {
    size_t id;
    struct platform_socket *socket;
    struct ls_blowfish *blowfish;
    struct ls_rsa *rsa;
    i32_t playOK1;
    i32_t playOK2;

    // If the session is being used, active will be 1.
    // Used to avoid double frees when sessions are
    // disconnected.
    int active;
};

struct ls_account {
    char username[14];
    byte_t encrypted_password[64];
};

struct ls_server {
    u8_t id;
    // ipv4, example: 255.255.255.255
    // We store the text ip in the database to make it easier to read from.
    // We could simply store an int, but it's harder to read and inspect.
    char text_ip[32];
    u32_t ip;
    u32_t port;
    u8_t age_limit;
    u8_t pvp;
    u16_t players;
    u16_t max_players;
    u8_t status;
    u32_t extra;
    u8_t brackets;
};

struct ls_state {
    struct ls_session sessions[MAX_CLIENTS];
    size_t session_instances[MAX_CLIENTS];
    size_t session_count;

    void (*send_response)(struct platform_socket *socket, void *buf, size_t n);
    void (*disconnect)(struct platform_socket *socket);
    u32_t (*text_ip_to_u32)(char *ip);
};

#endif
