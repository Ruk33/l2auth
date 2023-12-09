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

struct attributes {
    u32 str;
    u32 dex;
    u32 con;
    u32 _int;
    u32 wit;
    u32 men;
};

struct character {
    int active;
    wchar_t name[32];
    s32 x;
    s32 y;
    s32 z;
    s32 heading;
    u32 race_id;
    u32 sex;
    u32 class_id;
    u32 level;
    u32 xp;
    struct attributes attributes;
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
};

struct state {
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

int on_tick(void **buf)
{
    assert(buf);
    buf = buf;
    
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
    
    fprintf(info_file,
            "name=%ls" nl
            "race_id=%u" nl
            "sex=%u" nl
            "class_id=%u" nl
            "hair_style_id=%u" nl
            "hair_color_id=%u" nl
            "face_id=%u" nl,
            name,
            race_id,
            sex,
            class_id,
            hair_style_id,
            hair_color_id,
            face_id);
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
                   "face_id=%u" nl,
                   conn->character->name,
                   &conn->character->race_id,
                   &conn->character->sex,
                   &conn->character->class_id,
                   &conn->character->hair_style_id,
                   &conn->character->hair_color_id,
                   &conn->character->face_id);
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
    conn->character->access_level = 1;
    conn->character->xp = 10;
    conn->character->sp = 10;
    conn->character->level = 1;
    conn->character->max_hp = 500;
    conn->character->current_hp = 500;
    conn->character->max_mp = 500;
    conn->character->current_mp = 500;
    conn->character->max_cp = 500;
    conn->character->current_cp = 500;
    conn->character->attributes.str = 10;
    conn->character->attributes.dex = 10;
    conn->character->attributes.con = 10;
    conn->character->attributes._int = 10;
    conn->character->attributes.wit = 10;
    conn->character->attributes.men = 10;
    conn->character->collision_radius = 20;
    conn->character->collision_height = 20;
    conn->character->movement_speed_multiplier = 1;
    conn->character->atk_speed_multiplier = 1;
    conn->character->name_color = 0xFFFFFF;
    conn->character->x = -83968;
    conn->character->y = 244634;
    conn->character->z = -3730;
    
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
                  "%u", (u32)conn->character->current_hp,
                  "%u", (u32)conn->character->max_mp,
                  "%u", (u32)conn->character->current_mp,
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
