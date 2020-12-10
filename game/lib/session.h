#ifndef LIB_SESSION_H
#define LIB_SESSION_H

#include "headers.h"

struct Session {
        byte encrypt_key[8];
        byte decrypt_key[8];
        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;
        char username[16];
        int conn_encrypted;
        int selected_character_index;
        int in_world;
};

typedef struct Session session_t;

/**
 * Mark the connection as encrypted.
 * The first packet sent by the Lineage 2
 * client is not encrypted. After that,
 * all packets are encrypted.
 */
void session_encrypt_connection(session_t *session);

/**
 * Encrypt packet. The encrypt_key
 * stored in session WILL BE updated.
 * The key always changes when encrypting
 * content, this allows to continue encrypting
 * new packets. If the key doesn't change and
 * gets updated on every encrypt, the
 * result won't work.
 */
void session_encrypt_packet(session_t *session, byte *dest, packet *src, size_t src_size);

/**
 * Decrypt packet only if connection
 * is encrypted. First packet sent
 * by the Lineage 2 client is not
 * encrypted.
 * The decrypt_key stored in session
 * WILL BE updated and this is required
 * for following packets to be properly
 * decrypted. It's the same as encryption.
 * The key always gets updated.
 */
void session_decrypt_packet(session_t *session, byte *dest, packet *src, size_t src_size);

/**
 * The user using this session
 * has logged into the game's world.
 */
void session_entered_world(session_t *session);

/**
 * Mark the session as leaving the world.
 */
void session_leaved_world(session_t *session);

/**
 * Update session.
 */
void session_update(
        session_t *session,
        char *username,
        size_t username_size,
        int loginOK1,
        int loginOK2,
        int playOK1,
        int playOK2
);

/**
 * Print session to std out.
 */
void session_print(session_t *session);

#endif
