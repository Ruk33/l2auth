#include <assert.h> // assert
#include <stddef.h> // size_t
#include <stdio.h>  // fprintf
#include <string.h> // memcpy, memset

// #define LTM_DESC
// #include <tomcrypt.h>
// #include <tommath.h>

#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>

#include "asocket.h"
#include "blowfish.h"
// #include "rsa.h"

typedef unsigned char uchar;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;

#define nl "\n"
#define countof(x) (sizeof(x) / sizeof(*(x)))
#define trace(...) fprintf(stderr, __VA_ARGS__)
#define append(dest, src) (memcpy(dest, &(src), sizeof(src)), dest + sizeof(src))

struct connection {
    int socket;
    
    byte to_send[1024];
    uint to_send_count;
    uint sent;
    
    byte request[1024];
    uint request_head;
    uint request_count;
    
    BLOWFISH_CTX blowfish;
    
    // rsa_key rsa;
    BIGNUM *rsa_e;
    RSA *rsa_key;
};

static struct connection connections[32];

static struct connection *find_connection(int socket)
{
    // check for a connection already using this socket.
    for (size_t i = 0; i < countof(connections); i++) {
        if (connections[i].socket == socket)
            return connections + i;
    }
    // or try to give a new usable connection.
    for (size_t i = 0; i < countof(connections); i++) {
        if (!connections[i].socket) {
            // memset(connections + i, 0, sizeof(struct connection));
            connections[i].to_send_count = 0;
            connections[i].sent = 0;
            connections[i].request_head = 0;
            connections[i].request_count = 0;
            return connections + i;
        }
    }
    return 0;
}

static ushort checksum(byte *dest, byte *start, byte *end)
{
    assert(dest);
    assert(start);
    assert(end);
    assert(start < end);
    assert(end - start < 65535);
    
    ushort size = (ushort) (end - start);
    uint result = 0;
    for (ushort i = 0; i < size; i += 4) {
        uint ecx = *start++ & 0xff;
        ecx |= (*start++ << 8) & 0xff00;
        ecx |= (*start++ << 0x10) & 0xff0000;
        ecx |= (*start++ << 0x18) & 0xff000000;
        result ^= ecx;
    }
    
    append(end, result);
    size += (ushort) sizeof(result);
    
    ushort final_size = ((size + 7) & (~7)) + 2;
    memcpy(dest, &final_size, sizeof(final_size));
    
    trace("packet checksum, final size is %d" nl, (int) final_size);
    
    return final_size;
}

static void encrypt_packet(struct connection *conn, byte *start, byte *end)
{
    assert(conn);
    assert(start);
    assert(end);
    assert(start < end);
    assert(end - start < 65535);
    ushort size = end - start;
    for (ushort i = 0; i < size; i += 8) {
        Blowfish_Encrypt(&conn->blowfish,
                         (uint32_t *) (start + i),
                         (uint32_t *) (start + i + 4));
    }
}

static void send_init_packet(struct connection *conn)
{
    assert(conn);
    
    struct {
        byte session_id[4];
        byte protocol[4];
        byte modulus[128];
    } init = {
        {0xfd, 0x8a, 0x22, 0x00}, // unknown?
        {0x5a, 0x78, 0x00, 0x00}, // c4 protocol
        0,
    };
    
    const BIGNUM *n = 0;
    RSA_get0_key(conn->rsa_key, &n, 0, 0);
    BN_bn2bin(n, init.modulus);
    
    // scramble modulus
    {
        byte *n = init.modulus;
        //
        for (int i = 0; i < 4; i++) {
            byte temp = n[i];
            n[i] = n[0x4d + i];
            n[0x4d + i] = temp;
        };
        // step 2 xor first 0x40 bytes with last 0x40 bytes
        for (int i = 0; i < 0x40; i++)
            n[i] = (byte) (n[i] ^ n[0x40 + i]);
        // step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38
        for (int i = 0; i < 4; i++)
            n[0x0d + i] = (byte) (n[0x0d + i] ^ n[0x34 + i]);
        // step 4 xor last 0x40 bytes with first 0x40 bytes
        for (int i = 0; i < 0x40; i++)
            n[0x40 + i] = (byte) (n[0x40 + i] ^ n[i]);
    }
    
    byte *start = conn->to_send + sizeof(ushort);
    byte *end = start;
    
    byte type = 0x00;
    end = append(end, type);
    end = append(end, init.session_id);
    end = append(end, init.protocol);
    end = append(end, init.modulus);
    
    conn->to_send_count += checksum(conn->to_send, start, end);
}

static void send_ignore_gg_packet(struct connection *conn)
{
    assert(conn);
    
    byte *start = conn->to_send + sizeof(ushort);
    byte *end = start;
    
    byte type = 0x0b;
    end = append(end, type);
    
    uint ignore_gg = 0x0b;
    end = append(end, ignore_gg);
    
    ushort size = checksum(conn->to_send, start, end);
    encrypt_packet(conn, start, start + size - 2);
    
    conn->to_send_count += size;
}

static void check_auth_credentials(byte *request)
{
    assert(request);
    
    byte *content = request + 1;
    byte *username = content + 0x62;
    byte *password = content + 0x70;
    
    trace("username is %s" nl, username);
    trace("password is %s" nl, password);
}

static void on_request(struct connection *conn)
{
    assert(conn);
    
    byte *request = conn->request + conn->request_head;
    
    ushort size = 0;
    memcpy(&size, request, sizeof(size));
    
    // check for incomplete packet.
    if (size > conn->request_count - conn->request_head)
        return;
    
    trace("new packet of size %d (mod 8 = %d)" nl, (int) size, size % 8);
    
    ushort body_size = size - (ushort) sizeof(size);
    request += sizeof(size);
    trace("packet body size %d (mod 8 = %d)" nl, (int) body_size, body_size % 8);
    
    // blowfish decrypt
    {
        for (ushort i = 0; i < body_size; i += 8) {
            Blowfish_Decrypt(&conn->blowfish, 
                             (uint32_t *) (request + i), 
                             (uint32_t *) (request + i + 4));
        }
    }
    
    byte type = 0;
    memcpy(&type, request, sizeof(type));
    trace("received packet is of type %d" nl, (int) type);
    
    // rsa decrypt
    {
        RSA_private_decrypt(RSA_size(conn->rsa_key),
                            request + 1,
                            request + 1,
                            conn->rsa_key,
                            RSA_NO_PADDING);
    }
    
    switch (type) {
        case 0x07:
        send_ignore_gg_packet(conn);
        break;
        
        case 0x00:
        check_auth_credentials(request);
        break;
        
        default:
        break;
    }
    
    conn->request_head += size;
}

static void handle_event(int socket, enum asocket_event event, void *read, size_t len)
{
    struct connection *conn = find_connection(socket);
    switch (event) {
        case ASOCKET_NEW_CONN: {
            if (!conn) {
                trace("no more room. can't accept new connection. will be dropped" nl);
                asocket_close(socket);
                return;
            }
            conn->socket = socket;
            
            // blowfish key.
            char key[] = "_;5.]94-31==-%xT!^[$";
            Blowfish_Init(&conn->blowfish, key, sizeof(key));
            
            // rsa key.
            if (!conn->rsa_key) {
                conn->rsa_key = RSA_new();
                BN_dec2bn(&conn->rsa_e, "65537");
                RSA_generate_key_ex(conn->rsa_key, 1024, conn->rsa_e, 0);
            }
            
            send_init_packet(conn);
            trace("sending init packet" nl);
        } break;
        
        case ASOCKET_CLOSED: {
            conn->socket = 0;
            trace("client closed the connection" nl);
        } break;
        
        case ASOCKET_READ: {
            trace("bytes %d received from client" nl, (int) len);
            if (conn->request_head + len > countof(conn->request))
                conn->request_head = 0;
            memcpy(conn->request + conn->request_head, read, len);
            conn->request_count += (uint) len;
            on_request(conn);
        } break;
        
        case ASOCKET_CAN_WRITE: {
            if (conn->sent < conn->to_send_count) {
                trace("sending %d bytes of data" nl, conn->to_send_count - conn->sent);
                conn->sent += asocket_write(conn->socket,
                                            conn->to_send + conn->sent,
                                            conn->to_send_count - conn->sent);
            }
            // reset counters when all data has been sent.
            if (conn->sent >= conn->to_send_count) {
                conn->sent = 0;
                conn->to_send_count = 0;
            }
        } break;
        
        default:
        break;
    }
}

int main()
{
    int socket = asocket_port(2106);
    asocket_listen(socket, handle_event);
    return 0;
}
