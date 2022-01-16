#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include "include/platform.h"
#include "include/config.h"
#include "include/util.h"
#include "include/storage.h"
#include "include/packet.h"
#include "include/ls_types.h"
#include "include/ls_session.h"
#include "include/ls_server_packets.h"
#include "include/ls_request.h"

static void handle_gg_auth(struct ls_state *ls, struct ls_session *session)
{
    packet_t response[16] = { 0 };

    struct ls_packet_gg_auth gg_auth = { 0 };

    assert(ls);
    assert(session);

    gg_auth.response = PACKET_GG_AUTH_RESPONSE_SKIP;

    ls_packet_gg_auth_pack(response, &gg_auth);
    ls_session_encrypt_packet(session, response, response);

    ls_session_send_packet(ls, session, response);
}

static void handle_auth_login(struct ls_state *ls,
                              struct ls_session *session,
                              packet_t *request)
{
    packet_t response[64] = { 0 };

    packet_t *body = 0;

    char username[14] = { 0 };
    char password[16] = { 0 };

    // Todo: make this configurable!
    char *salt = "somethign!";

    byte_t encrypted_password[64] = { 0 };

    struct ls_packet_ok ok = { 0 };

    struct ls_account account = { 0 };

    int account_exists = 0;
    int valid_password = 0;

    assert(ls);
    assert(session);
    assert(request);

    // Small little check for invalid or questionable request...
    // Todo: improve.
    if (packet_size(request) < 128) {
        log_normal(
            "seems like a questionable packet... disconnect just in case...");
        ls_session_disconnect(ls, session);
        return;
    }

    // Ignore byte containing packet type.
    body = packet_body(request) + 1;

    memcpy(username, body + 0x62, sizeof(username));
    memcpy(password, body + 0x70, sizeof(password));

    account_exists = storage_get_account(&account, username);

    if (!PKCS5_PBKDF2_HMAC(password,
                           strnlen(password, sizeof(password)),
                           (unsigned char *) salt,
                           strlen(salt),
                           1000,
                           EVP_sha512(),
                           64,
                           encrypted_password)) {
        log_normal("unable to encrypt password.");
        ls_session_disconnect(ls, session);
        return;
    }

    if (account_exists) {
        valid_password =
            CRYPTO_memcmp(account.encrypted_password, encrypted_password, 64) ==
            0;

        if (!valid_password) {
            log_normal("invalid password. disconnecting client.");
            ls_session_disconnect(ls, session);
            return;
        }
    } else {
        memcpy(account.username, username, sizeof(account.username));
        memcpy(account.encrypted_password, encrypted_password, 64);
        if (!storage_create_account(&account)) {
            log_normal("unable to create account.");
            ls_session_disconnect(ls, session);
            return;
        }
    }

    session->playOK1 = rand();
    session->playOK2 = rand();
    ok.loginOK1      = session->playOK1;
    ok.loginOK2      = session->playOK2;

    ls_packet_ok_pack(response, &ok);
    ls_session_encrypt_packet(session, response, response);

    ls_session_send_packet(ls, session, response);
}

static void handle_request_server_list(struct ls_state *ls,
                                       struct ls_session *session)
{
    static packet_t response[512] = { 0 };

    static struct ls_packet_server_list server_list = { 0 };

    assert(ls);
    assert(ls->text_ip_to_u32);
    assert(session);

    macro_util_set_arr_zero(response);
    util_set_zero(&server_list, sizeof(server_list));

    server_list.count =
        storage_get_servers(server_list.servers,
                            macro_util_arr_len(server_list.servers));

    for (size_t i = 0; i < server_list.count; i += 1) {
        log_normal("ip %s\n", server_list.servers[i].text_ip);
        server_list.servers[i].ip =
            ls->text_ip_to_u32(server_list.servers[i].text_ip);
    }

    ls_packet_server_list_pack(response, &server_list);

    ls_session_encrypt_packet(session, response, response);
    ls_session_send_packet(ls, session, response);
}

static void handle_login_server(struct ls_state *ls, struct ls_session *session)
{
    packet_t response[32] = { 0 };

    struct ls_packet_play_ok play_ok = { 0 };

    assert(ls);
    assert(session);

    log_normal("player wants to log into game server.");

    play_ok.playOK1 = session->playOK1;
    play_ok.playOK2 = session->playOK2;

    ls_packet_play_ok_pack(response, &play_ok);
    ls_session_encrypt_packet(session, response, response);

    ls_session_send_packet(ls, session, response);
}

void ls_request_new_conn(struct ls_state *ls, struct platform_socket *socket)
{
    static packet_t response[256]     = { 0 };
    static struct ls_packet_init init = { 0 };

    // Todo: re-check from where does this session id comes from.
    byte_t session_id[] = { 0xfd, 0x8a, 0x22, 0x00 };

    // Chronicle 4 protocol only :)
    byte_t protocol[] = { 0x5a, 0x78, 0x00, 0x00 };

    struct ls_session *session = 0;

    assert(ls);
    assert(socket);

    macro_util_set_arr_zero(response);
    util_set_zero(&init, sizeof(init));

    session = ls_session_new(ls, socket);
    assert(session);

    macro_util_cpy_bytes_to_arr(init.session_id,
                                session_id,
                                sizeof(init.session_id));
    macro_util_cpy_bytes_to_arr(init.protocol, protocol, sizeof(init.protocol));
    ls_session_rsa_modulus(session, init.modulus);

    ls_packet_init_pack(response, &init);

    ls_session_send_packet(ls, session, response);
}

void ls_request(struct ls_state *ls,
                struct platform_socket *socket,
                byte_t *buf,
                size_t n)
{
    static packet_t packet[2048] = { 0 };

    struct ls_session *session = 0;

    u16_t size = 0;

    assert(ls);
    assert(socket);

    session = ls_session_find(ls, socket);

    if (!session) {
        log_normal("session not found. ignoring request.");
        return;
    }

    macro_util_set_arr_zero(packet);
    ls_session_decrypt_packet(session, packet, buf);

    switch (packet_type(packet)) {
    case 0x00: // Auth login
        handle_auth_login(ls, session, packet);
        break;
    case 0x02: // Login server
        handle_login_server(ls, session);
        break;
    case 0x05: // Request server list
        handle_request_server_list(ls, session);
        break;
    case 0x07: // GG Auth
        handle_gg_auth(ls, session);
        break;
    default:
        log_normal("ignoring unknown packet.");
        break;
    }

    size = packet_size(buf);

    // There can be multiple packets inside of buf.
    if (size < n) {
        ls_request(ls, socket, buf + size, n - size);
    }
}

void ls_request_disconnect(struct ls_state *ls, struct platform_socket *socket)
{
    struct ls_session *session = 0;

    assert(ls);
    assert(socket);

    session = ls_session_find(ls, socket);

    if (!session) {
        return;
    }

    log_normal("client disconnected from login server.");
    ls_session_disconnected(ls, session);
}
