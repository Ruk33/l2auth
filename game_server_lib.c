#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#endif

#include <stdio.h>
#include <stdint.h>

#include "utils.c"
#include "thread.c"

#ifdef _WIN32
#include "net_windows.c"
#endif

#ifdef __linux__
#include "net_linux.c"
#endif

struct connection {
    struct net_socket socket;

    byte encrypt_key[8];
    byte decrypt_key[8];

    byte to_send[8 kb];
    byte request[8 kb];

    int to_send_size;
    int request_size;
};

struct state {
    struct connection connections[1024];

    struct lock lock;
};

void init(byte **memory)
{
    u64 memory_to_allocate = 128 mb;

    check(memory_to_allocate >= sizeof(struct state));

    *memory = get_memory(memory_to_allocate);

    struct state *state = *(struct state **) memory;

    state->lock = lock_init();

    trace("init executed" nl);
}

void push_packet(struct connection *connection, byte *end_of_packet, int encrypt)
{
    byte *start_of_packet = connection->to_send;

    u16 size = (u16) (end_of_packet - start_of_packet);

    connection->to_send_size += size;

    if (size > 1)
        size -= 2;

    if (encrypt) {
        byte *body = start_of_packet + 2;

        u32 temp = 0;
        u32 temp2 = 0;
        for (u16 i = 0; i < size; i++) {
            temp2 = body[i] & 0xff;
            body[i] = (byte) (temp2 ^ connection->encrypt_key[i & 7] ^ temp);
            temp = body[i];
        }

        u32 old = ((u32) connection->encrypt_key[0] & 0xff);
        old |= ((u32) connection->encrypt_key[1] << 8 & 0xff00);
        old |= ((u32) connection->encrypt_key[2] << 0x10 & 0xff0000);
        old |= ((u32) connection->encrypt_key[3] << 0x18 & 0xff000000);

        old += size;

        connection->encrypt_key[0] = (byte) (old & 0xff);
        connection->encrypt_key[1] = (byte) (old >> 0x08 & 0xff);
        connection->encrypt_key[2] = (byte) (old >> 0x10 & 0xff);
        connection->encrypt_key[3] = (byte) (old >> 0x18 & 0xff);
    }

    copy_memory(start_of_packet, &size, sizeof(size));
}

void decrypt(struct connection *conn, byte *request)
{
    check(conn);
    check(request);

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

byte *new_packet(struct connection *connection)
{
    return connection->to_send + connection->to_send_size + sizeof(u16);
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

void push_protocol(struct connection *connection)
{
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

    byte *packet = new_packet(connection);
    byte *tail = packet;

    tail = append(tail, type);
    tail = append(tail, protocol);

    push_packet(connection, tail, 0);
}

void flush_packets(struct connection *connection)
{
    if (!connection->to_send_size) {
        trace("nothing to send?" nl);
        return;
    }

    trace("sending %d bytes of data" nl, connection->to_send_size);
    int sent = net_send(connection->socket, connection->to_send, connection->to_send_size);

    connection->to_send_size -= sent;
    memmove(connection->to_send, connection->to_send + sent, connection->to_send_size);
}

void try_handling_request(struct connection *connection)
{
    byte *request = connection->request;

    u16 size = 0;
    copy_memory(&size, request, sizeof(size));

    /*
     * Check for incomplete packet.
     */
    if (size > connection->request_size)
        return;

    trace("here you need to handle a packet of type x" nl);
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

            byte key[] = {0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87};
            copy_memory(connection->encrypt_key, key, sizeof(key));
            copy_memory(connection->decrypt_key, key, sizeof(key));

            push_protocol(connection);
            flush_packets(connection);
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

            // lock(state->lock);

            copy_memory(connection->request + connection->request_size, read, len);
            connection->request_size += len;

            try_handling_request(connection);

            // unlock(state->lock);
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
