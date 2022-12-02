#include "include/game_server.h"
#include "include/game_request.h"
#include "include/game_response.h"
#include "include/game_session.h"
#include "include/l2_string.h"
#include "include/storage.h"

static void on_protocol_version(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);

    struct protocol protocol = {{
        0x01,
        // crypt key
        0x94,
        0x35,
        0x00,
        0x00,
        0xa1,
        0x6c,
        0x54,
        0x87,
    }};
    struct response_protocol response_protocol = {0};
    response_protocol.protocol = protocol;

    log("handling protocol version.");
    response_protocol_encode(&session->response, &response_protocol);
}

static void on_auth_request(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);

    struct request_auth request = {0};
    struct response_auth_login response = {0};
    
    log("handling auth request.");
    request_auth_decode(&request, &session->request);
    l2_string_to_char_arr(session->username.buf, request.username.buf);
    log("%s user has access the game lobby.", session->username.buf);
    storage_get_characters(
        response.characters,
        &session->username,
        arr_len(response.characters),
        (int *) &response.count
    );
    log("sending %d characters found.", response.count);
    response_auth_login_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_show_creation_screen(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);

    struct response_show_creation_screen response = {0};
    log("handling show character creation screen.");

    // in this packet/response, we should include the character
    // templates but, it seems like it works without it so we
    // are not sending it. it really doesn't make sense to send
    // those templates since it's a place where the player could
    // potentially cheat (by setting the attributes to whatever)
    response_show_creation_screen_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_create_character(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);
    
    struct request_create_character request = {0};
    // todo: if i'm not mistaken, there is a create character response
    // packet, in which, we can tell if something went wrong (for instance,
    // the character's name is already being used)
    struct response_auth_login response = { 0 };
    request_create_character_decode(&request, &session->request);

    char name[32] = {0};
    l2_string_to_char(name, request.name.buf, sizeof(name));

    log(
        "player trying to create new character with name %s, "
        "race: %d, "
        "sex: %d, "
        "class: %d, "
        "hair style: %d, "
        "hair color: %d, "
        "face: %d\n",
        name,
        request.race_id,
        request.sex,
        request.class_id,
        request.hair_style_id,
        request.hair_color_id,
        request.face_id
    );

    // 1 - make sure the user can keep on creating character.
    // 2 - make sure the name is available.

    struct l2_character character = {0};
    character.game_time = 10;
    // talking island.
    character.x = -83968;
    character.y = 244634;
    character.z = -3730;
    character.name = request.name;
    character.play_ok1 = 1994;
    character.sex = request.sex;
    character.race_id = request.race_id;
    character.class_id = request.class_id;
    // todo: confirm what the active property does.
    character.active = 1;
    character.hp = 100;
    character.mp = 100;
    character.max_hp = character.hp;
    character.max_mp = character.mp;
    character.cp = 100;
    character.max_cp = character.cp;
    character.attrs._int = 10;
    character.attrs.con = 10;
    character.attrs.dex = 10;
    character.attrs.men = 10;
    character.attrs.str = 10;
    character.attrs.wit = 10;
    character.sp = 10;
    character.exp = 10;
    character.level = 1;
    character.hair_style_id = request.hair_style_id;
    character.hair_color_id = request.hair_color_id;
    character.face_id = request.face_id;
    character.p_attack = 10;
    character.m_attack = 10;
    character.p_def = 10;
    character.m_def = 10;
    character.evasion = 10;
    character.accuracy = 10;
    character.critical_hit = 10;
    character.inventory_limit = 10;
    character.run_speed = 200;
    character.walk_speed = 100;
    character.p_attack_speed = 10;
    character.m_attack_speed = 10;
    character.movement_speed_multiplier = 1;
    character.attack_speed_multiplier = 1;
    // todo: take a look at the real values from l2j.
    character.collision_radius = 20;
    character.collision_height = 20;
    character.name_color = 0xFFFFFF;
    character.current_load = 1;
    character.max_load = 10;

    storage_create_character(&session->username, &character);
    storage_get_characters(
        response.characters,
        &session->username,
        arr_len(response.characters),
        (int *) &response.count
    );
    
    log("sending %d characters found.", response.count);
    response_auth_login_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_select_character(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);
    struct request_selected_character request = {0};
    log("TODO: don't hardcode the values sent by the on select character function.");
    struct response_selected_character response = {0};

    request_selected_character_decode(&request, &session->request);
    log("the user is trying to access with the character at the index %d", request.index);

    // add the character to the character's pool.
    // first increase, and then use. this is because we can't use
    // id 0.
    state->characters_count++;
    session->character = state->characters + state->characters_count;
    storage_get_character(session->character, &session->username, request.index);
    session->character->id = state->characters_count;

    char name[32] = {0};
    l2_string_to_char_arr(name, &session->character->name);
    log("the user is trying to access with the character %s", name);

    // todo: make sure we found the character!
    response.id = session->character->id;
    response.play_ok1 = 1994;
    response.race_id = session->character->race_id;
    response.class_id = session->character->class_id;
    response.clan_id = session->character->clan_id;
    response.name = session->character->name;
    response.title = session->character->title;
    response.sex = session->character->sex;
    response.active = session->character->active;
    response.x = session->character->x;
    response.y = session->character->y;
    response.z = session->character->z;
    response.hp = session->character->hp;
    response.mp = session->character->mp;
    response.sp = session->character->sp;
    response.exp = session->character->exp;
    response.level = session->character->level;
    response.attrs = session->character->attrs;
    response.game_time = 10; // session->character->game_time;

    response_selected_character_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_auto_ss_bsps(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);
    struct response_d0 response = {0};
    response_d0_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_quest_list(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);
    struct response_quest_list response = {0};
    response_quest_list_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_enter_world(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);
    struct response_enter_world response = {0};
    
    // todo: not sure if we need to check if the user already has a character
    // assigned (since it should) but, something to keep in mind.
    assert(session->character);
    response.id = session->character->id;
    response.x = session->character->x;
    response.y = session->character->y;
    response.z = session->character->z;
    response.heading = 1; // ?
    response.name = session->character->name;
    response.race_id = session->character->race_id;
    response.sex = session->character->sex;
    response.class_id = session->character->class_id;
    response.level = session->character->level;
    response.exp = session->character->exp;
    response.attrs = session->character->attrs;
    response.max_hp = session->character->max_hp;
    response.hp = session->character->hp;
    response.max_mp = session->character->max_mp;
    response.mp = session->character->mp;
    response.sp = session->character->sp;
    response.current_load = session->character->current_load;
    response.max_load = session->character->max_load;
    response.p_attack = session->character->p_attack;
    response.p_attack_speed = session->character->p_attack_speed;
    response.p_def = session->character->p_def;
    response.evasion_rate = session->character->evasion;
    response.accuracy = session->character->accuracy;
    response.critical_hit = session->character->critical_hit;
    response.m_attack = session->character->m_attack;
    response.m_attack_speed = session->character->m_attack_speed;
    response.m_def = session->character->m_def;
    response.karma = session->character->karma;
    response.run_speed = session->character->run_speed;
    response.walk_speed = session->character->walk_speed;
    response.swim_run_speed = session->character->run_speed;
    response.swim_walk_speed = session->character->walk_speed;
    response.fly_run_speed = session->character->run_speed;
    response.fly_walk_speed = session->character->walk_speed;
    response.movement_speed_multiplier = session->character->movement_speed_multiplier;
    response.attack_speed_multiplier = session->character->attack_speed_multiplier;
    response.collision_radius = session->character->collision_radius;
    response.collision_height = session->character->collision_height;
    response.hair_style_id = session->character->hair_style_id;
    response.hair_color_id = session->character->hair_color_id;
    response.face_id = session->character->face_id;
    response.access_level = 1;
    response.title = session->character->title;
    response.clan_id = session->character->clan_id;
    // response.ally_id = session->character->ally_id;
    // response.dwarven_craft = session->character->
    // response.pk_kills = session->character->pk
    response.inventory_limit = session->character->inventory_limit;
    response.max_cp = session->character->max_cp;
    response.cp = session->character->cp;
    response.name_color = session->character->name_color;

    response_enter_world_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_restart(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);

    // todo: make sure the character can restart
    // (ie, it's not in aggro mode)

    struct response_restart response = {0};
    response.code = 1;
    response_restart_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_move(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);

    // todo: make sure the character can actually move
    // (ie, it's not rooted or stunt) and make sure
    // the destination makes sense. this is, the 
    // player, shouldn't be able to select a destination
    // too far away.

    struct request_move request = {0};
    request_move_decode(&request, &session->request);

    struct response_move response = {0};
    response.id = session->character->id;
    response.destination_x = request.destination_x;
    response.destination_y = request.destination_y;
    response.destination_z = request.destination_z;
    response.origin_x = request.origin_x;
    response.origin_y = request.origin_y;
    response.origin_z = request.origin_z;

    response_move_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_validate_position(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);

    // todo: make sure the character's position is properly
    // validated to avoid cheaters.

    struct request_validate_position request = {0};
    request_validate_position_decode(&request, &session->request);

    struct response_validate_position response = {0};
    response.id = session->character->id;
    response.x = request.x;
    response.y = request.y;
    response.z = request.z;
    response.heading = request.heading;

    response_validate_position_encode(&session->response, &response);
    game_session_encrypt_packet(session, &session->response);
}

static void on_game_server_new_request(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);

    u8 request_type = packet_type(&session->request);

    coroutine_begin(&session->state);

    // protocol
    if (request_type != 0x00) {
        log(
            "new player sent an incorrect packet. we were expecting the protocol "
            "packet first. to be safe, we will drop the player."
        );
        session->closed = 1;
        return;
    }
    on_protocol_version(state, session);
    yield;

    // auth request
    if (request_type != 0x08) {
        log(
            "new player sent an incorrect packet. we were expecting the auth request "
            "packet first. to be safe, we will drop the player."
        );
        session->closed = 1;
        return;
    }
    on_auth_request(state, session);

    // in character selection screen.
    // in the character selection, the player can click on
    // the create new character or he/she can enter into
    // the world. both actions are valid and must be handled.
in_character_selection:
    yield;
    switch (request_type) {
    // show character creation screen.
    case 0x0e:
        on_show_creation_screen(state, session);
        goto in_character_creation;
    // enter world.
    // case 0x03:
    //     // handle request.
    //     goto in_world;
    // selected character.
    case 0x0d:
        goto in_world;
    // ???
    case 0x9:
        log("todo: handle 0x9 packet. even tho the players gets disconnected after this.");
        return;
    default:
        log(
            "in character selection, the player sent an incorrect packet. "
            "we were expecting show character creation screen or, enter world, "
            "or 0x9 but we got 0x%x. just to be safe, we will drop the player.",
            request_type
        );
        session->closed = 1;
        return;
    }

// if the player is in the character creation screen and clicks cancel,
// (or go back to the character selection screen) we don't get a new packet
// that's why we have to accept or handle all the same packets we handle
// in the character selection screen. we could make use of gotos in here as 
// well, but duplicating the code seemed better, and easier to follow.
in_character_creation:
    yield;
    switch (request_type) {
    // show character creation screen.
    case 0x0e:
        on_show_creation_screen(state, session);
        return;
    // create new character.
    case 0x0b:
        on_create_character(state, session);
        goto in_character_selection;
    // ???
    case 0x9:
        log("todo: handle 0x9 packet. even tho the players gets disconnected after this.");
        return;
    // enter world.
    // case 0x03:
    //     // handle request.
    //     goto in_world;
    // select character.
    case 0x0d:
        goto in_world;
    default:
        log(
            "in character creation, the player sent an incorrect packet. "
            "we were expecting show character creation screen or, enter world, "
            "or create the character, or 0x9 but we got 0x%x. just to be safe, "
            "we will drop the player.",
            request_type
        );
        session->closed = 1;
        return;
    }

in_world:
    // yield;
    // selected character.
    if (request_type != 0x0d) {
        log("todo: we got unexpected packet, we were expecting 0x0d.");
        return;
    }
    on_select_character(state, session);
    yield;

    // auto ss bsps.
    if (request_type != 0xd0) {
        log("todo: we got unexpected packet, we were expecting 0xd0.");
        return;
    }
    on_auto_ss_bsps(state, session);
    yield;

    // quest list.
    if (request_type != 0x63) {
        log("todo: we got unexpected packet, we were expecting 0x63.");
        return;
    }
    on_quest_list(state, session);
    yield;

    // enter world.
    if (request_type != 0x03) {
        log("todo: we got unexpected packet, we were expecting 0x03.");
        return;
    }
    on_enter_world(state, session);
    yield;

    // todo: we may wanna move this to a new function
    // where it can have it's own coroutine state.

    switch (request_type) {
    case 0x01: // move
        on_move(state, session);
        break;
    case 0x46: // restart
        on_restart(state, session);
        goto in_character_selection;
        break;
    case 0x48: // validate the position
        on_validate_position(state, session);
        break;
    default:
        break;
    }

    log("oh no! we ran out of implementation code.");
    log("go back to the switch case.");
    return;

    coroutine_end;
}

struct game_session *game_server_new_conn(struct game_state *state)
{
    assert(state);
    log("got new connection, but will drop it!");

    struct game_session *new_session = 0;

    for_each(struct game_session, session, state->sessions) {
        if (session->active)
            continue;
        new_session = session;
        break;
    }
    if (!new_session) {
        log_err(
            "there is no more room for new players, "
            "maybe your server is already full. consider "
            "increasing the value of MAX_CONNECTIONS "
            "so more players can join your server. "
            "currently, the value of MAX_CONNECTIONS is "
            "%d",
            MAX_CONNECTIONS
        );
        return 0;
    }

    struct crypt_key key = {{0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87}};
    new_session->active = 1;
    new_session->id = l2_random();
    new_session->encrypt_key = key;
    new_session->decrypt_key = key;

    return new_session;
}

void game_server_request(struct game_state *state, struct game_session *session, void *buf, size_t n)
{
    assert(state);
    assert(session);

    // check for requests bigger than buffer.
    if (session->read + n > sizeof(session->request.buf)) {
        log_err("request too big to be handled.");                                                                                     
        return;
    }
    memcpy(session->request.buf + session->read, buf, n);
    session->read += n;

    // ignore if we haven't even read the packet size.
    if (session->read <= 2)
        return;

    u16 req_size = packet_size(&session->request);
    // check for partial request.
    if (session->read < (size_t) req_size)
        return;

    game_session_decrypt_packet(session, &session->request);
    session->conn_encrypted = 1;
    log("new packet received: 0x%x !", packet_type(&session->request));
    on_game_server_new_request(state, session);

    zero(&session->request);
    session->read = 0;
}

void game_server_disconnect(struct game_state *state, struct game_session *session)
{
    log("client got disconnected!");
}

