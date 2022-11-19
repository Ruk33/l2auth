#include "include/game_server.h"
#include "include/game_response.h"
#include "include/l2_string.h"

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

    // response.count = (u32) get_account_characters(
    //     response.characters,
    //     &session->username,
    //     ARR_LEN(response.characters)
    // );
    response.count = 1;
    l2_string_from_char_arr(response.characters[0].name.buf, "ruke");
    response.characters[0].active = 1;
    response.characters[0].accuracy = 10;
    response.characters[0].attack_speed_multiplier = 1;
    response.characters[0].m_attack = 1;
    response.characters[0].p_attack = 1;
    response.characters[0].p_attack_speed = 1;
    response.characters[0].m_attack_speed = 1;
    response.characters[0].id = 1;
    response.characters[0].x = 1;
    response.characters[0].y = 1;
    response.characters[0].z = 1;
    response.characters[0].race_id = 1;
    response.characters[0].class_id = 1;
    response.characters[0].sex = 1;
    response.characters[0].hp = 10;
    response.characters[0].mp = 10;
    response.characters[0].cp = 10;
    response.characters[0].max_hp = 10;
    response.characters[0].max_mp = 10;
    response.characters[0].max_cp = 10;
    response.characters[0].face_id = 1;
    response.characters[0].hair_color_id = 1;
    response.characters[0].name_color = 0xFFFFFF;
    response.characters[0].level = 1;

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
    // struct server_packet_create_character response = { 0 };
    // struct auth_login response = {0};
    // struct character character = {0};
    // struct character_template *template = 0;
    
    request_create_character_decode(&request, &session->request);

    char name[32] = {0};
    l2_string_to_char(name, request.name.buf, sizeof(name));

    log(
        "create new character with name %s, "
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
    
    // template = get_character_template_by_class(request.class_id);
    
    // if (template) {
    //     printf("template found.\n");
    //     character.name = request.name;
    //     character.race_id = request.race_id;
    //     character.sex = request.sex;
    //     character.class_id = request.class_id;
    //     character.hair_style_id = request.hair_style_id;
    //     character.hair_color_id = request.hair_color_id;
    //     character.face_id = request.face_id;
    //     character.attrs = template->attrs;
    //     character.hp = 40;
    //     character.mp = 40;
    //     character.max_hp = 40;
    //     character.max_mp = 40;
    //     character.level = 1;
    //     // Talking island.
    //     character.position.x = -83968;
    //     character.position.y = 244634;
    //     character.position.z = -3730;

    //     save_character(&session->username, &character);
    //     printf("character created and saved.\n");
    // }
    
    // response.count = (u32) get_account_characters(
    //     response.characters,
    //     &session->username,
    //     ARR_LEN(response.characters)
    // );
    // printf("%d characters found from account %s.\n", response.count, (char *) session->username.buf);
    
    // encode_auth_login(&session->response, &response);
    // encrypt_packet(session, &session->response);
}

static void on_select_character(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);
    log("TODO: don't hardcode the values sent by the on select character function.");
    struct response_selected_character response = {0};

    l2_string_from_char_arr(response.name.buf, "ruke");
    response.play_ok1 = 1994;
    response.id = 1;
    response.active = 1;
    response.race_id = 1;
    response.class_id = 1;
    response.exp = 1;
    response.sp = 1;
    response.level = 1;
    response.hp = 10;
    response.mp = 10;
    response.attrs._int = 10;
    response.attrs.con = 10;
    response.attrs.dex = 10;
    response.attrs.men = 10;
    response.attrs.str = 10;
    response.attrs.wit = 10;
    // talking island!
    // -83968;
    // 244634;
    // -3730;
    response.x = -83968;
    response.y = 244634;
    response.z = -3730;

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
    l2_string_from_char_arr(response.name.buf, "ruke");
    response.id = 1;
    response.race_id = 1;
    response.class_id = 1;
    response.exp = 1;
    response.sp = 1;
    response.level = 1;
    response.hp = 10;
    response.mp = 10;
    response.cp = 10;
    response.max_hp = 10;
    response.max_mp = 10;
    response.max_cp = 10;
    response.attrs._int = 10;
    response.attrs.con = 10;
    response.attrs.dex = 10;
    response.attrs.men = 10;
    response.attrs.str = 10;
    response.attrs.wit = 10;
    // talking island!
    // -83968;
    // 244634;
    // -3730;
    response.x = -83968;
    response.y = 244634;
    response.z = -3730;
    response.p_attack = 10;
    response.p_def = 10;
    response.m_attack = 10;
    response.m_def = 10;
    response.evasion_rate = 10;
    response.accuracy = 10;
    response.critical_hit = 10;
    response.hair_color_id = 1;
    response.hair_style_id = 1;
    response.face_id = 1;
    response.inventory_limit = 10;
    response.run_speed = 200;
    response.walk_speed = 100;
    response.p_attack_speed = 10;
    response.m_attack_speed = 10;
    response.movement_speed_multiplier = 1;
    response.attack_speed_multiplier = 1;
    response.collision_radius = 200;
    response.collision_height = 200;
    response.name_color = 0xffffff;
    response.current_load = 1;
    response.max_load = 1;

    response_enter_world_encode(&session->response, &response);
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

    log("oh no! we ran out of implementation code.");

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

