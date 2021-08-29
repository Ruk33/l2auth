#include <assert.h>
#include "include/util.h"
#include "include/list.h"
#include "include/os_io.h"
#include "include/storage.h"
#include "include/l2_string.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_server_packets.h"
#include "include/gs_character.h"
#include "include/gs_session.h"
#include "include/gs_ai.h"
#include "include/gs_request.h"

static void
handle_protocol_version(struct gs_state *gs, struct gs_session *session)
{
        struct gs_packet_protocol_version protocol_version = { 0 };

        packet_t response[16] = { 0 };

        assert(gs);
        assert(session);

        gs_packet_protocol_version_pack(response, &protocol_version);
        gs_session_send_packet(gs, session, response);
}

static void handle_enter_world(struct gs_state *gs, struct gs_session *session)
{
        static struct gs_packet_enter_world enter_world = { 0 };

        static packet_t response[1024] = { 0 };

        struct gs_character *character = 0;

        assert(gs);
        assert(session);

        bytes_zero((byte_t *) &enter_world, sizeof(enter_world));
        bytes_zero(response, sizeof(response));

        character = gs_character_from_session(gs, session);

        if (!character) {
                log_normal("entering world without a character? ignoring.");
                return;
        }

        gs_character_spawn(gs, character);

        l2_string_from_char(
                enter_world.name, character->name, sizeof(enter_world.name));

        enter_world.heading       = character->heading;
        enter_world.x             = character->position.x;
        enter_world.y             = character->position.y;
        enter_world.z             = character->position.z;
        enter_world.id            = character->id;
        enter_world.race_id       = character->race;
        enter_world.sex           = character->sex;
        enter_world.class_id      = character->_class;
        enter_world.level         = character->level;
        enter_world.exp           = character->exp;
        enter_world.str           = character->stats.str;
        enter_world.dex           = character->stats.dex;
        enter_world.con           = character->stats.con;
        enter_world._int          = character->stats._int;
        enter_world.wit           = character->stats.wit;
        enter_world.men           = character->stats.men;
        enter_world.hp            = character->stats.hp;
        enter_world.mp            = character->stats.mp;
        enter_world.max_hp        = character->stats.max_hp;
        enter_world.max_mp        = character->stats.max_mp;
        enter_world.cp            = character->stats.cp;
        enter_world.max_cp        = character->stats.max_cp;
        enter_world.sp            = character->sp;
        enter_world.exp           = character->exp;
        enter_world.sp            = character->sp;
        enter_world.p_attack      = character->stats.p_attack;
        enter_world.m_attack      = character->stats.m_attack;
        enter_world.p_def         = character->stats.p_def;
        enter_world.m_def         = character->stats.m_def;
        enter_world.evasion_rate  = character->stats.evasion_rate;
        enter_world.accuracy      = character->stats.accuracy;
        enter_world.critical_hit  = character->stats.critical_hit;
        enter_world.hair_style_id = character->hair_style;
        enter_world.hair_color_id = character->hair_color;
        enter_world.face          = character->face;
        // enter_world.access_level    = 1;
        enter_world.inventory_limit = 42;

        enter_world.run_speed      = character->stats.run_speed;
        enter_world.walk_speed     = character->stats.walk_speed;
        enter_world.p_attack_speed = character->stats.p_attack_speed;
        enter_world.m_attack_speed = character->stats.m_attack_speed;
        enter_world.movement_speed_multiplier =
                character->stats.movement_speed_multiplier;
        enter_world.attack_speed_multiplier =
                character->stats.attack_speed_multiplier;
        enter_world.collision_radius = character->collision_radius;
        enter_world.collision_height = character->collision_height;
        enter_world.name_color       = character->name_color;
        enter_world.current_load     = 1;
        enter_world.max_load         = character->max_load;
        enter_world.unknown          = 0x28;
        enter_world.name_color       = 0xFFFFFF;

        gs_packet_enter_world_pack(response, &enter_world);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);
}

// If packet is not NULL, the session will be updated and characters fetched.
// If packet is NULL, only the characters will be fetch (no session update).
static void handle_auth_login(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        static struct gs_packet_auth_login auth_login = { 0 };

        // Todo: confirm max amount of characters in lobby.
        static struct gs_character characters[10] = { 0 };

        static packet_t response[4096] = { 0 };

        struct gs_packet_auth_login_char *character = 0;

        struct gs_packet_auth_request auth_request = { 0 };

        char *username     = 0;
        size_t chars_max   = 0;
        size_t chars_found = 0;

        assert(gs);
        assert(session);

        bytes_zero((byte_t *) &auth_login, sizeof(auth_login));
        bytes_zero((byte_t *) characters, sizeof(characters));
        bytes_zero(response, sizeof(response));

        if (packet) {
                gs_packet_auth_request_unpack(&auth_request, packet);
                gs_session_update_auth(session, &auth_request);
        }

        username    = session->username;
        chars_max   = arr_size(characters);
        chars_found = storage_get_characters(characters, username, chars_max);

        auth_login.count = (u32_t) chars_found;

        for (size_t i = 0; i < chars_found; i += 1) {
                character = &auth_login.characters[i];

                l2_string_from_char(
                        character->name,
                        characters[i].name,
                        sizeof(character->name));

                character->playOK1       = session->playOK1;
                character->active        = 1;
                character->class_id      = characters[i]._class;
                character->race_id       = characters[i].race;
                character->face          = characters[i].face;
                character->hair_color_id = characters[i].hair_color;
                character->hair_style_id = characters[i].hair_style;
                character->hp            = characters[i].stats.hp;

                // Todo: double check, why are we using the session's id for the
                // character's id? Maybe this is why players are taking over
                // orcs? (bug)
                character->id     = session->id;
                character->level  = characters[i].level;
                character->max_hp = characters[i].stats.max_hp;
                character->max_mp = characters[i].stats.max_mp;
                character->mp     = characters[i].stats.mp;
                character->sex    = characters[i].sex;
                character->x      = characters[i].position.x;
                character->y      = characters[i].position.y;
                character->z      = characters[i].position.z;
                character->sp     = characters[i].sp;
                character->exp    = characters[i].exp;
        }

        gs_packet_auth_login_pack(response, &auth_login);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);
}

static void
handle_new_character(struct gs_state *gs, struct gs_session *session)
{
        // Hardcoded character templates
        // Todo: get from database?
        static struct gs_character_template templates[] = {
                // Human fighter
                { .race   = 0,
                  ._class = 0,
                  .stats  = { .str  = 40,
                             .dex  = 30,
                             .con  = 43,
                             ._int = 21,
                             .wit  = 11,
                             .men  = 25 } },

                // Human fighter copy
                // Not sure why the clients requires it...
                { .race   = 0,
                  ._class = 0,
                  .stats  = { .str  = 40,
                             .dex  = 30,
                             .con  = 43,
                             ._int = 21,
                             .wit  = 11,
                             .men  = 25 } },

                // Human mage
                { .race   = 0,
                  ._class = 10,
                  .stats  = { .str  = 22,
                             .dex  = 21,
                             .con  = 27,
                             ._int = 41,
                             .wit  = 20,
                             .men  = 39 } },

                // Elf figher
                { .race   = 1,
                  ._class = 18,
                  .stats  = { .str  = 36,
                             .dex  = 35,
                             .con  = 36,
                             ._int = 23,
                             .wit  = 14,
                             .men  = 26 } },

                // Elf mage
                { .race   = 1,
                  ._class = 25,
                  .stats  = { .str  = 21,
                             .dex  = 24,
                             .con  = 25,
                             ._int = 37,
                             .wit  = 23,
                             .men  = 40 } },

                // Dark elf fighter
                { .race   = 2,
                  ._class = 31,
                  .stats  = { .str  = 41,
                             .dex  = 34,
                             .con  = 32,
                             ._int = 25,
                             .wit  = 12,
                             .men  = 26 } },

                // Dark elf mage
                { .race   = 2,
                  ._class = 38,
                  .stats  = { .str  = 23,
                             .dex  = 23,
                             .con  = 24,
                             ._int = 44,
                             .wit  = 19,
                             .men  = 37 } },

                // Orc fighter
                { .race   = 3,
                  ._class = 44,
                  .stats  = { .str  = 40,
                             .dex  = 26,
                             .con  = 47,
                             ._int = 18,
                             .wit  = 12,
                             .men  = 27 } },

                // Orc mage
                { .race   = 3,
                  ._class = 49,
                  .stats  = { .str  = 27,
                             .dex  = 24,
                             .con  = 31,
                             ._int = 31,
                             .wit  = 15,
                             .men  = 42 } },

                // Dwarf
                { .race   = 4,
                  ._class = 53,
                  .stats  = { .str  = 39,
                             .dex  = 29,
                             .con  = 45,
                             ._int = 20,
                             .wit  = 10,
                             .men  = 27 } }
        };

        static struct gs_packet_new_char new_char = { 0 };

        static packet_t response[1024] = { 0 };

        assert(gs);
        assert(session);

        bytes_zero((byte_t *) &new_char, sizeof(new_char));
        bytes_zero(response, sizeof(response));

        new_char.count = (u32_t) arr_size(templates);

        for (u32_t i = 0; i < new_char.count; i += 1) {
                new_char.templates[new_char.count].race   = templates[i].race;
                new_char.templates[new_char.count]._class = templates[i]._class;
                new_char.templates[new_char.count].con = templates[i].stats.con;
                new_char.templates[new_char.count].dex = templates[i].stats.dex;
                new_char.templates[new_char.count].men = templates[i].stats.men;
                new_char.templates[new_char.count].str = templates[i].stats.str;
                new_char.templates[new_char.count].wit = templates[i].stats.wit;
                new_char.templates[new_char.count]._int =
                        templates[i].stats._int;
        }

        gs_packet_new_char_pack(response, &new_char);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);
}

static void handle_create_character(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        static packet_t response[128] = { 0 };

        struct gs_packet_create_char_request create_char_request = { 0 };

        struct gs_packet_create_char create_char = { 0 };

        struct gs_character character = { 0 };

        assert(gs);
        assert(session);
        assert(packet);

        bytes_zero(response, sizeof(response));

        gs_packet_create_char_request_unpack(&create_char_request, packet);
        gs_character_from_request(&character, &create_char_request);

        create_char.response =
                storage_create_character(session->username, &character);

        gs_packet_create_char_pack(response, &create_char);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);

        // When a new character is created, the user is taken
        // back to the lobby (where all his/her characters are)
        // this means we have to fetch and re-send the characters.
        // Using a null/empty packet for handle_auth_login
        // will make sure the session isn't updated.
        handle_auth_login(gs, session, 0);
}

static void handle_selected_character(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        static struct gs_packet_char_select char_select = { 0 };

        static packet_t response[512] = { 0 };

        struct gs_packet_char_select_request char_select_request = { 0 };

        struct gs_character character = { 0 };

        assert(gs);
        assert(session);
        assert(packet);

        bytes_zero((byte_t *) &char_select, sizeof(char_select));
        bytes_zero(response, sizeof(response));

        gs_packet_char_select_request_unpack(&char_select_request, packet);

        session->character_index = (size_t) char_select_request.index;

        storage_get_character(
                &character, session->username, session->character_index);

        character.id      = gs_character_get_free_id(gs);
        character.session = session;

        log_normal(
                "selecting character and assigning id %d with session %d",
                character.id,
                session->id);

        gs_character_add(gs, &character);

        char_select.playOK1 = session->playOK1;
        l2_string_from_char(
                char_select.name, character.name, sizeof(char_select.name));
        l2_string_from_char(
                char_select.title, character.title, sizeof(char_select.title));

        char_select.id        = character.id;
        char_select.active    = 1;
        char_select.race_id   = character.race;
        char_select.class_id  = character._class;
        char_select.exp       = character.exp;
        char_select.sp        = character.sp;
        char_select.level     = character.level;
        char_select.hp        = character.stats.hp;
        char_select.mp        = character.stats.mp;
        char_select.con       = character.stats.con;
        char_select.dex       = character.stats.dex;
        char_select.men       = character.stats.men;
        char_select.str       = character.stats.str;
        char_select.wit       = character.stats.wit;
        char_select._int      = character.stats._int;
        char_select.sex       = character.sex;
        char_select.x         = character.position.x;
        char_select.y         = character.position.y;
        char_select.z         = character.position.z;
        char_select.game_time = 42;

        gs_packet_char_select_pack(response, &char_select);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);
}

static void handle_quest_list(struct gs_state *gs, struct gs_session *session)
{
        struct gs_packet_quest_list quest_list = { 0 };

        packet_t response[32] = { 0 };

        assert(gs);
        assert(session);

        gs_packet_quest_list_pack(response, &quest_list);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);
}

static void handle_auto_ss_bsps(struct gs_state *gs, struct gs_session *session)
{
        struct gs_packet_d0 d0 = { 0 };

        packet_t response[32] = { 0 };

        assert(gs);
        assert(session);

        gs_packet_d0_pack(response, &d0);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);
}

static void protocol_version_state(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        assert(gs);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x00: // Protocol version
                handle_protocol_version(gs, session);
                session->state = AUTH_REQUEST;
                break;
        default:
                log_normal("can't handle packet from protocol version state.");
                break;
        }
}

static void auth_request_state(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        assert(gs);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x08: // Auth request
                handle_auth_login(gs, session, packet);
                session->state = CHARACTER_SELECTION;
                break;
        default:
                log_normal("can't handle packet from auth request state.");
                break;
        }
}

static void character_selection_state(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        assert(gs);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x0d: // Selected char.
                handle_selected_character(gs, session, packet);
                session->state = ENTERING_WORLD;
                break;
        case 0x0e: // New character
                handle_new_character(gs, session);
                session->state = CREATING_CHARACTER;
                break;
        default:
                log_normal(
                        "can't handle packet from character selection state.");
                break;
        }
}

static void creating_character_state(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        assert(gs);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x0b: // Create character
                handle_create_character(gs, session, packet);
                session->state = CHARACTER_SELECTION;
                break;
        default:
                /*
                 * When creating a new character, the user can go
                 * back to the character selection screen and no
                 * packet will be sent. This is why we also need to
                 * accept states from character selection.
                 */
                log_normal(
                        "packet delegated from creating character to character selection.");
                character_selection_state(gs, session, packet);
                break;
        }
}

static void entering_world_state(
        struct gs_state *gs,
        struct gs_session *session,
        packet_t *packet)
{
        assert(gs);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x03: // Enter world.
                log_normal("handling enter world.");
                handle_enter_world(gs, session);
                session->state = IN_WORLD;
                break;
        case 0x63: // Quest list.
                log_normal("handling quest list.");
                handle_quest_list(gs, session);
                break;
        case 0xd0: // Auto ss bsps.
                log_normal("handling auto ss bsps.");
                handle_auto_ss_bsps(gs, session);
                break;
        default:
                log_normal("can't handle packet from entering world state.");
                break;
        }
}

static void
in_world_state(struct gs_state *gs, struct gs_session *session, packet_t *packet)
{
        struct gs_character *character = 0;

        assert(gs);
        assert(session);
        assert(packet);

        character = gs_character_from_session(gs, session);

        if (!character) {
                log_normal("no character found for session. disconnect?");
                return;
        }

        gs_ai_handle_request(gs, character, packet);

        // Todo: refactor. We need to know if the restart was successful.
        // restart packet
        if (packet_type(packet) == 0x46) {
                handle_auth_login(gs, session, 0);
                session->state = CHARACTER_SELECTION;
        }
}

void gs_request_new_conn(struct gs_state *gs, struct os_io *socket)
{
        struct gs_session *session = 0;

        if (!gs) {
                log_normal(
                        "no state passed to gs_request_new_conn? ignoring request.");
                return;
        }

        if (!socket) {
                log_normal(
                        "no socket passed to gs_request_new_conn? ignoring request.");
                return;
        }

        session = gs_session_new(gs, socket);
        log_normal("new game session with id %d generated.", session->id);
}

void gs_request(struct gs_state *gs, struct os_io *socket, byte_t *buf, size_t n)
{
        // 65536 being the limit for a single packet.
        static packet_t packet[65536] = { 0 };

        struct gs_session *session = 0;

        size_t safe_packet_clean = 0;

        u16_t size = 0;

        if (!gs) {
                log_normal("no state passed to gs_request? ignoring request.");
                return;
        }

        if (!socket) {
                log_normal("no socket passed to gs_request? ignoring request.");
                return;
        }

        if (!buf || !n) {
                log_normal("empty request? ignoring.");
                return;
        }

        session = gs_session_find(gs, socket);

        if (!session) {
                log_normal(
                        "game server, no session found for request. ignoring.");
                return;
        }

        // Not sure how useful this may be, but it
        // may help reducing the amount of bytes required to
        // be reset.
        safe_packet_clean = _min(sizeof(packet), n * 2);
        bytes_zero(packet, safe_packet_clean);

        gs_session_decrypt(session, packet, buf);
        gs_session_encrypt_conn(session);

        switch (session->state) {
        case PROTOCOL_VERSION:
                protocol_version_state(gs, session, packet);
                break;
        case AUTH_REQUEST:
                auth_request_state(gs, session, packet);
                break;
        case CHARACTER_SELECTION:
                character_selection_state(gs, session, packet);
                break;
        case CREATING_CHARACTER:
                creating_character_state(gs, session, packet);
                break;
        case ENTERING_WORLD:
                entering_world_state(gs, session, packet);
                break;
        case IN_WORLD:
                in_world_state(gs, session, packet);
                break;
        default:
                log_normal("session in invalid state. disconnect?");
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                gs_request(gs, socket, buf + size, n - size);
        }
}

void gs_request_disconnect(struct gs_state *gs, struct os_io *socket)
{
        packet_t response[32] = { 0 };

        struct gs_session *session = 0;

        struct gs_character *character = 0;

        if (!gs) {
                log_normal(
                        "no state passed to gs_request_disconnect? ignoring request.");
                return;
        }

        if (!socket) {
                log_normal(
                        "no socket passed to gs_request_disconnect? ignoring request.");
                return;
        }

        session = gs_session_find(gs, socket);

        if (!session) {
                log_normal("disconnected client had no session, ignoring.");
                return;
        }

        character = gs_character_from_session(gs, session);

        if (character) {
                gs_character_disconnect(gs, character);
        }

        log_normal("sending disconnect packet.");

        gs_packet_leave_world_pack(response);

        gs_session_encrypt(session, response, response);
        gs_session_send_packet(gs, session, response);

        gs_session_disconnected(gs, session);
}

void gs_request_tick(struct gs_state *gs, double delta)
{
        struct gs_character *character = 0;

        if (!gs) {
                log_normal("no state for gs_request_tick? ignoring.");
                return;
        }

        list_each(struct gs_character, character, gs->list_characters)
        {
                gs_ai_tick(gs, character, delta);
        }
}
