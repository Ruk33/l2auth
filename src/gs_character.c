#include <assert.h>
#include "include/config.h"
#include "include/util.h"
#include "include/log.h"
#include "include/packet.h"
#include "include/conn.h"
#include "include/l2_string.h"
#include "include/gs_session.h"
#include "include/gs_packet_create_char_request.h"
#include "include/gs_packet_move.h"
#include "include/gs_packet_move_request.h"
#include "include/gs_packet_validate_pos.h"
#include "include/gs_packet_validate_pos_request.h"
#include "include/gs_character.h"

static gs_character_t *characters = 0;
static size_t *character_count    = 0;

static packet_t response[65536] = { 0 };

static void handle_move_request(gs_character_t *character, packet_t *packet)
{
        gs_packet_move_request_t move_request = { 0 };
        gs_packet_move_t move_response        = { 0 };

        assert(character);
        assert(packet);

        gs_packet_move_request_unpack(&move_request, packet);

        character->target_x = move_request.x;
        character->target_y = move_request.y;
        character->target_z = move_request.z;

        bytes_zero(response, sizeof(response));

        // Todo: notify close players.
        gs_packet_move(&move_response, character);
        gs_packet_move_pack(response, &move_response);

        gs_session_encrypt(character->session, response, response);
        conn_send_packet(character->session->socket, response);
}

static void
handle_validate_position_request(gs_character_t *character, packet_t *packet)
{
        gs_packet_validate_pos_request_t validate_request = { 0 };
        gs_packet_validate_pos_t validate_response        = { 0 };

        assert(character);
        assert(packet);

        gs_packet_validate_pos_request_unpack(&validate_request, packet);

        // Todo: Perform basic validation.
        // If it's close enough to current location, update. If not, don't.
        character->x       = validate_request.x;
        character->y       = validate_request.y;
        character->z       = validate_request.z;
        character->heading = validate_request.heading;

        bytes_zero(response, sizeof(response));

        gs_packet_validate_pos(&validate_response, character);
        gs_packet_validate_pos_pack(response, &validate_response);

        gs_session_encrypt(character->session, response, response);
        conn_send_packet(character->session->socket, response);
}

static void spawn_state(gs_character_t *character, packet_t *packet)
{
        assert(character);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x01: // Move backwards.
                handle_move_request(character, packet);
                break;
        case 0x04: // Action.
                log("TODO: Action");
                break;
        case 0x09: // Logout.
                log("TODO: Logout");
                break;
        case 0x38: // Say.
                log("TODO: Say");
                break;
        case 0x46: // Restart.
                log("TODO: Restart");
                break;
        case 0x48: // Validate position.
                handle_validate_position_request(character, packet);
                break;
        case 0xcd: // Show map.
                log("TODO: Show map");
                break;
        default:
                log("Can't handle packet from in world state.");
                break;
        }
}

void gs_character_set(gs_character_t *src, size_t *count)
{
        characters      = src;
        character_count = count;
}

void gs_character_from_request(
        gs_character_t *dest,
        gs_packet_create_char_request_t *src)
{
        assert(dest);
        assert(src);

        l2_string_to_char(dest->name, src->name, sizeof(dest->name));

        dest->race       = src->race;
        dest->_class     = src->_class;
        dest->sex        = src->sex;
        dest->face       = src->face;
        dest->hair_color = src->hair_color;
        dest->hair_style = src->hair_style;
        dest->con        = src->con;
        dest->dex        = src->dex;
        dest->men        = src->men;
        dest->str        = src->str;
        dest->wit        = src->wit;
        dest->_int       = src->_int;

        // Talking Island
        dest->x = -83968;
        dest->y = 244634;
        dest->z = -3730;

        dest->level                     = 1;
        dest->exp                       = 50;
        dest->sp                        = 10;
        dest->hp                        = 400;
        dest->mp                        = 400;
        dest->cp                        = 200;
        dest->max_hp                    = 400;
        dest->max_mp                    = 400;
        dest->max_cp                    = 200;
        dest->p_attack                  = 42;
        dest->m_attack                  = 42;
        dest->p_def                     = 42;
        dest->m_def                     = 42;
        dest->evasion_rate              = 2;
        dest->accuracy                  = 2;
        dest->critical_hit              = 2;
        dest->run_speed                 = 120;
        dest->walk_speed                = 100;
        dest->p_attack_speed            = 20;
        dest->m_attack_speed            = 20;
        dest->movement_speed_multiplier = 1;
        dest->attack_speed_multiplier   = 1;
        dest->collision_radius          = 9;
        dest->collision_height          = 23;
        dest->max_load                  = 42;
        dest->name_color                = 0xFFFFFF;
}

void gs_character_spawn(gs_session_t *session, gs_character_t *src)
{
        assert(session);
        assert(src);

        for (size_t i = 0; i < *character_count; i += 1) {
                log("Notify close players.");
        }

        log("Spawning new character and linking gs session.");

        characters[*character_count]         = *src;
        characters[*character_count].state   = SPAWN;
        characters[*character_count].id      = session->id;
        characters[*character_count].session = session;

        *character_count += 1;
}

gs_character_t *gs_character_from_session(gs_session_t *session)
{
        assert(session);

        for (size_t i = 0; i < *character_count; i += 1) {
                if (characters[i].session == session) {
                        return &characters[i];
                }
        }

        return 0;
}

void gs_character_request(gs_character_t *character, packet_t *packet)
{
        switch (character->state) {
        case SPAWN:
                spawn_state(character, packet);
                break;
        case IDLE:
                break;
        case MOVING:
                break;
        default:
                break;
        }
}
