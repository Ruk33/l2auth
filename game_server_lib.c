#include <assert.h>
#include <stdlib.h> // calloc, size_t
#include <stdio.h>  // fprintf
#include <string.h> // memset, memcpy, strncpy, strnlen
#include <stdint.h> // fixed int types
#include <locale.h> // setlocale
#include <wchar.h>  // wchar_t
#include <time.h>   // time_t, now
#include <stdarg.h> // va_arg, va_list
#include <math.h>   // sqrtf

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __linux__
#include <pthread.h> // pthread_create
#include <unistd.h>
#endif

#include "directory.h"
#include "net.h"
#include "wqueue.h"
#include "pevent.h"

typedef uint8_t byte;

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef  int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float seconds;

#define nl "\n"
#define kb *1024
#define mb *1024*1024
#define sqr(x) ((x)*(x))
#define trace(...) fprintf(stderr, __VA_ARGS__)
#define warn(...)  trace("warning / " __VA_ARGS__)
#define error(...) trace("  error / " __VA_ARGS__)
#define countof(x) (sizeof(x) / sizeof(*(x)))

#define bprintf(dest, n, ...) \
bprintf_((dest), (n), __VA_ARGS__, 0)

#define bscanf(src, n, ...) \
bscanf_((src), (n), __VA_ARGS__, 0)

#define pscanf(src, ...) \
bscanf_((src) + 3, packet_size(src) - 3, __VA_ARGS__, 0)

#define push_response(conn, _encrypt, ...) \
push_response_((conn), (_encrypt), __VA_ARGS__, 0)

#define broadcast(state, character, _encrypt, ...) \
broadcast_((state), (character), (_encrypt), __VA_ARGS__, 0)

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
if (__coro->sleep_for > 0) \
break;

#define reset *__coro = (struct coroutine) {0}

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
    
    byte to_send[8 kb];
    size_t to_send_count;
    size_t sent;
    
    byte request[8 kb];
    size_t request_count;
    
    struct character *character;
};

struct character {
    int active;
    struct connection *conn;
    wchar_t name[32];
    u32 template_id;
    s32 x;
    s32 y;
    s32 z;
    s32 heading;
    s32 client_x;
    s32 client_y;
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
    
    u32 target_id;
    
    enum action_type {
        idle,
        moving,
        attacking,
    } action_type, prev_action_type;
    
    union action_payload {
        struct action_moving {
            int queue_action;
            u32 offset;
            
            u64 ticks_to_move;
            u64 move_start_time;
            u64 move_timestamp;
            
            float x_speed_ticks;
            float y_speed_ticks;
            
            s32 src_x;
            s32 src_y;
            s32 src_z;
            
            s32 target_x;
            s32 target_y;
            s32 target_z;
            
            u32 target_id;
        } moving;
        
        struct action_attacking {
            u32 obj_id;
            struct coroutine state;
        } attacking;
    } action_payload, prev_action_payload;
};

struct state {
    float d;
    double run_time;
    u64 ticks;
    struct wqueue send_responses_worker;
    struct connection connections[1024];
    struct character characters[1024];

#ifdef _WIN32
    HANDLE timer;
    HANDLE lock;
#endif

#ifdef __linux__
    pthread_t timer;
    pthread_mutex_t lock;
#endif
};

static void lock(struct state *state)
{
#ifdef _WIN32
    WaitForSingleObject(state->lock, INFINITE);
#endif

#ifdef __linux__
    pthread_mutex_lock(&state->lock);
#endif
}

static void unlock(struct state *state)
{
#ifdef _WIN32
    ReleaseMutex(state->lock);
#endif

#ifdef __linux__
    pthread_mutex_unlock(&state->lock);
#endif
}

static struct connection *get_connection_from_socket(struct state *state, int socket);

static void encrypt(struct connection *conn, byte *packet);
static void decrypt(struct connection *conn, byte *request);

static int handle_request(struct state *state, struct connection *conn);
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
static void handle_action(struct state *state, struct connection *conn, byte *req);
static void handle_deselect_target(struct state *state, struct connection *conn);

// order character to walk to a point.
static void move_to(struct state *state, struct character *src, s32 x, s32 y, s32 z, u32 offset, u32 target_id, int queue_action);
// make character select a target. this is the first step
// in order to interact with the target character.
static void select_target(struct state *state, struct character *src, u32 target_id);
// make character interact with a target.
// - if target is attackable, then an attack order will take place.
// - if target isn't attackable, then a dialog will show up.
static void start_interaction(struct state *state, struct character *src);
// NOTE(fmontenegro): not sure about this one...
static void broadcast_char_info(struct state *state, struct character *src);
//

enum attr_status {
    attr_status_level = 0x01,
    attr_status_xp = 0x02,
    attr_status_str = 0x03,
    attr_status_dex = 0x04,
    attr_status_con = 0x05,
    attr_status_int = 0x06,
    attr_status_wit = 0x07,
    attr_status_men = 0x08,
    attr_status_current_hp = 0x09,
    attr_status_max_hp = 0x0a,
    attr_status_current_mp = 0x0b,
    attr_status_max_mp = 0x0c,
    attr_status_sp = 0x0d,
    attr_status_current_load = 0x0e,
    attr_status_max_load = 0x0f,
    attr_status_p_atk = 0x11,
    attr_status_atk_speed = 0x12,
    attr_status_p_def = 0x13,
    attr_status_evasion = 0x14,
    attr_status_accuracy = 0x15,
    attr_status_critical = 0x16,
    attr_status_m_atk = 0x17,
    attr_status_cast_speed = 0x18,
    attr_status_m_def = 0x19,
    attr_status_pvp_flag = 0x1a,
    attr_status_karma = 0x1b,
    attr_status_current_cp = 0x21,
    attr_status_max_cp = 0x22,
};
static void send_attr_status(struct state *state, struct character *from, struct character *to, enum attr_status *status, size_t n);

// get character by id. if none is found, null will be returned.
static struct character *get_character_by_id(struct state *state, u32 id);
static u32 get_character_id(struct state *state, struct character *src);

// static void idle_update(struct state *state, struct character *character);
static void moving_update(struct state *state, struct character *character);
static void attacking_update(struct state *state, struct character *character);

static u32 distance_between(s32 x, s32 y, s32 z, s32 x2, s32 y2, s32 z2);
static u32 distance_between_characters(struct character *a, struct character *b);

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

            /*
             * wchar_t is 2 bytes in windows and 4
             * in linux, which means, we can't use
             * predefined functions such as wcsncpy,
             * we have to do it manually. lineage 
             * sends us two bytes per character.
             */
            for (size_t i = 0; i < max_letters && *(u16 *) tail; i++) {
                *dest++ = (wchar_t) ((tail[1] << 8) | tail[0]);
                tail += 2;
            }
            /*
             * null terminator.
             */
            *dest = 0;
            tail += 2;
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
            u16 src = (u16) va_arg(va, u32);
            *(u16 *) tail = src;
            tail += sizeof(src);
            continue;
        }
        if (fmt[0] == '%' && fmt[1] == 'c') {
            u8 src = (u8) va_arg(va, u32);
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

static void push_response_va(struct connection *conn, int _encrypt, va_list va)
{
    assert(conn);
    
    byte *start = conn->to_send + conn->to_send_count;
    size_t max = sizeof(conn->to_send) - (size_t) (start - conn->to_send);
    byte *end = bprintf_va(start, max, va);

    u16 size = (u16) (end - start);
    *(u16 *) start = size;
    
    if (_encrypt)
        encrypt(conn, start);
    
    conn->to_send_count += size;
}

static void push_response_(struct connection *conn, int _encrypt, ...)
{
    assert(conn);
    
    va_list va;
    va_start(va, _encrypt);
    push_response_va(conn, _encrypt, va);
    va_end(va);
}

static void broadcast_(struct state *state, struct character *character, int _encrypt, ...)
{
    assert(state);
    assert(character);
    for (size_t i = 0; i < countof(state->characters); i++) {
        if (!state->characters[i].active)
            continue;
        struct connection *conn = state->characters[i].conn;
        if (!conn)
            continue;
        va_list va;
        va_start(va, _encrypt);
        push_response_va(conn, _encrypt, va);
        va_end(va);
    }
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

static void send_responses(struct wqueue *q, void *w)
{
    struct state *state = q->p;
    lock(state);

    struct connection *conn = (struct connection *) w;
    void *head = conn->to_send + conn->sent;
    unsigned long long to_send = conn->to_send_count - conn->sent;
    trace("sending %d bytes of data to %d" nl, (s32) to_send, conn->socket);
    conn->sent += net_send(conn->socket, head, to_send);

    // all data has been sent, reset the counters.
    if (conn->sent >= conn->to_send_count) {
        conn->sent = 0;
        conn->to_send_count = 0;
    }

    unlock(state);
}

static void character_update(struct state *state, struct character *character)
{
    switch (character->action_type) {
        case idle:
        // idle_update(state, character);
        break;
        case moving:
        moving_update(state, character);
        break;
        case attacking:
        attacking_update(state, character);
        break;
        default:
        break;
    }
}

static void on_tick(struct state *state)
{
    u64 old_ticks = state->ticks;
    
    state->d = 1000.0f;
    state->run_time += (double) state->d;
    state->ticks = (u64) state->run_time / 100;

    if (state->ticks == old_ticks)
        return;

    for (size_t i = 0; i < countof(state->characters); i++) {
        struct character *character = state->characters + i;
        if (!character->active)
            continue;
        character_update(state, character);
    }

    for (size_t i = 0; i < countof(state->connections); i++) {
        struct connection *conn = state->connections + i;
        if (!conn->socket)
            continue;
        if (!conn->character || !conn->character->active)
            continue;
        if (!conn->to_send_count)
            continue;
        wpush(&state->send_responses_worker, conn);
    }
}

#ifdef _WIN32
static DWORD timer_thread(LPVOID param)
#endif
#ifdef __linux__
static void *timer_thread(void *param)
#endif
{
    struct state *state = (struct state *) param;

    while (1) {
#ifdef _WIN32
        Sleep(1000);
#endif
#ifdef __linux__
        sleep(1);
#endif

        lock(state);
        on_tick(state);
        unlock(state);
    }
    return 0;
}

static void init_threads(struct state *state)
{
    assert(state);

#ifdef _WIN32
    state->lock = CreateMutex(0, FALSE, 0);
    state->timer = CreateThread(0, 0, timer_thread, state, 0, 0);
#endif

#ifdef __linux__
    pthread_mutex_init(&state->lock, 0);
    pthread_create(&state->timer, 0, timer_thread, state);
#endif

    state->send_responses_worker.p = state;
    wstart(&state->send_responses_worker, send_responses);
}

static int on_init(void **buf)
{
    assert(buf);
    size_t to_alloc = 256 mb;
    assert(to_alloc > sizeof(struct state));
    *buf = calloc(1, to_alloc);
    if (!*buf) {
        trace("unable to allocate memory for game server" nl);
        return 0;
    }
    init_threads((struct state *) *buf);
    setlocale(LC_ALL, "");
    return 1;
}

static void on_connection(void **buf, int socket)
{
    assert(buf);
    
    struct state *state = *(struct state **) buf;
    assert(state);
    trace("new connection to game server!" nl);
    struct connection *connection = get_connection_from_socket(state, socket);
    if (!connection) {
        trace("there is no more space to accept new players. dropping new connection" nl);
        net_close(socket);
        return;
    }
    memset(connection, 0, sizeof(*connection));
    connection->socket = socket;
    
    byte key[] = {0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87};
    memcpy(connection->encrypt_key, key, sizeof(key));
    memcpy(connection->decrypt_key, key, sizeof(key));
}

static void on_request(void **buf, int socket, void *request, size_t len)
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
    while (handle_request(state, conn));
    wpush(&state->send_responses_worker, conn);
}

static void on_disconnect(void **buf, int socket)
{
    assert(buf);
    struct state *state = *(struct state **) buf;
    assert(state);
    
    struct connection *conn = get_connection_from_socket(state, socket);
    if (!conn)
        return;
    trace("client disconnected" nl);
    conn->socket = 0;
    if (conn->character) {
        conn->character->active = 0;
        conn->character = 0;
    }
}

int on_pevent(void **buf, enum pevent event, union ppayload *payload)
{
    struct state *state = *(struct state **) buf;
    switch (event) {
    case pevent_init: {
        return on_init(buf);
    } break;
    case pevent_before_reload: {
        wclose(&state->send_responses_worker);

#ifdef _WIN32
        TerminateThread(state->timer, 0);
        CloseHandle(state->timer);
        CloseHandle(state->lock);
#endif

#ifdef __linux__
        pthread_cancel(state->timer);
        pthread_mutex_destroy(&state->lock);
#endif
    } break;
    case pevent_after_reload: {
        init_threads(state);
    } break;
    case pevent_socket_connection: {
        lock(state);
        on_connection(buf, payload->pevent_socket.socket);
        unlock(state);
    } break;
    case pevent_socket_request: {
        lock(state);
        on_request(buf, 
                   payload->pevent_socket.socket, 
                   payload->pevent_socket.read, 
                   payload->pevent_socket.len);
        unlock(state);
    } break;
    case pevent_socket_disconnected: {
        lock(state);
        on_disconnect(buf, payload->pevent_socket.socket);
        unlock(state);
    } break;
    default:
    break;
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

static void handle_request_by_type(struct state *state, struct connection *conn)
{
    byte *request = conn->request;
    
    byte type = *(request + 2);
    trace("received packet is of type %#04x" nl, (int) type);
    
    // check if the received packet matches the expected packet.
    // if it doesn't, drop/close the connection.
#define expected_or_close(expected) \
if (type != (expected)) { \
error("expecting packet %d but got %d. the connection will be dropped." nl, \
(expected), \
type); \
net_close(conn->socket); \
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
            // action
            case 0x04:
            handle_action(state, conn, request);
            break;
            // deselect target
            case 0x37:
            handle_deselect_target(state, conn);
            break;
            default:
            break;
        }
    }
}

static int handle_request(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    byte *request = conn->request;
    
    u16 size = *(u16 *) request;
    if (size > conn->request_count)
        return 0;
    
    if (conn->encrypted)
        decrypt(conn, request);
    
    handle_request_by_type(state, conn);
    
    memmove(conn->request, conn->request + size, conn->request_count - size);
    conn->request_count -= size;

    return 1;
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

    trace("username is %S" nl, conn->username);

    char access_path[512] = {0};
    snprintf(access_path, 
             sizeof(access_path) - 1, 
             "data/accounts/%ls/access.txt",
             conn->username);
    FILE *access_file = fopen(access_path, "r");
    if (!access_file) {
        error("unable to read %s. can't check if this is a valid auth."
              "the user will be dropped." nl, 
              access_path);
        net_close(conn->socket);
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
        net_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    if (stored_valid_until < time(0)) {
        trace("keys expired for %ls. the connection will be dropped." nl,
              conn->username);
        net_close(conn->socket);
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
    
    char characters_path[512] = {0};
    snprintf(characters_path, 
             sizeof(characters_path) - 1, 
             "data/accounts/%ls/characters",
             conn->username);
    directory_create(characters_path);
    
    char info_path[1024] = {0};
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
    
    char characters_path[512] = {0};
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
    
    char characters_path[512] = {0};
    snprintf(characters_path, 
             sizeof(characters_path) - 1, 
             "data/accounts/%ls/characters",
             conn->username);
    
    conn->character = 0;
    for (size_t i = 1; i < countof(state->characters); i++) {
        if (state->characters[i].active)
            continue;
        memset(state->characters + i, 0, sizeof(struct character));
        conn->character = state->characters + i;
        conn->character->active = 1;
        conn->character->conn = conn;
        break;
    }
    if (!conn->character) {
        error("can't enter more characters in the world. dropping %ls." nl,
              conn->username);
        net_close(conn->socket);
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
            u32 recommendations_left = 0;
            u32 recommendations_have = 0;
            u32 inventory_limit = 0;
            u32 hero_symbol = 0;
            u32 hero = 0;

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
                   conn->character->title,
                   &conn->character->clan_id,
                   &conn->character->crest_id,
                   &conn->character->ally_id,
                   &conn->character->ally_crest_id,
                   &conn->character->pk_kills,
                   &conn->character->pvp_kills,
                   &recommendations_left,
                   &recommendations_have,
                   &inventory_limit,
                   &conn->character->clan_crest_large_id,
                   &hero_symbol,
                   &hero,
                   &conn->character->fish_x,
                   &conn->character->fish_y,
                   &conn->character->fish_z,
                   &conn->character->name_color);
            fclose(character_file);

            conn->character->recommendations_left = (u16) recommendations_left;
            conn->character->recommendations_have = (u16) recommendations_have;
            conn->character->inventory_limit = (u16) inventory_limit;
            conn->character->hero_symbol = (u8) hero_symbol;
            conn->character->hero = (u8) hero;
        }
        break;
    }
    
    if (!found) {
        error("character with index %u not found for %ls. the connection will be dropped." nl,
              index,
              conn->username);
        conn->character->active = 0;
        net_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    u32 in_world_id = (u32) (size_t) (conn->character - state->characters);
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
    u32 clan_leader = 0;
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", type,
                  "%u", conn->character->x,
                  "%u", conn->character->y,
                  "%u", conn->character->z,
                  "%u", conn->character->heading,
                  "%u", get_character_id(state, conn->character),
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
    
    // go through all npcs
    for (size_t i = 0; i < countof(state->characters); i++) {
        struct character *npc = state->characters + i;
        if (!npc->active)
            continue;
        if (npc->conn)
            continue;
        u32 attackable = 1;
        push_response(conn, 1,
                      "%h", 0,
                      "%c", 0x16,
                      "%u", get_character_id(state, npc),
                      "%u", npc->template_id,
                      "%u", attackable,
                      "%u", npc->x,
                      "%u", npc->y,
                      "%u", npc->z,
                      "%u", npc->heading,
                      "%u", 0,
                      "%u", npc->m_atk_speed,
                      "%u", npc->p_atk_speed,
                      "%u", npc->run_speed,
                      "%u", npc->walk_speed,
                      // swim speed
                      "%u", npc->run_speed,
                      "%u", npc->walk_speed,
                      // fly speed
                      "%u", npc->run_speed,
                      "%u", npc->walk_speed,
                      "%u", npc->run_speed,
                      "%u", npc->walk_speed,
                      "%lf", 1.0,
                      "%lf", 1.0,
                      "%lf", npc->collision_radius,
                      "%lf", npc->collision_height,
                      // right hand weapon
                      "%u", 0,
                      "%u", 0,
                      // left hand weapon
                      "%u", 0,
                      "%c", 1,
                      // running?
                      "%c", 1,
                      // combat?
                      "%c", 0,
                      // alike dead?
                      "%c", 0,
                      // summoned?
                      "%c", 0,
                      "%ls", countof(npc->name), npc->name,
                      "%ls", countof(npc->title), npc->title,
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
                      "%lf", npc->collision_radius,
                      "%lf", npc->collision_height,
                      "%u", 0);
    }
}

static void handle_leave_world(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    if (conn->character)
        conn->character->active = 0;
    push_response(conn, 1,
                  "%h", 0,
                  "%c", 0x7e);
}

static void handle_restart(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    conn->character->active = 0;
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
    move_to(state, conn->character, target_x, target_y, target_z, 0, 0, 0);
}

static void handle_validate_position(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    assert(req);
    
    pscanf(req,
           "%u", &conn->character->client_x,
           "%u", &conn->character->client_y,
           "%u", &conn->character->z,
           "%u", &conn->character->heading);
}

static void handle_show_map(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    trace("spawning orc!" nl);
    
    // TODO(fmontenegro): show map.
    
    struct character *orc = 0;
    u32 id = 1;
    for (; id < countof(state->characters); id++) {
        if (state->characters[id].active)
            continue;
        orc = state->characters + id;
        break;
    }
    orc->active = 1;
    
    swprintf(orc->name, sizeof(orc->name) - 1, L"%ls", L"Orc");
    swprintf(orc->title, sizeof(orc->title) - 1, L"%ls", L"Orc");
    
    orc->template_id = 500 + 1000000;
    orc->x = conn->character->x;
    orc->y = conn->character->y;
    orc->z = conn->character->z;
    orc->collision_radius = 8;
    orc->collision_height = 25;
    orc->level = 10;
    orc->sex = 0;
    orc->current_hp = 197;
    orc->max_hp = 197;
    orc->current_mp = 102;
    orc->max_mp = 102;
    orc->attributes.str = 40;
    orc->attributes.con = 43;
    orc->attributes.dex = 30;
    orc->attributes._int = 21;
    orc->attributes.wit = 20;
    orc->attributes.men = 10;
    orc->p_atk = 41;
    orc->p_def = 55;
    orc->m_atk = 6;
    orc->m_def = 45;
    orc->p_atk_speed = 249;
    orc->m_atk_speed = 227;
    orc->walk_speed = 45;
    orc->run_speed = 110;
    
    broadcast_char_info(state, orc);
}

static void handle_action(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    assert(req);
    
    u32 target_id = 0;
    s32 origin_x = 0;
    s32 origin_y = 0;
    s32 origin_z = 0;
    u8 action_id = 0;
    
    pscanf(req,
           "%u", &target_id,
           "%u", &origin_x,
           "%u", &origin_y,
           "%u", &origin_z,
           // 0 -> simple click, 1 -> shift
           "%c", &action_id);
    
    switch (conn->character->action_type) {
        case idle:
        case moving:
        if (conn->character->target_id == target_id) {
            start_interaction(state, conn->character);
            return;
        }
        break;
        case attacking:
        if (conn->character->target_id == target_id)
            return;
        break;
        default:
        break;
    }
    
    select_target(state, conn->character, target_id);
}

static void handle_deselect_target(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    u32 target_id = conn->character->target_id;
    conn->character->target_id = 0;
    
    struct character *target = get_character_by_id(state, target_id);
    if (!target)
        return;
    
    conn->character->action_type = idle;
    
    push_response(conn, 1,
                  "%h", 0,
                  "%c", 0x2a,
                  "%u", get_character_id(state, conn->character),
                  "%u", conn->character->x,
                  "%u", conn->character->y,
                  "%u", conn->character->z);
}

static void move_to(struct state *state, struct character *character, s32 x, s32 y, s32 z, u32 offset, u32 target_id, int queue_action)
{
    assert(state);
    assert(character);
    
    if (queue_action) {
        character->prev_action_type = character->action_type;
        character->prev_action_payload = character->action_payload;
    }
    
    character->action_type = moving;
    character->action_payload = (union action_payload) {0};
    
    character->action_payload.moving.queue_action = queue_action;
    character->action_payload.moving.offset = offset;
    character->action_payload.moving.target_id = target_id;
    
    character->action_payload.moving.src_x = character->x;
    character->action_payload.moving.src_y = character->y;
    character->action_payload.moving.src_z = character->z;
    
    s32 dx = x - character->x;
    s32 dy = y - character->y;
    s32 dz = z - character->z;
    float d = sqrtf(sqr((float) dx) + sqr((float) dy) + sqr((float) dz));
    d -= offset;
    float speed = (float) character->run_speed;
    
    float ticks_per_second = 10.0f;
    
    character->action_payload.moving.move_start_time = state->ticks;
    character->action_payload.moving.ticks_to_move = (u64) (ticks_per_second * d / speed);
    if (character->action_payload.moving.ticks_to_move <= 0)
        character->action_payload.moving.ticks_to_move = 1;
    
    float sin = (float) dy / d;
    float cos = (float) dx / d;
    character->action_payload.moving.x_speed_ticks = cos * speed / ticks_per_second;
    character->action_payload.moving.y_speed_ticks = sin * speed / ticks_per_second;
    
    x = character->x + (s32) (d * cos);
    y = character->y + (s32) (d * sin);
    
    character->action_payload.moving.target_x = x;
    character->action_payload.moving.target_y = y;
    character->action_payload.moving.target_z = z;
    
    if (!character->conn) {
        character->z = z;
        broadcast_char_info(state, character);
    }
    
    if (!target_id) {
        broadcast(state,
                  character, 1,
                  "%h", 0,
                  "%c", 0x01,
                  "%u", get_character_id(state, character),
                  "%u", x,
                  "%u", y,
                  "%u", z,
                  "%u", character->x,
                  "%u", character->y,
                  "%u", character->z);
        return;
    }
    
    broadcast(state,
              character, 1,
              "%h", 0,
              "%c", 0x60,
              "%u", get_character_id(state, character),
              "%u", target_id,
              "%u", offset,
              "%u", character->x,
              "%u", character->y,
              "%u", character->z);
}

static void select_target(struct state *state, struct character *src, u32 target_id)
{
    assert(state);
    assert(src);
    
    struct character *target = get_character_by_id(state, target_id);
    if (!target || !target->active)
        return;
    
    src->target_id = target_id;
    
    // select target.
    push_response(src->conn, 1,
                  "%h", 0,
                  "%c", 0xa6,
                  "%u", target_id,
                  "%h", 0);
    
    // display correct life and max life.
    enum attr_status status[] = {attr_status_current_hp, attr_status_max_hp};
    send_attr_status(state, target, src, status, 2);
    
    // validate position.
    push_response(src->conn, 1,
                  "%h", 0,
                  "%c", 0x61,
                  "%u", target_id,
                  "%u", target->x,
                  "%u", target->y,
                  "%u", target->z,
                  "%u", target->heading);
}

static void start_interaction(struct state *state, struct character *src)
{
    assert(state);
    assert(src);
    
    // an interaction can begin only when the player has selected a target.
    if (!src->target_id)
        return;
    
    u32 attacker_id = get_character_id(state, src);
    u32 target_id = src->target_id;
    if (attacker_id == target_id)
        return;
    
    broadcast_char_info(state, get_character_by_id(state, target_id));
    
    src->action_type = attacking;
    src->action_payload = (union action_payload) {0};
    src->action_payload.attacking.obj_id = target_id;
    
    attacking_update(state, src);
}

static void broadcast_char_info(struct state *state, struct character *src)
{
    assert(state);
    if (!src || !src->active)
        return;
    
    int is_npc = !src->conn;
    if (is_npc) {
        u32 attackable = 1;
        broadcast(state,
                  src, 1,
                  "%h", 0,
                  "%c", 0x16,
                  "%u", get_character_id(state, src),
                  "%u", src->template_id,
                  "%u", attackable,
                  "%u", src->x,
                  "%u", src->y,
                  "%u", src->z,
                  "%u", src->heading,
                  "%u", 0,
                  "%u", src->m_atk_speed,
                  "%u", src->p_atk_speed,
                  "%u", src->run_speed,
                  "%u", src->walk_speed,
                  // swim speed
                  "%u", src->run_speed,
                  "%u", src->walk_speed,
                  // fly speed
                  "%u", src->run_speed,
                  "%u", src->walk_speed,
                  "%u", src->run_speed,
                  "%u", src->walk_speed,
                  // "%lf", 1.1,
                  "%lf", 1.0,
                  // "%lf", (double) src->p_atk_speed / 277.478340719,
                  "%lf", 1.0,
                  "%lf", src->collision_radius,
                  "%lf", src->collision_height,
                  // right hand weapon
                  "%u", 0,
                  // chest
                  "%u", 0,
                  // left hand weapon
                  "%u", 0,
                  "%c", 1,
                  // running?
                  "%c", 1,
                  // combat?
                  "%c", src->action_type == attacking,
                  // alike dead?
                  "%c", 0,
                  // summoned?
                  "%c", 0,
                  "%ls", countof(src->name), src->name,
                  "%ls", countof(src->title), src->title,
                  // playable flag?
                  "%u", 0,
                  "%u", 0,
                  "%u", 0,
                  // abnormal effect
                  "%u", 0,
                  // clan id
                  "%u", 0,
                  // clan crest
                  "%u", 0,
                  // ally id
                  "%u", 0,
                  // ally crest
                  "%u", 0,
                  "%c", 0,
                  // aura color?
                  "%c", 0,
                  // collision radius
                  "%lf", src->collision_radius,
                  "%lf", src->collision_height,
                  "%u", 0);
        return;
    }
    
    // send char info...
}

static void send_attr_status(struct state *state, struct character *from, struct character *to,
                             enum attr_status *status, size_t n)
{
    assert(state);
    assert(status);
    if (!from || !to || !from->active || !to->active || !to->conn)
        return;
    
    byte attrs[256] = {0};
    byte *end = attrs;
    
    for (size_t i = 0; i < n; i++) {
        u32 value = 0;
        
        switch (status[i]) {
            case attr_status_level:
            value = from->level;
            break;
            case attr_status_xp:
            // TODO(fmontenegro): ?
            value = 1;
            break;
            case attr_status_str:
            value = from->attributes.str;
            break;
            case attr_status_dex:
            value = from->attributes.dex;
            break;
            case attr_status_con:
            value = from->attributes.con;
            break;
            case attr_status_int:
            value = from->attributes._int;
            break;
            case attr_status_wit:
            value = from->attributes.wit;
            break;
            case attr_status_men:
            value = from->attributes.men;
            break;
            case attr_status_current_hp:
            value = (u32) from->current_hp;
            break;
            case attr_status_max_hp:
            value = (u32) from->max_hp;
            break;
            case attr_status_current_mp:
            value = (u32) from->current_mp;
            break;
            case attr_status_max_mp:
            value = (u32) from->max_mp;
            break;
            // TODO(fmontenegro): implement.
            case attr_status_sp:
            value = 1;
            break;
            case attr_status_current_load:
            value = from->current_load;
            break;
            case attr_status_max_load:
            value = from->max_load;
            break;
            case attr_status_p_atk:
            value = from->p_atk;
            break;
            case attr_status_atk_speed:
            value = from->p_atk_speed;
            break;
            case attr_status_p_def:
            value = from->p_def;
            break;
            case attr_status_evasion:
            value = from->evasion_rate;
            break;
            case attr_status_accuracy:
            value = from->accuracy;
            break;
            // TODO(fmontenegro): ?
            case attr_status_critical:
            value = from->critical_hit;
            break;
            case attr_status_m_atk:
            value = from->m_atk;
            break;
            // NOTE(fmontenegro): is this ok?
            case attr_status_cast_speed:
            value = from->m_atk_speed;
            break;
            case attr_status_m_def:
            value = from->m_def;
            break;
            // TODO(fmontenegro): check if pvp flag is sent as 0 and 1.
            case attr_status_pvp_flag: {
                value = 0;
                if (from->action_type == attacking) {
                    u32 target_id = from->action_payload.attacking.obj_id;
                    struct character *target = get_character_by_id(state, target_id);
                    if (target && target->active && target->conn)
                        value = 1;
                }
            } break;
            case attr_status_karma:
            value = from->karma;
            break;
            case attr_status_current_cp:
            value = (u32) from->current_cp;
            break;
            case attr_status_max_cp:
            value = (u32) from->max_cp;
            break;
            default:
            warn("invalid status %d" nl, status[i]);
            break;
        }
        
        end = bprintf(end, 
                      sizeof(attrs) - (size_t) (end - attrs),
                      "%u", status[i],
                      "%u", value);
    }
    
    u32 from_id = get_character_id(state, from);
    push_response(to->conn, 1,
                  "%h", 0,
                  "%c", 0x0e,
                  "%u", from_id,
                  "%u", (u32) n,
                  "%b", (size_t) (end - attrs), attrs);
}

static struct character *get_character_by_id(struct state *state, u32 id)
{
    assert(state);
    if (id >= countof(state->characters))
        return 0;
    return state->characters + id;
}

static u32 get_character_id(struct state *state, struct character *src)
{
    assert(state);
    assert(src);
    return (u32) ((size_t) (src - state->characters));
}

static void moving_update(struct state *state, struct character *character)
{
    assert(state);
    assert(character);
    
    struct action_moving payload = character->action_payload.moving;
    
    if (payload.move_timestamp == state->ticks)
        return;
    
    u64 elapsed = state->ticks - payload.move_start_time;
    
    if (elapsed >= payload.ticks_to_move)
        goto arrived;
    
    s32 src_x = payload.src_x;
    float x_speed_ticks = payload.x_speed_ticks;
    character->x = src_x + (s32) (elapsed * x_speed_ticks);
    
    s32 src_y = payload.src_y;
    float y_speed_ticks = payload.y_speed_ticks;
    character->y = src_y + (s32) (elapsed * y_speed_ticks);
    
    character->action_payload.moving.move_timestamp = state->ticks;
    
    if (payload.target_id) {
        struct character *target = get_character_by_id(state, payload.target_id);
        
        // stop movement if the target is no longer valid.
        if (!target || !target->active) {
            payload.target_x = character->x;
            payload.target_y = character->y;
            payload.target_z = character->z;
            
            // stop movement.
            broadcast(state,
                      character, 1,
                      "%h", 0,
                      "%c", 0x47,
                      "%u", get_character_id(state, character),
                      "%u", character->x,
                      "%u", character->y,
                      "%u", character->z,
                      "%u", character->heading);
            
            goto arrived;
        }
        
        // stop the movement if we have arrived.
        u32 d2 = distance_between_characters(character, target);
        if (d2 <= sqr(payload.offset)) {
            payload.target_x = character->x;
            payload.target_y = character->y;
            payload.target_z = character->z;
            
            // stop movement.
            broadcast(state,
                      character, 1,
                      "%h", 0,
                      "%c", 0x47,
                      "%u", get_character_id(state, character),
                      "%u", character->x,
                      "%u", character->y,
                      "%u", character->z,
                      "%u", character->heading);
            
            goto arrived;
        }

        // recalculate
        move_to(state, 
                character, 
                target->x, 
                target->y, 
                target->z, 
                payload.offset, 
                payload.target_id, 
                0);
        return;
    }

    // correct position if client position and server
    // position are way too off.
    u32 distance = distance_between(character->x, 
                                    character->y, 
                                    character->z,
                                    character->client_x, 
                                    character->client_y, 
                                    character->z);
    if (distance < sqr(32))
        push_response(character->conn, 1,
                      "%h", 0,
                      "%c", 0x61,
                      "%u", get_character_id(state, character),
                      "%u", character->x,
                      "%u", character->y,
                      "%u", character->z,
                      "%u", character->heading);
    return;
    
    arrived:
    character->x = payload.target_x;
    character->y = payload.target_y;
    character->z = payload.target_z;
    
    character->action_type = character->prev_action_type;
    character->action_payload = character->prev_action_payload;
    
    character->prev_action_type = idle;
    character->prev_action_payload = (union action_payload) {0};

    character->active = 0;
    broadcast(state,
              character, 1,
              "%h", 0,
              "%c", 0x61,
              "%u", get_character_id(state, character),
              "%u", character->x,
              "%u", character->y,
              "%u", character->z,
              "%u", character->heading);
    character->active = 1;

    // when arrived, if there was a queued action
    // try to update the character and push any 
    // packets if needed. this is done so we don't 
    // have to wait for the tick event to fire.
    // for instance, when attacking, if the target
    // is too far, we must first walk closer to the
    // target and then attack. the flow then goes as
    // walk closer -> attack. if we didn't update
    // the character, the flow would be
    // walk -> wait for tick event -> attack, which
    // doesn't seem like much but it's a HUGE difference.
    // the tick event refers to the function that updates
    // the world state every second.
    character_update(state, character);
    if (character->conn)
        wpush(&state->send_responses_worker, character->conn);
}

static void attacking_update(struct state *state, struct character *attacker)
{
    u32 attacker_id = get_character_id(state, attacker);
    u32 target_id = attacker->action_payload.attacking.obj_id;
    struct character *target = get_character_by_id(state, target_id);
    
    u32 atk_range = 60;
    
    coroutine(attacker->action_payload.attacking.state) {
        if (!target || !target->active) {
            attacker->action_type = idle;
            return;
        }
        
        u32 d2 = distance_between_characters(attacker, target);
        
        // check if we are in attack range.
        // if not in range, walk closer to the
        // target.
        if (d2 > sqr(atk_range)) {
            s32 target_x = target->x;
            s32 target_y = target->y;
            s32 target_z = target->z;
            u32 offset = atk_range - 10;
            move_to(state, 
                    attacker, 
                    target_x, 
                    target_y, 
                    target_z, 
                    offset, 
                    target_id, 
                    1);
            return;
        }
        
        u32 damage = 2;
        
        // start aggro state.
        broadcast(state,
                  attacker, 1,
                  "%h", 0,
                  "%c", 0x2b,
                  "%u", target_id);
        
        // launch attack.
        broadcast(state,
                  attacker, 1,
                  "%h", 0,
                  "%c", 0x05,
                  "%u", attacker_id,
                  "%u", target_id,
                  "%u", damage,
                  // flags
                  "%c", 0,
                  "%u", attacker->x,
                  "%u", attacker->y,
                  "%u", attacker->z,
                  // TODO(not-set): i think this should be 0
                  "%h", 0);
        
#if 1
        int is_target_npc = !target->conn;
        if (is_target_npc && target->action_type != attacking) {
            target->action_type = attacking;
            target->action_payload = (union action_payload) {0};
            target->action_payload.attacking.obj_id = attacker_id;
        }
#endif
        
        trace("launch first attack." nl);
        syield(200.0f, state->d);
        trace("wait completed, reset!" nl);
        reset;
    }
}

static u32 distance_between(s32 x, s32 y, s32 z, s32 x2, s32 y2, s32 z2)
{
    s32 dx = x2 - x;
    s32 dy = y2 - y;
    s32 dz = z2 - z;
    u32 d2 = sqr(dx) + sqr(dy) + sqr(dz);
    
    return d2;
}

static u32 distance_between_characters(struct character *a, struct character *b)
{
    if (!a || !a->active || !b || !b->active)
        return 0;
    
    s32 dx = b->x - a->x;
    s32 dy = b->y - a->y;
    s32 dz = b->z - a->z;
    u32 d2 = sqr(dx) + sqr(dy) + sqr(dz);
    
    return d2;
}
