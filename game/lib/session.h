#ifndef LIB_SESSION_H
#define LIB_SESSION_H

#include "headers.h"

enum SessionState {
        /*
         * Initial state. When the user tries to
         * connect to the game server the first
         * packet sent is the protocol version.
         */
        PROTOCOL_VERSION,
        /*
         * Successfully authenticated.
         */
        AUTH_REQUEST,
        /*
         * The user is on the character selection screen.
         */
        CHARACTER_SELECTION,
        /*
         * The user is in the create character screen.
         */
        CREATING_CHARACTER,

        /*
         * All states below this line assume the
         * user is already in the world with a character.
         * Or, in other words, the player is just playing.
         */

        /*
         * The user is loading and entering into the world.
         */
        ENTERING_WORLD,
        /*
         * The user enters the world with a character.
         */
        IN_WORLD,
};

typedef enum SessionState session_state_t;

struct Session {
        int socket;
        byte_t encrypt_key[8];
        byte_t decrypt_key[8];
        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;
        char username[16];
        int conn_encrypted;
        unsigned int selected_character_index;
        int in_world;
        session_state_t state;
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
void session_encrypt_packet(session_t *session, byte_t *dest, packet *src, size_t src_size);

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
void session_decrypt_packet(session_t *session, byte_t *dest, packet *src, size_t src_size);

/**
 * Update session's state.
 */
void session_update_state(session_t *session, session_state_t new_state);

/**
 * Update session.
 */
void session_update(session_t *session, char *username, size_t username_size, int loginOK1, int loginOK2, int playOK1, int playOK2);

/**
 * Print session to std out.
 */
void session_print(session_t *session);

#endif
