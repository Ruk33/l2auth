#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#endif

#include <stdio.h>
#include <stdint.h>

#include "utils.c"
#include "packet.c"

#ifdef _WIN32
#include "net_windows.c"
#endif

#ifdef __linux__
#include "net_linux.c"
#endif

struct connection {
    struct net_socket socket;

    int is_encrypted;

    byte encrypt_key[8];
    byte decrypt_key[8];

    byte response_buffer[8 kb];
    byte request_buffer[8 kb];

    u32 response_size;
    u32 request_size;
};

struct state {
    struct connection connections[1024];
};

void init(byte **memory)
{
    u64 memory_to_allocate = 128 mb;

    check(memory_to_allocate >= sizeof(struct state));

    *memory = get_memory(memory_to_allocate);

    trace("game server initialized" nl);
}

void drop_connection(struct connection *connection)
{
    net_close(connection->socket);
    connection->socket = (struct net_socket) {0};
}

void flush_responses(struct connection *connection)
{
    if (!connection->response_size)
        return;

    trace("flushing %d bytes of data" nl, connection->response_size);

    int sent = net_send(connection->socket,
                        connection->response_buffer,
                        connection->response_size);

    trace("flushed %d bytes of data (%d bytes remaining)" nl,
          sent,
          connection->response_size - sent);

    connection->response_size -= sent;

    move_memory(connection->response_buffer,
                connection->response_buffer + sent,
                connection->response_size);
}

void push_response(struct connection *connection, byte *packet)
{
    u16 size = packet_size(packet);

    u64 response_buffer_size = sizeof(connection->response_buffer);

    /*
     * Try flushing packets if there is no more room in the queue.
     */
    if (connection->response_size + size > response_buffer_size) {
        flush_responses(connection);

        /*
         * If we can't flush and make room for more packets, we will have to
         * drop the client.
         */
        if (connection->response_size + size > response_buffer_size) {
            trace("there is no more room to queue packets for this client. the client will be dropped." nl);

            drop_connection(connection);

            return;
        }
    }

    copy_memory(connection->response_buffer + connection->response_size,
                packet,
                size);

    connection->response_size += size;
}

void encrypt_packet(struct connection *connection, byte *packet)
{
    u16 size = packet_size(packet);

    u32 temp  = 0;
    u32 temp2 = 0;

    byte *body = packet + 2;

    for (u16 i = 0; i < size - 2; i++) {
        temp2   = body[i] & 0xff;
        body[i] = (byte) (temp2 ^ connection->encrypt_key[i & 7] ^ temp);
        temp    = body[i];
    }

    u32 old = 0;
    old  = ((u32) connection->encrypt_key[0] & 0xff);
    old |= ((u32) connection->encrypt_key[1] << 8 & 0xff00);
    old |= ((u32) connection->encrypt_key[2] << 0x10 & 0xff0000);
    old |= ((u32) connection->encrypt_key[3] << 0x18 & 0xff000000);

    old += size;

    connection->encrypt_key[0] = (byte) (old & 0xff);
    connection->encrypt_key[1] = (byte) (old >> 0x08 & 0xff);
    connection->encrypt_key[2] = (byte) (old >> 0x10 & 0xff);
    connection->encrypt_key[3] = (byte) (old >> 0x18 & 0xff);
}

void decrypt_packet(struct connection *conn, byte *request)
{
    u16 size = packet_size(request);

    u32 temp  = 0;
    u32 temp2 = 0;

    byte *body = request + 2;

    for (u16 i = 0; i < size - 2; i++) {
        temp2   = body[i];
        body[i] = (byte) (temp2 ^ conn->decrypt_key[i & 7] ^ temp);
        temp    = temp2;
    }

    u32 old = 0;
    old  = conn->decrypt_key[0] & 0xff;
    old |= conn->decrypt_key[1] << 8 & 0xff00;
    old |= conn->decrypt_key[2] << 0x10 & 0xff0000;
    old |= conn->decrypt_key[3] << 0x18 & 0xff000000;

    old += size;

    conn->decrypt_key[0] = (byte) (old & 0xff);
    conn->decrypt_key[1] = (byte) (old >> 0x08 & 0xff);
    conn->decrypt_key[2] = (byte) (old >> 0x10 & 0xff);
    conn->decrypt_key[3] = (byte) (old >> 0x18 & 0xff);
}

struct connection *find_connection(struct state *state, struct net_socket socket)
{
    /*
     * Find active connection.
     */
    for (u64 i = 0; i < array_length(state->connections); i++) {
        struct connection *connection = state->connections + i;
        if (connection->socket.handle == socket.handle)
            return connection;
    }

    /*
     * Find free connection to be used.
     */
    for (u64 i = 0; i < array_length(state->connections); i++) {
        struct connection *connection = state->connections + i;
        if (!connection->socket.handle)
            return connection;
    }

    return 0;
}

void push_protocol_packet(struct connection *connection)
{
    byte type = 0x00;

    byte protocol[] = {
        0x01,
        connection->encrypt_key[0],
        connection->encrypt_key[1],
        connection->encrypt_key[2],
        connection->encrypt_key[3],
        connection->encrypt_key[4],
        connection->encrypt_key[5],
        connection->encrypt_key[6],
        connection->encrypt_key[7],
    };

    byte *packet = packet(byte(type)
                          bytes2(protocol));

    push_response(connection, packet);

    trace("sending protocol packet" nl);
}

void try_handling_request(struct connection *connection)
{
    if (!connection->request_size)
        return;

    byte *request = connection->request_buffer;

    u16 size = packet_size(request);

    /*
     * Check for incomplete packet.
     */
    if (size > connection->request_size)
        return;

    if (connection->is_encrypted)
        decrypt_packet(connection, request);

    u8 type = packet_type(request);

    trace("here you need to handle a packet of type: %d" nl,
          (int) type);

    switch (type) {
    case 0x00:
        connection->is_encrypted = 1;
        push_protocol_packet(connection);
        break;
    default:
        trace("ok, no more implementations." nl);
        break;
    }

    move_memory(connection->request_buffer,
                connection->request_buffer + size,
                sizeof(connection->request_buffer) - size);

    connection->request_size -= size;

    try_handling_request(connection);
}

void handle_request(byte **memory, struct net_socket socket, enum net_event event, byte *read, int len)
{
    struct state *state = *(struct state **) memory;

    switch (event) {
        case net_conn: {
            struct connection *connection = find_connection(state, socket);

            trace("new client trying to connect to game server." nl);

            if (!connection) {
                trace("i can't accept more connection. i will drop the client." nl);
                net_close(socket);
                return;
            }

            trace("connection accepted, will send the init packet." nl);

            connection->socket = socket;

            byte key[] = {0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87};
            copy_memory(connection->encrypt_key, key, sizeof(key));
            copy_memory(connection->decrypt_key, key, sizeof(key));
        } break;

        case net_closed: {
            struct connection *connection = find_connection(state, socket);

            trace("client closed the connection." nl);

            if (connection)
                reset_memory(connection, sizeof(struct connection));
        } break;

        case net_read: {
            struct connection *connection = find_connection(state, socket);

            if (!connection)
                return;

            // TODO:
            // we need to make sure we don't read exceeding the buf size.
            // we could perform a loop, so we handle, make room, and then
            // continue handling the rest. if there is no more space,
            // drop the connection.

            copy_memory(connection->request_buffer + connection->request_size,
                        read,
                        len);

            connection->request_size += len;

            try_handling_request(connection);

            flush_responses(connection);
        } break;
    }
}

void update(byte **memory)
{
    struct state *state = *(struct state **) memory;

    for (u64 i = 0; i < array_length(state->connections); i++) {
        // struct connection *connection = state->connections + i;

        // if (!connection->socket.handle)
        //     continue;

        // // "input"
        // try_handling_request(connection);

        // // update

        // flush_packets(connection);
    }
}
