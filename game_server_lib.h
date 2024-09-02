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
// #include <pthread.h> // pthread_create
#include <unistd.h>  // sleep
#endif

#include "directory.h"
#include "net.h"
#include "pevent.h"

typedef uint8_t byte;

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef   int8_t  s8;
typedef  int16_t s16;
typedef  int32_t s32;
typedef  int64_t s64;

typedef float seconds;

#define nl "\n"
#define kb * 1024
#define mb * 1024 * 1024
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

#define coroutine(x)                    \
    struct coroutine *__coro = &(x);    \
    switch (__coro->line)               \
        case 0:

#define yield                           \
    {                                   \
        __coro->line = __COUNTER__ + 1; \
        break;                          \
    }                                   \
    case __COUNTER__:

#define syield(sleep, delta)            \
    {                                   \
        __coro->sleep_for = (sleep);    \
        yield;                          \
        __coro->sleep_for -= (delta);   \
        if (__coro->sleep_for > 0)      \
            break;                      \
    }

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
    int wait_before_attacking;
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
        dead,
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

struct state {
    clock_t last_clock;
    float d;
    double run_time;
    u64 ticks;
    struct connection connections[1024];
    struct character characters[1024];
};

static int on_init(void **buf);
static void on_tick(struct state *state);
static void on_connection(struct state *state, int socket);
static void on_request(struct state *state, int socket, void *request, size_t len);
static void on_disconnect(struct state *state, int socket);

static void send_responses(struct connection *conn);

static void character_update(struct state *state, struct character *character);

static byte *bscanf_va(byte *src, size_t n, va_list va);
static byte *bscanf_(byte *src, size_t n, ...);
static byte *bprintf_va(byte *dest, size_t n, va_list va);
static byte *bprintf_(byte *dest, size_t n, ...);

static u16 packet_size(byte *src);
static void push_response_va(struct connection *conn, int _encrypt, va_list va);
static void push_response_(struct connection *conn, int _encrypt, ...);
static void broadcast_(struct state *state, struct character *character, int _encrypt, ...);
static void encrypt(struct connection *conn, byte *packet);
static void decrypt(struct connection *conn, byte *request);

static struct connection *get_connection_from_socket(struct state *state, int socket);

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
static void send_attr_status(struct state *state, struct character *from, struct character *to, enum attr_status *status, size_t n);

// get character by id. if none is found, null will be returned.
static struct character *get_character_by_id(struct state *state, u32 id);
static u32 get_character_id(struct state *state, struct character *src);

static void moving_update(struct state *state, struct character *character);
static void attacking_update(struct state *state, struct character *character);

static u32 distance_between(s32 x, s32 y, s32 z, s32 x2, s32 y2, s32 z2);
static u32 distance_between_characters(struct character *a, struct character *b);
