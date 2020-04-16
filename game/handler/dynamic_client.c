#include <dlfcn.h>
#include <string.h>
#include <log/log.h>
#include <core/connection_thread.h>
#include <game/handler/dynamic_client.h>

void game_handler_dynamic_client(struct ConnectionThread* conn)
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

                if (!library) {
                        log_fatal("Could not load library");
                        break;
                }

                client_handler = dlsym(library, "game_handler_client");

                if (!client_handler) {
                        log_fatal("Could not find handler");
                        break;
                }

                closed_connection = (*client_handler)(conn);

                dlclose(library);

                if (closed_connection) break;
        }

        log_info("Gameserver client connection closed");
}
