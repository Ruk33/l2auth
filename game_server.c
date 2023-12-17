#define WIN32_LEAN_AND_MEAN

#include <assert.h> // assert
#include <stdlib.h> // calloc, size_t
#include <stdio.h>  // fprintf
#include <string.h> // memset, memcpy, strncpy, strnlen
#include <stdint.h> // fixed int types
#include <locale.h> // setlocale
#include <wchar.h>  // wchar_t
#include <time.h>   // time_t, now
#include <stdarg.h> // va_arg, va_list

#include "directory.h"
#include "asocket.h"

typedef uint8_t byte;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float seconds;

#define nl "\n"
#define mb *1024*1024
#define trace(...) fprintf(stderr, __VA_ARGS__)
#define warn(...) trace("warning / " __VA_ARGS__)
#define error(...) trace("error / " __VA_ARGS__)
#define countof(x) (sizeof(x) / sizeof(*(x)))

#define bprintf(dest, n, ...) \
bprintf_((dest), (n), __VA_ARGS__, 0)

#define bscanf(src, n, ...) \
bscanf_((src), (n), __VA_ARGS__, 0)

#define pscanf(src, ...) \
bscanf_((src) + 3, packet_size(src) - 3, __VA_ARGS__, 0)

#define push_response(conn, _encrypt, ...) \
push_response_((conn), (_encrypt), __VA_ARGS__, 0)

#define coroutine(x) \
struct coroutine *__coro = &(x); \
switch (__coro->line) \
case 0:

#define yield \
__coro->line = __COUNTER__ + 1; \
break; \
case __COUNTER__:

#define syield(sleep, delta) \
__coro->sleep_for = (sleep); \
yield; \
__coro->sleep_for -= (delta); \
if (coro->sleep_for > 0) \
break;

struct coroutine {
    u32 line;
    seconds sleep_for;
};

struct connection {
    int socket;
    int encrypted;
    
    u32 login_ok1;
    u32 login_ok2;
    
    u32 play_ok1;
    u32 play_ok2;
    
    wchar_t username[32];
    
    byte encrypt_key[8];
    byte decrypt_key[8];
    
    struct coroutine state;
    
    byte to_send[65535 * 4];
    size_t to_send_count;
    size_t sent;
    
    byte request[65535 * 2];
    size_t request_count;
    
    struct character *character;
};

struct character {
    int active;
    wchar_t name[32];
    u32 template_id;
    s32 x;
    s32 y;
    s32 z;
    s32 heading;
    u32 race_id;
    u32 sex;
    u32 class_id;
    u32 level;
    u32 xp;
    struct attributes {
        u32 str;
        u32 dex;
        u32 con;
        u32 _int;
        u32 wit;
        u32 men;
    } attributes;
    double max_hp;
    double current_hp;
    double max_mp;
    double current_mp;
    double max_cp;
    double current_cp;
    u32 sp;
    u32 current_load;
    u32 max_load;
    u32 p_atk;
    u32 p_atk_speed;
    u32 p_def;
    u32 evasion_rate;
    u32 accuracy;
    u32 critical_hit;
    u32 m_atk;
    u32 m_atk_speed;
    u32 m_def;
    u32 pvp_flag;
    u32 karma;
    u32 run_speed;
    u32 walk_speed;
    u32 swim_run_speed;
    u32 swim_walk_speed;
    u32 fly_run_speed;
    u32 fly_walk_speed;
    double movement_speed_multiplier;
    double atk_speed_multiplier;
    double collision_radius;
    double collision_height;
    u32 hair_style_id;
    u32 hair_color_id;
    u32 face_id;
    u32 access_level;
    wchar_t title[32];
    u32 clan_id;
    u32 crest_id;
    u32 ally_id;
    u32 ally_crest_id;
    u8 mount_type;
    u8 private_store_type;
    u8 dwarven_craft;
    u32 pk_kills;
    u32 pvp_kills;
    u16 cubics;
    u8 party_members;
    u32 abnormal_effect;
    u32 clan_privileges;
    u16 recommendations_left;
    u16 recommendations_have;
    u16 inventory_limit;
    u8 mounted;
    u32 clan_crest_large_id;
    u8 hero_symbol;
    u8 hero;
    s32 fish_x;
    s32 fish_y;
    s32 fish_z;
    u32 name_color;
    
    enum action_type {
        idle,
        moving,
        attacking,
    } action_type;
    
    union action_payload {
        struct {
            s32 target_x;
            s32 target_y;
            s32 target_z;
        } moving;
        
    } action_payload;
};

struct state {
    float d;
    struct connection connections[1024];
    struct character characters[1024];
};

static struct connection *get_connection_from_socket(struct state *state, int socket);

static void encrypt(struct connection *conn, byte *packet);
static void decrypt(struct connection *conn, byte *request);

static void handle_request(struct state *state, struct connection *conn);
static void handle_send_protocol(struct state *state, struct connection *conn);
static void handle_auth(struct state *state, struct connection *conn, byte *req);
static void handle_show_create_character_screen(struct state *state, struct connection *conn);
static void handle_character_creation(struct state *state, struct connection *conn, byte *req);
static void handle_send_character_list(struct state *state, struct connection *conn);
static void handle_select_character(struct state *state, struct connection *conn, byte *req);
static void handle_auto_ss_bsps(struct state *state, struct connection *conn);
static void handle_send_quest_list(struct state *state, struct connection *conn);
static void handle_enter_world(struct state *state, struct connection *conn);
static void handle_leave_world(struct state *state, struct connection *conn);
static void handle_restart(struct state *state, struct connection *conn);
static void handle_movement(struct state *state, struct connection *conn, byte *req);
static void handle_validate_position(struct state *state, struct connection *conn, byte *req);
static void handle_show_map(struct state *state, struct connection *conn);

static void move_to(struct state *state, struct connection *conn, s32 x, s32 y, s32 z);

static void moving_update(struct state *state, struct character *character);

static byte *bscanf_va(byte *src, size_t n, va_list va)
{
    byte *tail = src;
    while ((size_t) (tail - src) < n) {
        char *fmt = va_arg(va, char *);
        if (fmt == 0)
            break;
        if (fmt[0] == '%' && fmt[1] == 'l' && fmt[2] == 's') {
            size_t max_letters = va_arg(va, size_t);
            wchar_t *dest = va_arg(va, wchar_t *);
            wcsncpy(dest, (const wchar_t *) tail, max_letters - 1);
            tail += (wcsnlen(dest, max_letters) + 1) * 2;
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'l' && fmt[2] == 'f') {
            double *dest = va_arg(va, double *);
            *dest = *(double *) tail;
            tail += sizeof(*dest);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'u') {
            u32 *dest = va_arg(va, u32 *);
            *dest = *(u32 *) tail;
            tail += sizeof(*dest);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'h') {
            u16 *dest = va_arg(va, u16 *);
            *dest = *(u16 *) tail;
            tail += sizeof(*dest);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'c') {
            u8 *dest = va_arg(va, u8 *);
            *dest = *tail;
            tail += sizeof(*dest);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'x') {
            size_t bytes_to_skip = va_arg(va, size_t);
            tail += bytes_to_skip;
            continue;
        }
        
        error("unexpected format '%s' in bscanf." nl, fmt);
        assert(0);
    }
    return tail;
}

static byte *bscanf_(byte *src, size_t n, ...)
{
    va_list va;
	va_start(va, n);
	byte *result = bscanf_va(src, n, va);
	va_end(va);
    return result;
}

static byte *bprintf_va(byte *dest, size_t n, va_list va)
{
    byte *tail = dest;
    while ((size_t) (tail - dest) < n) {
        char *fmt = va_arg(va, char *);
        if (fmt == 0)
            break;
        if (fmt[0] == '%' && fmt[1] == 'l' && fmt[2] == 's') {
            size_t max_letters = va_arg(va, size_t);
            wchar_t *src = va_arg(va, wchar_t *);
            wcsncpy((wchar_t *) tail, src, max_letters - 1);
            tail += (wcsnlen(src, max_letters) + 1) * 2;
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'u') {
            u32 src = va_arg(va, u32);
            *(u32 *) tail = src;
            tail += sizeof(src);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'h') {
            u16 src = va_arg(va, u16);
            *(u16 *) tail = src;
            tail += sizeof(src);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'c') {
            u8 src = va_arg(va, u8);
            *tail = src;
            tail += sizeof(src);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'l' && fmt[2] == 'f') {
            double src = va_arg(va, double);
            *(double *) tail = src;
            tail += sizeof(src);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'x') {
            size_t bytes_to_skip = va_arg(va, size_t);
            memset(tail, 0, bytes_to_skip);
            tail += bytes_to_skip;
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'b') {
            size_t bytes_to_write = va_arg(va, size_t);
            byte *src = va_arg(va, byte *);
            memcpy(tail, src, bytes_to_write);
            tail += bytes_to_write;
            continue;
        }
        
        error("unexpected format '%s' in bprintf." nl, fmt);
        assert(0);
    }
    return tail;
}

static byte *bprintf_(byte *dest, size_t n, ...)
{
    va_list va;
	va_start(va, n);
	byte *result = bprintf_va(dest, n, va);
	va_end(va);
    return result;
}

static u16 packet_size(byte *src)
{
    u16 size = *(u16 *) src;
    return size;
}

static void push_response_(struct connection *conn, int _encrypt, ...)
{
    assert(conn);
    
    byte *start = conn->to_send + conn->to_send_count;
    size_t max = sizeof(conn->to_send) - (size_t) (start - conn->to_send);
    
    va_list va;
    va_start(va, _encrypt);
    byte *end = bprintf_va(start, max, va);
    va_end(va);
    
    assert((end - start) < 65535);
    u16 size = (u16) (end - start);
    *(u16 *) start = size;
    
    if (_encrypt)
        encrypt(conn, start);
    
    conn->to_send_count += size;
}

static void encrypt(struct connection *conn, byte *packet)
{
    assert(conn);
    assert(packet);
    
    u16 size = *(u16 *) packet;
    if (size > 1)
        size -= 2;
    
    byte *body = packet + 2;
    
    u32 temp = 0;
    u32 temp2 = 0;
    for (u16 i = 0; i < size; i++) {
        temp2 = body[i] & 0xff;
        body[i] = (byte) (temp2 ^ conn->encrypt_key[i & 7] ^ temp);
        temp = body[i];
    }
    
    u32 old = ((u32) conn->encrypt_key[0] & 0xff);
    old |= ((u32) conn->encrypt_key[1] << 8 & 0xff00);
    old |= ((u32) conn->encrypt_key[2] << 0x10 & 0xff0000);
    old |= ((u32) conn->encrypt_key[3] << 0x18 & 0xff000000);
    
    old += size;
    
    conn->encrypt_key[0] = (byte) (old & 0xff);
    conn->encrypt_key[1] = (byte) (old >> 0x08 & 0xff);
    conn->encrypt_key[2] = (byte) (old >> 0x10 & 0xff);
    conn->encrypt_key[3] = (byte) (old >> 0x18 & 0xff);
}

static void decrypt(struct connection *conn, byte *request)
{
    assert(conn);
    assert(request);
    
    u16 size = *(u16 *) request;
    if (size > 1)
        size -= 2;
    
    byte *body = request + 2;
    u32 temp = 0;
    u32 temp2 = 0;
    for (u16 i = 0; i < size; i++) {
        temp2 = body[i];
        body[i] = (byte) (temp2 ^ conn->decrypt_key[i & 7] ^ temp);
        temp = temp2;
    }
    
    u32 old = conn->decrypt_key[0] & 0xff;
    old |= conn->decrypt_key[1] << 8 & 0xff00;
    old |= conn->decrypt_key[2] << 0x10 & 0xff0000;
    old |= conn->decrypt_key[3] << 0x18 & 0xff000000;
    
    old += size;
    
    conn->decrypt_key[0] = (byte) (old & 0xff);
    conn->decrypt_key[1] = (byte) (old >> 0x08 & 0xff);
    conn->decrypt_key[2] = (byte) (old >> 0x10 & 0xff);
    conn->decrypt_key[3] = (byte) (old >> 0x18 & 0xff);
}

int on_init(void **buf)
{
    assert(buf);
    size_t to_alloc = 512 mb;
    assert(to_alloc > sizeof(struct state));
    *buf = calloc(1, to_alloc);
    if (!*buf) {
        trace("unable to allocate memory for game server" nl);
        return 0;
    }
    setlocale(LC_ALL, "");
    return 1;
}

void on_connection(void **buf, int socket)
{
    assert(buf);
    
    struct state *state = *(struct state **) buf;
    assert(state);
    trace("new connection to game server!" nl);
    struct connection *connection = get_connection_from_socket(state, socket);
    if (!connection) {
        trace("there is no more space to accept new players. dropping new connection" nl);
        asocket_close(socket);
        return;
    }
    memset(connection, 0, sizeof(*connection));
    connection->socket = socket;
    
    byte key[] = {0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87};
    memcpy(connection->encrypt_key, key, sizeof(key));
    memcpy(connection->decrypt_key, key, sizeof(key));
}

void on_request(void **buf, int socket, void *request, size_t len)
{
    assert(buf);
    struct state *state = *(struct state **) buf;
    assert(state);
    struct connection *conn = get_connection_from_socket(state, socket);
    if (!conn) {
        trace("request for a non connected client?" nl);
        return;
    }
    memcpy(conn->request + conn->request_count, request, len);
    conn->request_count += len;
    handle_request(state, conn);
}

void on_response(void **buf, int socket)
{
    assert(buf);
    struct state *state = *(struct state **) buf;
    assert(state);
    struct connection *conn = get_connection_from_socket(state, socket);
    if (!conn) {
        trace("answering a non connected client?" nl);
        return;
    }
    if (conn->sent < conn->to_send_count) {
        trace("sending %lu bytes of data" nl, conn->to_send_count - conn->sent);
        conn->sent += asocket_write(conn->socket,
                                    conn->to_send + conn->sent,
                                    conn->to_send_count - conn->sent);
    }
    // reset counters when all data has been sent.
    if (conn->sent >= conn->to_send_count) {
        conn->sent = 0;
        conn->to_send_count = 0;
    }
}

void on_disconnect(void **buf, int socket)
{
    assert(buf);
    struct state *state = *(struct state **) buf;
    assert(state);
    struct connection *conn = get_connection_from_socket(state, socket);
    if (!conn)
        return;
    trace("client disconnected" nl);
    conn->socket = 0;
}

// int64_t millis()
// {
// struct timespec now;
// timespec_get(&now, TIME_UTC);
// return (((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000) / 100000000000;
// }

int on_tick(void **buf)
{
    assert(buf);
    struct state *state = *buf;
    
    state->d = 0.17f;
    // returns 17
    // trace("clock %d" nl, millis());
    
    for (int i = 0; i < countof(state->characters); i++) {
        struct character *character = state->characters + i;
        if (!character->active)
            continue;
        switch (character->action_type) {
            case idle:
            break;
            case moving:
            moving_update(state, character);
            break;
            case attacking:
            break;
            default:
            break;
        }
    }
    
    return 1;
}

static struct connection *get_connection_from_socket(struct state *state, int socket)
{
    assert(state);
    for (size_t i = 0; i < countof(state->connections); i++) {
        if (state->connections[i].socket == socket)
            return state->connections + i;
    }
    // find a free connection to be used for this socket.
    for (size_t i = 0; i < countof(state->connections); i++) {
        if (!state->connections[i].socket)
            return state->connections + i;
    }
    return 0;
}

static void handle_request(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    byte *request = conn->request;
    
    u16 size = *(u16 *) request;
    if (size > conn->request_count)
        return;
    
    if (conn->encrypted)
        decrypt(conn, request);
    
    byte type = *(request + 2);
    trace("received packet is of type %#04x" nl, (int) type);
    
    // check if the received packet matches the expected packet.
    // if it doesn't, drop/close the connection.
#define expected_or_close(expected) \
if (type != (expected)) { \
error("expecting packet %d but got %d. the connection will be dropped." nl, \
(expected), \
type); \
asocket_close(conn->socket); \
conn->socket = 0; \
return; \
}
    
    coroutine(conn->state) {
        // expect protocol request.
        expected_or_close(0x00);
        handle_send_protocol(state, conn);
        conn->encrypted = 1;
        
        yield;
        // expect auth request.
        expected_or_close(0x08);
        handle_auth(state, conn, request);
        
        yield;
        switch (type) {
            // show create character screen.
            case 0x0e:
            handle_show_create_character_screen(state, conn);
            break;
            // create character.
            case 0x0b:
            handle_character_creation(state, conn, request);
            break;
            // select character
            case 0x0d:
            handle_select_character(state, conn, request);
            break;
            // bsps
            case 0xd0:
            handle_auto_ss_bsps(state, conn);
            break;
            // quest list
            case 0x63:
            handle_send_quest_list(state, conn);
            break;
            // enter world
            case 0x03:
            handle_enter_world(state, conn);
            break;
            // quit
            case 0x09:
            handle_leave_world(state, conn);
            break;
            // restart
            case 0x46:
            handle_restart(state, conn);
            break;
            // move
            case 0x01:
            handle_movement(state, conn, request);
            break;
            // validate position
            case 0x48:
            handle_validate_position(state, conn, request);
            break;
            // show map
            case 0xcd:
            handle_show_map(state, conn);
            break;
            default:
            break;
        }
    }
    
    memmove(conn->request, conn->request + size, conn->request_count - size);
    conn->request_count -= size;
}

static void handle_send_protocol(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    byte type = 0x00;
    byte protocol[] = {
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
    };
    
    push_response(conn, 0,
                  "%h", 0,
                  "%c", type,
                  "%b", sizeof(protocol), protocol);
}

static void handle_auth(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    assert(req);
    
    pscanf(req,
           "%ls", countof(conn->username), conn->username,
           "%u", &conn->play_ok2,
           "%u", &conn->play_ok1,
           "%u", &conn->login_ok1,
           "%u", &conn->login_ok2);
    
    char access_path[256] = {0};
    snprintf(access_path, 
             sizeof(access_path) - 1, 
             "data/accounts/%ls/access.txt",
             conn->username);
    FILE *access_file = fopen(access_path, "r");
    if (!access_file) {
        error("unable to read %s. can't check if this is a valid auth."
              "the user will be dropped." nl, 
              access_path);
        asocket_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    u32 stored_login_ok1 = 0;
    u32 stored_login_ok2 = 0;
    u32 stored_created_at = 0;
    u32 stored_valid_until = 0;
    fscanf(access_file, 
           "login_ok1=%u" nl
           "login_ok2=%u" nl
           "created_at=%u" nl
           "valid_until=%u" nl,
           &stored_login_ok1,
           &stored_login_ok2,
           &stored_created_at,
           &stored_valid_until);
    
    if (stored_login_ok1 != conn->login_ok1 || stored_login_ok2 != conn->login_ok2) {
        warn("invalid loginok1 or loginok2 from %ls. the connection will be dropped." nl, 
             conn->username);
        asocket_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    if (stored_valid_until < time(0)) {
        trace("keys expired for %ls. the connection will be dropped." nl,
              conn->username);
        asocket_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    handle_send_character_list(state, conn);
}

static void handle_show_create_character_screen(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    byte type = 0x17;
    u32 count = 0;
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%u", count);
}

static void handle_character_creation(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    
    wchar_t name[32] = {0};
    u32 race_id = 0;
    u32 sex = 0;
    u32 class_id = 0;
    u32 ignore = 0;
    u32 hair_style_id = 0;
    u32 hair_color_id = 0;
    u32 face_id = 0;
    
    pscanf(req, 
           "%ls", countof(name), name,
           "%u", &race_id,
           "%u", &sex,
           "%u", &class_id,
           "%u", &ignore,
           "%u", &ignore,
           "%u", &ignore,
           "%u", &ignore,
           "%u", &ignore,
           "%u", &ignore,
           "%u", &hair_style_id,
           "%u", &hair_color_id,
           "%u", &face_id);
    
    // TODO(fmontenegro): check if name exists
    
    char characters_path[256] = {0};
    snprintf(characters_path, 
             sizeof(characters_path) - 1, 
             "data/accounts/%ls/characters",
             conn->username);
    directory_create(characters_path);
    
    char info_path[256] = {0};
    snprintf(info_path, 
             sizeof(info_path) - 1, 
             "data/accounts/%ls/characters/%ls.txt",
             conn->username,
             name);
    
    FILE *info_file = fopen(info_path, "w");
    if (!info_file) {
        error("failed to store data for new characters in %s" nl,
              info_path);
        return;
    }
    
    s32 x = -83968;
    s32 y = 244634;
    s32 z = -3730;
    u32 level = 1;
    u32 xp = 1;
    u32 str = 10;
    u32 dex = 10;
    u32 con = 10;
    u32 _int = 10;
    u32 wit = 10;
    u32 men = 10;
    double max_hp = 500.0;
    double current_hp = 500.0;
    double max_mp = 500.0;
    double current_mp = 500.0;
    double max_cp = 500.0;
    double current_cp = 500.0;
    u32 sp = 1;
    u32 current_load = 0;
    u32 max_load = 100;
    u32 p_atk = 10;
    u32 p_atk_speed = 10;
    u32 p_def = 10;
    u32 evasion_rate = 10;
    u32 accuracy = 10;
    u32 critical_hit = 10;
    u32 m_atk = 10;
    u32 m_atk_speed = 10;
    u32 m_def = 10;
    u32 karma = 10;
    u32 run_speed = 20;
    u32 walk_speed = 10;
    double collision_radius = 20.0;
    double collision_height = 20.0;
    u32 access_level = 1;
    wchar_t title[32] = L"l2auth";
    u32 clan_id = 0;
    u32 crest_id = 0;
    u32 ally_id = 0;
    u32 ally_crest_id = 0;
    u32 pk_kills = 0;
    u32 pvp_kills = 0;
    u16 recommendations_left = 0;
    u16 recommendations_have = 32;
    u32 inventory_limit = 10;
    u32 clan_crest_large_id = 0;
    u8 hero_symbol = 0;
    u8 hero = 0;
    s32 fish_x = 0;
    s32 fish_y = 0;
    s32 fish_z = 0;
    u32 name_color = 0xffffff;
    
    fprintf(info_file,
            "name=%ls" nl
            "race_id=%u" nl
            "sex=%u" nl
            "class_id=%u" nl
            "hair_style_id=%u" nl
            "hair_color_id=%u" nl
            "face_id=%u" nl
            "x=%d" nl
            "y=%d" nl
            "z=%d" nl
            "level=%u" nl
            "xp=%u" nl
            "str=%u" nl
            "dex=%u" nl
            "con=%u" nl
            "int=%u" nl
            "wit=%u" nl
            "men=%u" nl
            "max_hp=%lf" nl
            "current_hp=%lf" nl
            "max_mp=%lf" nl
            "current_mp=%lf" nl
            "max_cp=%lf" nl
            "current_cp=%lf" nl
            "sp=%u" nl
            "current_load=%u" nl
            "max_load=%u" nl
            "p_atk=%u" nl
            "p_atk_speed=%u" nl
            "p_def=%u" nl
            "evasion_rate=%u" nl
            "accuracy=%u" nl
            "critical_hit=%u" nl
            "m_atk=%u" nl
            "m_atk_speed=%u" nl
            "m_def=%u" nl
            "karma=%u" nl
            "run_speed=%u" nl
            "walk_speed=%u" nl
            "collision_radius=%lf" nl
            "collision_height=%lf" nl
            "access_level=%u" nl
            "title=%ls" nl
            "clan_id=%u" nl
            "crest_id=%u" nl
            "ally_id=%u" nl
            "ally_crest_id=%u" nl
            "pk_kills=%u" nl
            "pvp_kills=%u" nl
            "recommendations_left=%u" nl
            "recommendations_have=%u" nl
            "inventory_limit=%u" nl
            "clan_crest_large_id=%u" nl
            "hero_symbol=%u" nl
            "hero=%u" nl
            "fish_x=%d" nl
            "fish_y=%d" nl
            "fish_z=%d" nl
            "name_color=%u" nl,
            name,
            race_id,
            sex,
            class_id,
            hair_style_id,
            hair_color_id,
            face_id,
            x,
            y,
            z,
            level,
            xp,
            str,
            dex,
            con,
            _int,
            wit,
            men,
            max_hp,
            current_hp,
            max_mp,
            current_mp,
            max_cp,
            current_cp,
            sp,
            current_load,
            max_load,
            p_atk,
            p_atk_speed,
            p_def,
            evasion_rate,
            accuracy,
            critical_hit,
            m_atk,
            m_atk_speed,
            m_def,
            karma,
            run_speed,
            walk_speed,
            collision_radius,
            collision_height,
            access_level,
            title,
            clan_id,
            crest_id,
            ally_id,
            ally_crest_id,
            pk_kills,
            pvp_kills,
            recommendations_left,
            recommendations_have,
            inventory_limit,
            clan_crest_large_id,
            hero_symbol,
            hero,
            fish_x,
            fish_y,
            fish_z,
            name_color);
    fclose(info_file);
    
    byte type = 0x19;
    u32 success = 1;
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%u", success);
    
    handle_send_character_list(state, conn);
}

static void handle_send_character_list(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    byte type = 0x13;
    u32 count = 0;
    
    char characters_path[256] = {0};
    snprintf(characters_path, 
             sizeof(characters_path) - 1, 
             "data/accounts/%ls/characters",
             conn->username);
    
    in_directory(characters_path)
        count++;
    
    byte list[1024] = {0};
    byte *end = list;
    
    in_directory(characters_path) {
        FILE *character_file = fopen(directory.full_path, "r");
        if (!character_file)
            continue;
        
        u32 id = 0;
        wchar_t name[32];
        u32 clan_id = 0;
        u32 sex = 0;
        u32 race_id = 0;
        // u32 base_race_id = 0;
        u32 active = 1;
        double current_hp = 500;
        double current_mp = 500;
        u32 sp = 0;
        u32 xp = 0;
        u32 level = 1;
        u32 karma = 0;
        u32 hair_style_id = 0;
        u32 hair_color_id = 0;
        u32 face_id = 0;
        double max_hp = 500;
        double max_mp = 500;
        u32 delete_time = 0;
        u32 class_id = 0;
        u32 auto_select = 0;
        u8 enchant_effect = 0;
        
        fscanf(character_file, 
               "name=%ls" nl
               "race_id=%u" nl
               "sex=%u" nl
               "class_id=%u" nl
               "hair_style_id=%u" nl
               "hair_color_id=%u" nl
               "face_id=%u" nl,
               name,
               &race_id,
               &sex,
               &class_id,
               &hair_style_id,
               &hair_color_id,
               &face_id);
        
        end = bprintf(end, sizeof(list) - (size_t) (end - list),
                      "%ls", countof(name), name,
                      "%u", id,
                      "%ls", countof(conn->username), conn->username,
                      "%u", conn->play_ok1,
                      "%u", clan_id,
                      "%u", 0,
                      "%u", sex,
                      "%u", race_id,
                      "%u", class_id,
                      "%u", active,
                      "%u", 0, // x
                      "%u", 0, // y
                      "%u", 0, // z
                      "%lf", current_hp,
                      "%lf", current_mp,
                      "%u", sp,
                      "%u", xp,
                      "%u", level,
                      "%u", karma,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      "%u", 0,
                      
                      "%u", hair_style_id,
                      "%u", hair_color_id,
                      "%u", face_id,
                      "%lf", max_hp,
                      "%lf", max_mp,
                      "%u", delete_time,
                      "%u", class_id,
                      "%u", auto_select,
                      "%c", enchant_effect);
        
        fclose(character_file);
    }
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%u", count,
                  "%b", (size_t) (end - list), list);
}

static void handle_select_character(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    assert(req);
    
    u32 index = 0;
    pscanf(req, "%u", &index);
    
    char characters_path[256] = {0};
    snprintf(characters_path, 
             sizeof(characters_path) - 1, 
             "data/accounts/%ls/characters",
             conn->username);
    
    conn->character = 0;
    for (int i = 0; i < countof(state->characters); i++) {
        if (state->characters[i].active)
            continue;
        memset(state->characters + i, 0, sizeof(struct character));
        conn->character = state->characters + i;
        conn->character->active = 1;
        break;
    }
    if (!conn->character) {
        error("can't enter more characters in the world. dropping %ls." nl,
              conn->username);
        asocket_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    u32 i = 0;
    int found = 0;
    
    in_directory(characters_path) {
        if (index != i) {
            i++;
            continue;
        }
        
        FILE *character_file = fopen(directory.full_path, "r");
        if (character_file) {
            found = 1;
            fscanf(character_file,
                   "name=%ls" nl
                   "race_id=%u" nl
                   "sex=%u" nl
                   "class_id=%u" nl
                   "hair_style_id=%u" nl
                   "hair_color_id=%u" nl
                   "face_id=%u" nl
                   "x=%d" nl
                   "y=%d" nl
                   "z=%d" nl
                   "level=%u" nl
                   "xp=%u" nl
                   "str=%u" nl
                   "dex=%u" nl
                   "con=%u" nl
                   "int=%u" nl
                   "wit=%u" nl
                   "men=%u" nl
                   "max_hp=%lf" nl
                   "current_hp=%lf" nl
                   "max_mp=%lf" nl
                   "current_mp=%lf" nl
                   "max_cp=%lf" nl
                   "current_cp=%lf" nl
                   "sp=%u" nl
                   "current_load=%u" nl
                   "max_load=%u" nl
                   "p_atk=%u" nl
                   "p_atk_speed=%u" nl
                   "p_def=%u" nl
                   "evasion_rate=%u" nl
                   "accuracy=%u" nl
                   "critical_hit=%u" nl
                   "m_atk=%u" nl
                   "m_atk_speed=%u" nl
                   "m_def=%u" nl
                   "karma=%u" nl
                   "run_speed=%u" nl
                   "walk_speed=%u" nl
                   "collision_radius=%lf" nl
                   "collision_height=%lf" nl
                   "access_level=%u" nl
                   "title=%ls" nl
                   "clan_id=%u" nl
                   "crest_id=%u" nl
                   "ally_id=%u" nl
                   "ally_crest_id=%u" nl
                   "pk_kills=%u" nl
                   "pvp_kills=%u" nl
                   "recommendations_left=%u" nl
                   "recommendations_have=%u" nl
                   "inventory_limit=%u" nl
                   "clan_crest_large_id=%u" nl
                   "hero_symbol=%u" nl
                   "hero=%u" nl
                   "fish_x=%d" nl
                   "fish_y=%d" nl
                   "fish_z=%d" nl
                   "name_color=%u" nl,
                   conn->character->name,
                   &conn->character->race_id,
                   &conn->character->sex,
                   &conn->character->class_id,
                   &conn->character->hair_style_id,
                   &conn->character->hair_color_id,
                   &conn->character->face_id,
                   &conn->character->x,
                   &conn->character->y,
                   &conn->character->z,
                   &conn->character->level,
                   &conn->character->xp,
                   &conn->character->attributes.str,
                   &conn->character->attributes.dex,
                   &conn->character->attributes.con,
                   &conn->character->attributes._int,
                   &conn->character->attributes.wit,
                   &conn->character->attributes.men,
                   &conn->character->max_hp,
                   &conn->character->current_hp,
                   &conn->character->max_mp,
                   &conn->character->current_mp,
                   &conn->character->max_cp,
                   &conn->character->current_cp,
                   &conn->character->sp,
                   &conn->character->current_load,
                   &conn->character->max_load,
                   &conn->character->p_atk,
                   &conn->character->p_atk_speed,
                   &conn->character->p_def,
                   &conn->character->evasion_rate,
                   &conn->character->accuracy,
                   &conn->character->critical_hit,
                   &conn->character->m_atk,
                   &conn->character->m_atk_speed,
                   &conn->character->m_def,
                   &conn->character->karma,
                   &conn->character->run_speed,
                   &conn->character->walk_speed,
                   &conn->character->collision_radius,
                   &conn->character->collision_height,
                   &conn->character->access_level,
                   &conn->character->title,
                   &conn->character->clan_id,
                   &conn->character->crest_id,
                   &conn->character->ally_id,
                   &conn->character->ally_crest_id,
                   &conn->character->pk_kills,
                   &conn->character->pvp_kills,
                   &conn->character->recommendations_left,
                   &conn->character->recommendations_have,
                   &conn->character->inventory_limit,
                   &conn->character->clan_crest_large_id,
                   &conn->character->hero_symbol,
                   &conn->character->hero,
                   &conn->character->fish_x,
                   &conn->character->fish_y,
                   &conn->character->fish_z,
                   &conn->character->name_color);
#if 0
            fscanf(character_file, 
                   "name=%ls" nl
                   "race_id=%u" nl
                   "sex=%u" nl
                   "class_id=%u" nl
                   "hair_style_id=%u" nl
                   "hair_color_id=%u" nl
                   "face_id=%u" nl,
                   conn->character->name,
                   &conn->character->race_id,
                   &conn->character->sex,
                   &conn->character->class_id,
                   &conn->character->hair_style_id,
                   &conn->character->hair_color_id,
                   &conn->character->face_id
                   );
#endif
            fclose(character_file);
        }
        break;
    }
    
    if (!found) {
        error("character with index %u not found for %ls. the connection will be dropped." nl,
              index,
              conn->username);
        conn->character->active = 0;
        asocket_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    u32 in_world_id = 1;
    conn->character->movement_speed_multiplier = 1;
    conn->character->atk_speed_multiplier = 1;
    
    byte type = 0x15;
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%ls", countof(conn->character->name), conn->character->name,
                  "%u", in_world_id,
                  "%ls", countof(conn->character->title), conn->character->title,
                  "%u", conn->play_ok1,
                  "%u", conn->character->clan_id,
                  "%u", 0,
                  "%u", conn->character->sex,
                  "%u", conn->character->race_id,
                  "%u", conn->character->class_id,
                  "%u", conn->character->active,
                  "%u", conn->character->x,
                  "%u", conn->character->y,
                  "%u", conn->character->z,
                  "%lf", conn->character->current_hp,
                  "%lf", conn->character->current_mp,
                  "%u", conn->character->sp,
                  "%u", conn->character->xp,
                  "%u", conn->character->level,
                  "%u", 0,
                  "%u", 0,
                  "%u", conn->character->attributes._int,
                  "%u", conn->character->attributes.str,
                  "%u", conn->character->attributes.con,
                  "%u", conn->character->attributes.men,
                  "%u", conn->character->attributes.dex,
                  "%u", conn->character->attributes.wit,
                  
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0);
}

static void handle_auto_ss_bsps(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    byte type = 0x1b;
    u16 empty = 0;
    u32 manor_size = 0;
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%h", empty,
                  "%u", manor_size);
}

static void handle_send_quest_list(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    byte type = 0x80;
    u8 empty[7] = {0};
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%b", sizeof(empty), empty);
}

// NOTE(fmontenegro): this is userinfo packet.
static void handle_enter_world(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    byte type = 0x04;
    u32 in_world_id = 1;
    u32 clan_leader = 0;
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%u", conn->character->x,
                  "%u", conn->character->y,
                  "%u", conn->character->z,
                  "%u", conn->character->heading,
                  "%u", in_world_id,
                  "%ls", countof(conn->character->name), conn->character->name,
                  "%u", conn->character->race_id,
                  "%u", conn->character->sex,
                  "%u", conn->character->class_id,
                  "%u", conn->character->level,
                  "%u", conn->character->xp,
                  "%u", conn->character->attributes.str,
                  "%u", conn->character->attributes.dex,
                  "%u", conn->character->attributes.con,
                  "%u", conn->character->attributes._int,
                  "%u", conn->character->attributes.wit,
                  "%u", conn->character->attributes.men,
                  "%u", (u32) conn->character->max_hp,
                  "%u", (u32) conn->character->current_hp,
                  "%u", (u32) conn->character->max_mp,
                  "%u", (u32) conn->character->current_mp,
                  "%u", conn->character->sp,
                  "%u", conn->character->current_load,
                  "%u", conn->character->max_load,
                  "%u", 0x28,
                  
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  
                  "%u", conn->character->p_atk,
                  "%u", conn->character->p_atk_speed,
                  "%u", conn->character->p_def,
                  "%u", conn->character->evasion_rate,
                  "%u", conn->character->accuracy,
                  "%u", conn->character->critical_hit,
                  "%u", conn->character->m_atk,
                  "%u", conn->character->m_atk_speed,
                  "%u", conn->character->p_atk_speed,
                  "%u", conn->character->m_def,
                  "%u", conn->character->pvp_flag,
                  "%u", conn->character->karma,
                  "%u", conn->character->run_speed,
                  "%u", conn->character->walk_speed,
                  "%u", conn->character->swim_run_speed,
                  "%u", conn->character->swim_walk_speed,
                  "%u", conn->character->fly_run_speed,
                  "%u", conn->character->fly_walk_speed,
                  "%u", conn->character->fly_run_speed,
                  "%u", conn->character->fly_walk_speed,
                  "%lf", conn->character->movement_speed_multiplier,
                  "%lf", conn->character->atk_speed_multiplier,
                  "%lf", conn->character->collision_radius,
                  "%lf", conn->character->collision_height,
                  "%u", conn->character->hair_style_id,
                  "%u", conn->character->hair_color_id,
                  "%u", conn->character->face_id,
                  "%u", conn->character->access_level,
                  "%ls", countof(conn->character->title), conn->character->title,
                  "%u", conn->character->clan_id,
                  "%u", conn->character->crest_id,
                  "%u", conn->character->ally_id,
                  "%u", conn->character->ally_crest_id,
                  "%u", clan_leader,
                  "%c", conn->character->mount_type,
                  "%c", conn->character->private_store_type,
                  "%c", conn->character->dwarven_craft,
                  "%u", conn->character->pk_kills,
                  "%u", conn->character->pvp_kills,
                  "%h", conn->character->cubics,
                  "%c", conn->character->party_members,
                  "%u", conn->character->abnormal_effect,
                  "%c", 0,
                  "%u", conn->character->clan_privileges,
                  
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  
                  "%h", conn->character->recommendations_left,
                  "%h", conn->character->recommendations_have,
                  "%u", 0,
                  "%h", conn->character->inventory_limit,
                  "%u", conn->character->class_id,
                  "%u", 0,
                  "%u", (u32) conn->character->max_cp,
                  "%u", (u32) conn->character->current_cp,
                  "%c", conn->character->mounted,
                  "%c", 0, // 1 = blue, 2 = red
                  "%u", conn->character->clan_crest_large_id,
                  "%c", conn->character->hero_symbol,
                  "%c", conn->character->hero,
                  "%c", 0,
                  "%u", conn->character->fish_x,
                  "%u", conn->character->fish_y,
                  "%u", conn->character->fish_z,
                  "%u", conn->character->name_color);
}

static void handle_leave_world(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    push_response(conn, 1,
                  "%h", 0,
                  "%c", 0x7e);
}

static void handle_restart(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    push_response(conn, 1,
                  "%h", 0,
                  "%c", 0x5f,
                  "%u", 1);
    handle_send_character_list(state, conn);
}

static void handle_movement(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    assert(req);
    
    s32 target_x = 0;
    s32 target_y = 0;
    s32 target_z = 0;
    s32 origin_x = 0;
    s32 origin_y = 0;
    s32 origin_z = 0;
    s32 movement = 0;
    
    pscanf(req,
           "%u", &target_x,
           "%u", &target_y,
           "%u", &target_z,
           "%u", &origin_x,
           "%u", &origin_y,
           "%u", &origin_z,
           "%u", &movement);
    
    conn->character->x = origin_x;
    conn->character->y = origin_y;
    conn->character->z = origin_z;
    move_to(state, conn, target_x, target_y, target_z);
}

static void handle_validate_position(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    assert(req);
    
    s32 x = 0;
    s32 y = 0;
    s32 z = 0;
    s32 heading = 0;
    
    pscanf(req,
           "%u", &x,
           "%u", &y,
           "%u", &z,
           "%u", &heading);
    
    conn->character->x = x;
    conn->character->y = y;
    conn->character->z = z;
    conn->character->heading = heading;
    
    // TODO(fmontenegro): only if the diff is too large.
#if 0
    u32 in_world_id = 1;
    push_response(conn, 1,
                  "%h", 0,
                  "%c", 0x61,
                  "%u", in_world_id,
                  "%u", x,
                  "%u", y,
                  "%u", z,
                  "%u", heading);
#endif
}

static void handle_show_map(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    trace("spawning orc!" nl);
    
    // TODO(fmontenegro): show map.
    
    struct character orc = {0};
    
    swprintf(orc.name, sizeof(orc.name) - 1, L"%ls", L"Orc");
    swprintf(orc.title, sizeof(orc.title) - 1, L"%ls", L"Orc");
    
    orc.template_id = 7082 + 1000000;
    orc.x = conn->character->x;
    orc.y = conn->character->y;
    orc.z = conn->character->z;
    orc.collision_radius = 8;
    orc.collision_height = 25;
    orc.level = 10;
    orc.sex = 0;
    orc.current_hp = 197;
    orc.max_hp = 197;
    orc.current_mp = 102;
    orc.max_mp = 102;
    orc.attributes.str = 40;
    orc.attributes.con = 43;
    orc.attributes.dex = 30;
    orc.attributes._int = 21;
    orc.attributes.wit = 20;
    orc.attributes.men = 10;
    orc.p_atk = 41;
    orc.p_def = 55;
    orc.m_atk = 6;
    orc.m_def = 45;
    orc.p_atk_speed = 249;
    orc.m_atk_speed = 227;
    orc.walk_speed = 45;
    orc.run_speed = 110;
    
    u32 id = 3;
    u32 attackable = 1;
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", 0x16,
                  "%u", id,
                  "%u", orc.template_id,
                  "%u", attackable,
                  "%u", orc.x,
                  "%u", orc.y,
                  "%u", orc.z,
                  "%u", orc.heading,
                  "%u", 0,
                  "%u", orc.m_atk_speed,
                  "%u", orc.p_atk_speed,
                  "%u", orc.run_speed,
                  "%u", orc.walk_speed,
                  // swim speed
                  "%u", orc.run_speed,
                  "%u", orc.walk_speed,
                  // fly speed
                  "%u", orc.run_speed,
                  "%u", orc.walk_speed,
                  "%u", orc.run_speed,
                  "%u", orc.walk_speed,
                  "%lf", 1.1,
                  "%lf", (double) orc.p_atk_speed / 277.478340719,
                  "%lf", orc.collision_radius,
                  "%lf", orc.collision_height,
                  // right hand weapon
                  "%u", 0,
                  "%u", 0,
                  // left hand weapon
                  "%u", 0,
                  "%c", 1,
                  // running?
                  "%c", 0,
                  // combat?
                  "%c", 0,
                  // alike dead?
                  "%c", 0,
                  // summoned?
                  "%c", 0,
                  "%ls", countof(orc.name), orc.name,
                  "%ls", countof(orc.title), orc.title,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  // abnormal effect
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  "%c", 0,
                  "%c", 0,
                  "%lf", 0.0,
                  "%lf", 0.0,
                  "%u", 0);
}

static void move_to(struct state *state, struct connection *conn, s32 x, s32 y, s32 z)
{
    assert(state);
    assert(conn);
    
    conn->character->action_type = moving;
    conn->character->action_payload.moving.target_x = x;
    conn->character->action_payload.moving.target_y = y;
    conn->character->action_payload.moving.target_z = z;
    
    u32 in_world_id = 1;
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", 0x01,
                  "%u", in_world_id,
                  "%u", x,
                  "%u", y,
                  "%u", z,
                  "%u", conn->character->x,
                  "%u", conn->character->y,
                  "%u", conn->character->z);
}

static void moving_update(struct state *state, struct character *character)
{
    assert(state);
    assert(character);
    
    // s32 dx = character->action_payload.moving.target_x - character->x;
    // s32 dy = character->action_payload.moving.target_y - character->y;
    // s32 dz = character->action_payload.moving.target_x - character->z;
    
    
}

