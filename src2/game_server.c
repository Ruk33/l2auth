#include "include/game_server.h"

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
    
    printf("handling auth request.\n");
    request_auth_decode(&request, &session->request);
    l2_string_to_char_arr(session->username.buf, request.username.buf);

    // response.count = (u32) get_account_characters(
    //     response.characters,
    //     &session->username,
    //     ARR_LEN(response.characters)
    // );

    response_auth_login_encode(&session->response, &response);
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
    yield;

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

