#include "../headers.h"
#include "../storage/session.h"
#include "../storage/character.h"
#include "../server_packet/create_char.h"
#include "../client_request/auth_request.h"
#include "../character.h"
#include "create_char.h"

static void store_new_char_info_from_packet(
        struct ClientRequestCreateChar *dest,
        packet *src
)
{
        assert(dest);
        assert(src);

        byte *p = packet_body(src);

        l2_string_to_char(dest->name, p, sizeof(dest->name));
        p += l2_string_bytes(p);

        BYTE_READ_VAL(dest->race, p);
        BYTE_READ_VAL(dest->sex, p);
        BYTE_READ_VAL(dest->_class, p);
        BYTE_READ_VAL(dest->_int, p);
        BYTE_READ_VAL(dest->str, p);
        BYTE_READ_VAL(dest->con, p);
        BYTE_READ_VAL(dest->men, p);
        BYTE_READ_VAL(dest->dex, p);
        BYTE_READ_VAL(dest->wit, p);
        BYTE_READ_VAL(dest->hair_style, p);
        BYTE_READ_VAL(dest->hair_color, p);
        BYTE_READ_VAL(dest->face, p);
}

static void print_new_char_info(struct ClientRequestCreateChar *new_char_info)
{
        assert(new_char_info);

        printf("Name: %s.\n", new_char_info->name);
        printf("Race: %d.\n", new_char_info->race);
        printf("Sex: %d.\n", new_char_info->sex);
        printf("Class: %d.\n", new_char_info->_class);
        printf("Int: %d.\n", new_char_info->_int);
        printf("Str: %d.\n", new_char_info->str);
        printf("Con: %d.\n", new_char_info->con);
        printf("Men: %d.\n", new_char_info->men);
        printf("Dex: %d.\n", new_char_info->dex);
        printf("Wit: %d.\n", new_char_info->wit);
        printf("Hair style: %d.\n", new_char_info->hair_style);
        printf("Hair color: %d.\n", new_char_info->hair_color);
        printf("Face: %d.\n", new_char_info->face);
}

static void store_new_char_from_packet(
        char *account,
        storage_character_t *character_storage,
        struct ClientRequestCreateChar *new_char_info
)
{
        assert(account);
        assert(character_storage);
        assert(new_char_info);

        character_t new_character = {0};

        new_character.id = 2;
        memcpy(new_character.name, new_char_info->name, strlen(new_char_info->name) + 1);
        new_character.active = 1,
        new_character.hp = 42;
        new_character.mp = 42;
        new_character.max_hp = 42;
        new_character.max_mp = 33;
        new_character.level = 1;
        new_character.sex = new_char_info->sex;
        new_character.race_id = new_char_info->race;
        new_character.class_id = new_char_info->_class;
        new_character.x = 0;
        new_character.y = 0;
        new_character.z = 0;

        storage_character_add(
                character_storage,
                account,
                strlen(account) + 1,
                &new_character
        );
}

void client_request_create_char(
        int client,
        packet *request,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(request);
        assert(session);
        assert(character_storage);
        assert(send_response);

        char *username = session->username;

        packet response[SERVER_PACKET_CREATE_CHAR_FULL_SIZE] = {0};
        struct ClientRequestCreateChar new_char_info = {0};

        store_new_char_info_from_packet(&new_char_info, request);
        print_new_char_info(&new_char_info);
        store_new_char_from_packet(username, character_storage, &new_char_info);

        server_packet_create_char(response);
        session_encrypt_packet(
                session,
                response,
                response,
                (size_t) packet_get_size(response)
        );
        send_response(
                client,
                response,
                (size_t) packet_get_size(response)
        );

        client_request_auth_request_from_session(
                client,
                session,
                character_storage,
                send_response
        );
}
