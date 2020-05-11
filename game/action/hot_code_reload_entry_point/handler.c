#include <dlfcn.h>
#include <string.h>
#include <log/log.h>
#include <core/connection_thread.h>
#include "handler.h"

void game_action_hot_code_reload_entry_point_handler(struct ConnectionThread* conn)
{
        unsigned char encrypt_key[] = {
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        unsigned char decrypt_key[] = {
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        int closed_connection;

        void* library;
        int (*client_handler)(struct ConnectionThread*);

        log_info("Handling new client from gameserver");

        memcpy(conn->encrypt_key, encrypt_key, sizeof(encrypt_key));
        memcpy(conn->decrypt_key, decrypt_key, sizeof(decrypt_key));

        while (1) {
                library = dlopen("libgameserver.so", RTLD_LAZY);

                if (library) {
                        log_info("Library successfully loaded");
                } else {
                        log_fatal("Could not load library");
                        break;
                }

                client_handler = dlsym(library, "game_handler_client");

                if (client_handler) {
                        log_info("Client handler successfully found");
                } else {
                        log_fatal("Could not find client handler");
                        break;
                }

                closed_connection = (*client_handler)(conn);

                dlclose(library);

                if (closed_connection) break;
        }

        log_info("Gameserver client connection closed");
}
