#include <assert.h> // assert
#include <stddef.h> // size_t
#include <stdio.h>  // fprintf, fscanf, fopen, fclose, sscanf
#include <string.h> // memcpy
#include <stdint.h> // fixed int types
#include <time.h>   // time_t, time

#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>

#include "asocket.h"
#include "blowfish.h"

typedef uint8_t byte;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define nl "\n"

// length of array
#define countof(x) (sizeof(x) / sizeof(*(x)))

// printf alternative.
// write to stderr so we don't have to fflush the buffer
#define trace(...) fprintf(stderr, __VA_ARGS__)

// append src to dest and return dest + sizeof(src)
#define append(dest, src) (memcpy((dest), &(src), sizeof(src)), (dest) + sizeof(src))

struct connection {
    int socket;
    char username[32];
    u32 login_ok1;
    u32 login_ok2;
    // buffer reserved for the response of the connection/client.
    // when bytes are sent, we have to keep in mind that
    // sometimes the entire response can't be sent as once, 
    // instead, in chunks. if that's the case, "sent" records
    // how many bytes of the entire response ("to_send_count")
    // has been already sent.
    byte to_send[1024];
    size_t to_send_count;
    size_t sent;
    // buffer reserved for the request of client. this is intended
    // to be a circular buffer, meaning, when it's full, the beginning
    // of the buffer will be written again. "request_head" is
    // the start of the request being handled. keep in mind, the client
    // can send multiple requests in one big chunk. when the request
    // is handled, request_head will be updated to be
    // request_head + the size of the request we just handled.
    byte request[1024];
    size_t request_head;
    size_t request_count;
    
    BLOWFISH_CTX blowfish;
    BIGNUM *rsa_e;
    RSA *rsa_key;
};

static struct connection connections[32];

static u32 ip_to_u32(char *ip)
{
    if (!ip)
        return 0;
    u8 ip_bytes[4] = {0};
    sscanf(ip, 
           "%u.%u.%u.%u", 
           &ip_bytes[3], 
           &ip_bytes[2], 
           &ip_bytes[1], 
           &ip_bytes[0]);
    u32 result = ip_bytes[0] | ip_bytes[1] << 8 | ip_bytes[2] << 16 | ip_bytes[3] << 24;
    return result;
}

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
            connections[i].to_send_count = 0;
            connections[i].sent = 0;
            connections[i].request_head = 0;
            connections[i].request_count = 0;
            return connections + i;
        }
    }
    return 0;
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

static void encrypt_packet(struct connection *conn, byte *start, byte *end)
{
    assert(conn);
    assert(start);
    assert(end);
    assert(start < end);
    assert(end - start < 65535);
    u16 size = (u16) (end - start);
    for (u16 i = 0; i < size; i += 8)
        Blowfish_Encrypt(&conn->blowfish, 
                         (u32 *) (start + i),
                         (u32 *) (start + i + 4));
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
    // credits: l2j
    {
        byte *modulus = init.modulus;
        //
        for (int i = 0; i < 4; i++) {
            byte temp = modulus[i];
            modulus[i] = modulus[0x4d + i];
            modulus[0x4d + i] = temp;
        };
        // step 2 xor first 0x40 bytes with last 0x40 bytes
        for (int i = 0; i < 0x40; i++)
            modulus[i] = (byte) (modulus[i] ^ modulus[0x40 + i]);
        // step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38
        for (int i = 0; i < 4; i++)
            modulus[0x0d + i] = (byte) (modulus[0x0d + i] ^ modulus[0x34 + i]);
        // step 4 xor last 0x40 bytes with first 0x40 bytes
        for (int i = 0; i < 0x40; i++)
            modulus[0x40 + i] = (byte) (modulus[0x40 + i] ^ modulus[i]);
    }
    
    byte *start = conn->to_send + sizeof(u16);
    byte *end = start;
    
    byte type = 0x00;
    end = append(end, type);
    end = append(end, init.session_id);
    end = append(end, init.protocol);
    end = append(end, init.modulus);
    
    conn->to_send_count += checksum(conn->to_send, start, end);
    trace("sending init packet" nl);
}

static void send_ignore_gg_packet(struct connection *conn)
{
    assert(conn);
    
    byte *start = conn->to_send + sizeof(u16);
    byte *end = start;
    
    byte type = 0x0b;
    end = append(end, type);
    
    u32 ignore_gg = 0x0b;
    end = append(end, ignore_gg);
    
    u16 size = checksum(conn->to_send, start, end);
    encrypt_packet(conn, start, start + size - 2);
    
    conn->to_send_count += size;
}

static void handle_auth_request(struct connection *conn, byte *request)
{
    assert(conn);
    assert(request);
    
    byte *content = request + 1;
    char *username = (char *) content + 0x62;
    // char *password = (char *) content + 0x70;
    
    trace("user %s is trying to authenticate" nl, username);
    // trace("password is %s" nl, password);
    strncpy(conn->username, username, sizeof(conn->username) - 1);
    
    byte *start = conn->to_send + sizeof(u16);
    byte *end = start;
    
    // assume success login.
    u8 type = 0x03;
    end = append(end, type);
    
    RAND_bytes((unsigned char *) &conn->login_ok1, sizeof(conn->login_ok1));
    end = append(end, conn->login_ok1);
    
    RAND_bytes((unsigned char *) &conn->login_ok2, sizeof(conn->login_ok2));
    end = append(end, conn->login_ok2);
    
    byte unknown[] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0xea, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x60, 0x62, 0xe0, 0x00,
        0x00, 0x00, 0x00,
    };
    end = append(end, unknown);
    
    u16 size = checksum(conn->to_send, start, end);
    encrypt_packet(conn, start, start + size - 2);
    
    conn->to_send_count += size;
    
    trace("access granted to %s" nl, username);
}

static void handle_server_list_request(struct connection *conn)
{
    assert(conn);
    
    struct {
        u8 id;
        union {
            char text_ip[sizeof("255.255.255.255")];
            u32 ip;
        };
        u32 port;
        u8 age_limit;
        u8 pvp;
        u16 players;
        u16 max_players;
        u8 status;
        u32 extra;
        u8 brackets;
    } servers[8] = {0};
    u8 server_count = 0;
    
    trace("%s requested the servers list" nl, conn->username);
    
    FILE *servers_file = fopen("servers.txt", "r");
    if (servers_file) {
        while (server_count < (u8) countof(servers)) {
            int matched = fscanf(servers_file, 
                                 "id=%d" nl
                                 "ip=%s" nl
                                 "port=%d" nl
                                 // "age_limit=%d" nl
                                 // "pvp=%d" nl
                                 "max_players=%d" nl
                                 "status=%d" nl
                                 // "extra=%d" nl
                                 // "brackets=%d" nl
                                 ,
                                 &servers[server_count].id,
                                 servers[server_count].text_ip,
                                 &servers[server_count].port,
                                 // &servers[server_count].age_limit,
                                 // &servers[server_count].pvp,
                                 &servers[server_count].max_players,
                                 &servers[server_count].status);
            
            if (matched != 5)
                break;
            
            servers[server_count].ip = ip_to_u32(servers[server_count].text_ip);
            servers[server_count].age_limit = 18;
            servers[server_count].pvp = 1;
            servers[server_count].extra = 0;
            servers[server_count].brackets = 0;
            
            server_count++;
        }
        fclose(servers_file);
        
        if (!server_count)
            trace("WARNING: i couldn't find any server in servers.txt." nl
                  "just in case, this is the format i'm expecting:" nl
                  nl
                  "id=1" nl
                  "ip=0.0.0.0" nl
                  "port=7777" nl
                  "max_players=1000" nl
                  "status=1" nl
                  nl);
    } else {
        server_count = 1;
        servers[0].id = 1;
        servers[0].port = 7777;
        servers[0].max_players = 1000;
        servers[0].status = 1;
        
        trace("WARNING! no servers.txt file found, i will try to create a default one" nl);
        
        FILE *default_servers_file = fopen("servers.txt", "w+");
        if (default_servers_file) {
            fprintf(default_servers_file, 
                    "id=%d" nl
                    "ip=0.0.0.0" nl
                    "port=%d" nl
                    "max_players=%d" nl
                    "status=%d" nl,
                    servers[0].id,
                    servers[0].port,
                    servers[0].max_players,
                    servers[0].status);
            fclose(default_servers_file);
        } else {
            trace("i was unable to create the default file." nl
                  "please check the folder's permission or, " nl
                  "create a servers.txt file manually with:" nl
                  nl
                  "id=%d" nl
                  "ip=0.0.0.0" nl
                  "port=%d" nl
                  "max_players=%d" nl
                  "status=%d" nl
                  nl
                  "in the meantime, i'll send the player the default server" nl
                  "i would if you had the default servers.txt file i just" nl
                  "showed you" nl,
                  servers[0].id,
                  servers[0].port,
                  servers[0].max_players,
                  servers[0].status);
        }
    }
    
    byte *start = conn->to_send + sizeof(u16);
    byte *end = start;
    
    byte type = 0x04;
    end = append(end, type);
    
    end = append(end, server_count);
    
    u8 unknown = 0;
    end = append(end, unknown);
    
    for (u8 i = 0; i < server_count; i += 1) {
        end = append(end, servers[i].id);
        end = append(end, servers[i].ip);
        end = append(end, servers[i].port);
        end = append(end, servers[i].age_limit);
        end = append(end, servers[i].pvp);
        end = append(end, servers[i].players);
        end = append(end, servers[i].max_players);
        end = append(end, servers[i].status);
        end = append(end, servers[i].extra);
        end = append(end, servers[i].brackets);
    }
    
    u16 size = checksum(conn->to_send, start, end);
    encrypt_packet(conn, start, start + size - 2);
    
    conn->to_send_count += size;
}

static void handle_enter_game_server(struct connection *conn)
{
    assert(conn);
    
    FILE *access_file = fopen("login_access.txt", "a");
    if (!access_file) {
        trace("unable to open/create login_access.txt file." nl
              "this file is used to check if a connection to a game server" nl
              "actually went through the login server successfully." nl
              "the connection with %s will be dropped" nl,
              conn->username);
        asocket_close(conn->socket);
        conn->socket = 0;
        conn->username[0] = 0;
        return;
    }
    time_t now = time(0);
    time_t created_at = now;
    // make these ids valid for 1 minute
    time_t valid_until = now + 60;
    struct tm created_at_tm = *gmtime(&created_at);
    struct tm valid_until_tm = *gmtime(&valid_until);
    char created_at_str[128] = {0};
    char valid_until_str[128] = {0};
    strftime(created_at_str, 
             sizeof(created_at_str) - 1, 
             "%Y-%m-%d %H:%M:%S", 
             &created_at_tm);
    strftime(valid_until_str, 
             sizeof(valid_until_str) - 1, 
             "%Y-%m-%d %H:%M:%S", 
             &valid_until_tm);
    // save these ids so later the game server can check that the user
    // went through the login server successfully.
    // save dates in utc format, in number (easier to check) and 
    // formatted, easier to read & debug :)
    fprintf(access_file, 
            "username=%s" nl
            "login_ok1=%u" nl
            "login_ok2=%u" nl
            "created_at=%u" nl
            "valid_until=%u" nl
            "created_at(yyyy-mm-dd hh:mm:ss utc)=%s" nl
            "valid_until(yyyy-mm-dd hh:mm:ss utc)=%s" nl,
            conn->username,
            conn->login_ok1,
            conn->login_ok2,
            created_at,
            valid_until,
            created_at_str,
            valid_until_str);
    fclose(access_file);
    
    byte *start = conn->to_send + sizeof(u16);
    byte *end = start;
    
    byte type = 0x07;
    end = append(end, type);
    
    end = append(end, conn->login_ok1);
    end = append(end, conn->login_ok2);
    
    u16 size = checksum(conn->to_send, start, end);
    encrypt_packet(conn, start, start + size - 2);
    
    conn->to_send_count += size;
    
    trace("the user %s entered the game server sucessfully" nl, conn->username);
}

static void on_request(struct connection *conn)
{
    assert(conn);
    
    byte *request = conn->request + conn->request_head;
    
    u16 size = 0;
    memcpy(&size, request, sizeof(size));
    
    // check for incomplete packet.
    if (size > conn->request_count - conn->request_head)
        return;
    
    trace("new packet of size %d (mod 8 = %d)" nl, (int) size, size % 8);
    
    u16 body_size = size - (u16) sizeof(size);
    request += sizeof(size);
    trace("packet body size %d (mod 8 = %d)" nl, (int) body_size, body_size % 8);
    
    // blowfish decrypt
    for (u16 i = 0; i < body_size; i += 8)
        Blowfish_Decrypt(&conn->blowfish, 
                         (u32 *) (request + i), 
                         (u32 *) (request + i + 4));
    
    // rsa decrypt
    // +1 don't include the packet type, just the body of the packet
    RSA_private_decrypt(RSA_size(conn->rsa_key),
                        request + 1,
                        request + 1,
                        conn->rsa_key,
                        RSA_NO_PADDING);
    
    byte type = 0;
    memcpy(&type, request, sizeof(type));
    trace("received packet is of type %d" nl, (int) type);
    
    // requests are in order, first 0x07, then 0x00, and so on.
    switch (type) {
        case 0x07:
        send_ignore_gg_packet(conn);
        break;
        
        case 0x00:
        handle_auth_request(conn, request);
        break;
        
        case 0x05:
        handle_server_list_request(conn);
        break;
        
        case 0x02:
        handle_enter_game_server(conn);
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
                trace("no more room. can't accept new connection (will be dropped)" nl);
                asocket_close(socket);
                return;
            }
            conn->socket = socket;
            
            // blowfish key.
            char key[] = "_;5.]94-31==-%xT!^[$";
            Blowfish_Init(&conn->blowfish, (unsigned char *) key, sizeof(key));
            
            // generate a new rsa key if required.
            if (!conn->rsa_key) {
                conn->rsa_key = RSA_new();
                BN_dec2bn(&conn->rsa_e, "65537");
                RSA_generate_key_ex(conn->rsa_key, 1024, conn->rsa_e, 0);
            }
            
            send_init_packet(conn);
        } break;
        
        case ASOCKET_CLOSED: {
            conn->socket = 0;
            conn->username[0] = 0;
            trace("client closed the connection" nl);
        } break;
        
        case ASOCKET_READ: {
            trace("bytes %d received from client" nl, (s32) len);
            // circular buffer. 
            // start from the beginning of the buffer is there is no
            // more room to store the request.
            if (conn->request_head + len > countof(conn->request))
                conn->request_head = 0;
            memcpy(conn->request + conn->request_head, read, len);
            conn->request_count += len;
            on_request(conn);
        } break;
        
        case ASOCKET_CAN_WRITE: {
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
        } break;
        
        default:
        break;
    }
}

int main()
{
#define port 2106
    int socket = asocket_port(port);
    trace("login server, listening for connection on port %d" nl, port);
    asocket_listen(socket, handle_event);
    return 0;
}
