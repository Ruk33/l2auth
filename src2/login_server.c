#include "include/l2auth.h"
#include "include/login_session.h"

static void on_auth_login(struct state *state, struct login_session *session)
{
    assert(state);
    assert(session);

    log("handling auth login.");

    struct request_auth_login auth_login = {0};
    request_auth_login_decode(&auth_login, &session->request);

    struct account account = {0};    
    if (storage_get_account(&account, &auth_login.username)) {
        if (!str_matches(account.password.buf, auth_login.password.buf)) {
            log("oh my! password incorrect.");
            login_session_drop(session);
            return;
        }
    } else {
        // try to auto create account.
        if (!storage_create_account(&auth_login.username, &auth_login.password)) {
            log("unable to auto-create account. dropping the client.");
            login_session_drop(session);
            return;
        }
    }

    struct response_auth_login_ok ok_response = {0};
    ok_response.login_ok1 = session->play_ok1;
    ok_response.login_ok2 = session->play_ok2;

    log("sending packet ok.");
    response_auth_login_ok_encode(&session->response, &ok_response);
    login_session_encrypt_packet(session, &session->response);
}

static void on_gg_auth(struct state *state, struct login_session *session)
{
    assert(state);
    assert(session);

    log("handling gg auth.");

    struct response_gg_auth gg_auth = {0};
    gg_auth.gg_response = 0x0b; // skip.

    log("sending response to gg auth (skip it)");
    response_gg_auth_encode(&session->response, &gg_auth);
    login_session_encrypt_packet(session, &session->response);
}

struct login_session *login_server_new_conn(struct state *state)
{
    assert(state);

    struct login_session *result = 0;
    for_each(struct login_session, session, state->sessions) {
        if (session->active)
            continue;
        result = session;
        break;
    }
    if (!result) {
        log("no more available sessions to use.");
        return 0;
    }
    if (!login_session_init(result)) {
        log("unable to initialize session.");
        return 0;
    }

    struct response_init init = {0};
    // todo: re-check from where does this session id comes from.
    struct session_id session_id = {{0xfd, 0x8a, 0x22, 0x00}};
    // chronicle 4 protocol only :)
    struct protocol c4_protocol = {{0x5a, 0x78, 0x00, 0x00}};
    init.session_id = session_id;
    init.protocol = c4_protocol;
    login_session_rsa_modulus(&init.modulus, result);
    response_init_encode(&result->response, &init);
    packet_checksum(&result->response);

    return result;
}

void login_server_request(struct state *state, struct login_session *session, void *buf, size_t n)
{
    assert(state);
    assert(session);

    // check for requests bigger than buffer.
    if (session->read + n > sizeof(session->request.buf)) {
        log("error! request too big to be handled.");                                                                                     
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

    login_session_decrypt_packet(session, &session->request);
    log("new packet received: 0x%x", packet_type(&session->request));
    switch (packet_type(&session->request)) {
    case 0x00: // auth login
        on_auth_login(state, session);
        break;
    case 0x02: // login server
        // on_login_server(state, session);
        break;
    case 0x05: // request server list
        // on_request_server_list(state, session);
        break;
    case 0x07: // gg auth
        on_gg_auth(state, session);
        break;
    default:
        log("received unknown packet. ignoring...");
        break;
    }

    zero(&session->request);
    session->read = 0;
}

void login_server_disconnect(struct state *state, struct login_session *session)
{
    assert(state);
    assert(session);
    login_session_release(session);
}
