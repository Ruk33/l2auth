#include <assert.h> // assert
#include <stdlib.h> // calloc, size_t
#include <stdio.h>  // fprintf
#include <string.h> // memset, memcpy, strncpy, strnlen
#include <stdint.h> // fixed int types
#include <locale.h>
#include <wchar.h>

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
#define countof(x) (sizeof(x) / sizeof(*(x)))

#define append(dest, src) \
(memcpy((dest), &(src), sizeof(src)), (dest) + sizeof(src))
#define scan(dest, src) \
(memcpy(&(dest), (src), sizeof(dest)), (src) += sizeof(dest))
#define sscan(dest, src) \
(strncpy((dest), (const char *) (src), sizeof(dest) - 1), (src) += strnlen((dest), sizeof(dest)))
#define wsscan(dest, src) \
(wcsncpy((dest), (const wchar_t *) (src), sizeof(dest) - 1), (src) += wcsnlen((dest), sizeof(dest)))

#define coroutine(x) \
struct coroutine *__coro = &(x); \
switch (__coro->line) \
case 0:

#define yield \
__coro->line = __COUNTER__ + 1; \
return; \
case __COUNTER__:

#define syield(sleep, delta) \
__coro->sleep_for = (sleep); \
yield; \
__coro->sleep_for -= (delta); \
if (coro->sleep_for > 0) \
return;

struct coroutine {
    int line;
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
    
    byte to_send[1024];
    size_t to_send_count;
    size_t sent;
    
    byte request[1024];
    size_t request_head;
    size_t request_count;
};

struct state {
    struct connection connections[32];
};

static struct connection *get_connection_from_socket(struct state *state, int socket);
static void handle_request(struct state *state, struct connection *conn);
static u16 checksum(byte *dest, byte *start, byte *end);
static void encrypt(struct connection *conn, byte *packet);
static void decrypt(struct connection *conn, byte *request);
static void send_protocol(struct state *state, struct connection *conn);
static void handle_auth(struct state *state, struct connection *conn, byte *req);

int on_init(void **buf)
{
    assert(buf);
    size_t to_alloc = 10 mb;
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
    if (conn->request_head + len > countof(conn->request))
        conn->request_head = 0;
    memcpy(conn->request + conn->request_head, request, len);
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
        trace("sending %d bytes of data" nl, 
              (s32) (conn->to_send_count - conn->sent));
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
    byte *request = conn->request + conn->request_head;
    
    u16 size = 0;
    memcpy(&size, request, sizeof(size));
    
    if (size > conn->request_count - conn->request_head)
        return;
    if (conn->encrypted)
        decrypt(conn, request);
    request += sizeof(size);
    
    byte type = 0;
    memcpy(&type, request, sizeof(type));
    trace("received packet is of type %d" nl, (int) type);
    
    coroutine(conn->state) {
        if (type != 0x00) {
            trace("expecting 0x00 but got %d. the connection will be dropped." nl, (int) type);
            asocket_close(conn->socket);
            conn->socket = 0;
            return;
        }
        
        send_protocol(state, conn);
        conn->encrypted = 1;
        
        yield;
        if (type != 0x08) {
            trace("expecting 0x08 but got %d. the connection will be dropped." nl, (int) type);
            asocket_close(conn->socket);
            conn->socket = 0;
            return;
        }
        
        handle_auth(state, conn, request);
        
        yield;
    }
}

static u16 checksum(byte *dest, byte *start, byte *end)
{
    assert(dest);
    assert(start);
    assert(end);
    assert(start < end);
    assert(end - start < 65535);
    
    u16 size = (u16) (end - start);
    u32 result = 0;
    for (u16 i = 0; i < size; i += 4) {
        u32 ecx = *start++ & 0xff;
        ecx |= (*start++ << 8) & 0xff00;
        ecx |= (*start++ << 0x10) & 0xff0000;
        ecx |= (*start++ << 0x18) & 0xff000000;
        result ^= ecx;
    }
    
    append(end, result);
    size += (u16) sizeof(result);
    
    // the packet must be multiple of 8
    u16 body_padded_size = ((size + 7) & (~7));
    u16 size_header = 2;
    // the final size of the packet consists of 
    // the padded size plus 2 bytes used to store how
    // big the packet is.
    u16 final_size = body_padded_size + size_header;
    memcpy(dest, &final_size, sizeof(final_size));
    
    return final_size;
}

static void encrypt(struct connection *conn, byte *packet)
{
    assert(conn);
    assert(packet);
    
    u16 size = 0;
    memcpy(&size, packet, sizeof(size));
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
    
    u16 size = 0;
    memcpy(&size, request, sizeof(size));
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

static void send_protocol(struct state *state, struct connection *conn)
{
    assert(state);
    assert(conn);
    
    byte *start = conn->to_send + sizeof(u16);
    byte *end = start;
    
    byte type = 0x00;
    end = append(end, type);
    
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
    end = append(end, protocol);
    
    u16 size = checksum(conn->to_send, start, end);
    conn->to_send_count += size;
}

static void handle_auth(struct state *state, struct connection *conn, byte *req)
{
    assert(state);
    assert(conn);
    assert(req);
    
    req++;
    wsscan(conn->username, req);
    trace("name is '%ls'" nl, conn->username);
    scan(conn->play_ok2, req);
    scan(conn->play_ok1, req);
    scan(conn->login_ok1, req);
    scan(conn->login_ok2, req);
}
