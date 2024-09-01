#define WIN32_LEAN_AND_MEAN

#include <assert.h> // assert
#include <stddef.h> // size_t
#include <stdio.h>  // fprintf, fscanf, fopen, fclose, fread, fwrite, fgetc, sscanf
#include <string.h> // memcpy, memcmp, strlen, strncpy, strcmp
#include <stdint.h> // fixed int types
#include <time.h>   // time_t, time

#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/blowfish.h>
#include <openssl/evp.h>

#include "directory.h"
#include "net.h"

typedef uint8_t byte;

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef  int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

u32 htonl(u32 x);
u32 ntohl(u32 x);

#define nl "\n"

/*
 * Minutes to seconds.
 */
#define minute * 60

/*
 * Kb in bytes.
 */
#define kb * 1024

#define countof(x) (sizeof(x) / sizeof(*(x)))

/*
 * printf alternative.
 * Write to stderr so we don't have to fflush the buffer.
 */
#define trace(...) fprintf(stderr, __VA_ARGS__)

/*
 * Append src to dest and return dest + sizeof(src)
 */
#define append(dest, src) (memcpy((dest), &(src), sizeof(src)), (dest) + sizeof(src))

struct connection {
    int socket;
    char username[32];
    u32 login_ok1;
    u32 login_ok2;
    /*
     * Buffer reserved for the response of the connection/client.
     * When bytes are sent, we have to keep in mind that
     * sometimes the entire response can't be sent as once, 
     * instead, in chunks. If that's the case, "sent" records
     * how many bytes of the entire response ("to_send_count")
     * has been already sent.
     */
    byte to_send[1 kb];
    size_t to_send_count;
    size_t sent;
    /*
     * Buffer reserved for the request of client.
     */
    byte request[1 kb];
    size_t request_count;
    
    BF_KEY blowfish;
    BIGNUM *rsa_e;
    RSA *rsa_key;
};

static struct connection connections[32] = {0};

static u32 ip_to_u32(char *src)
{
    if (!src)
        return 0;

    u32 ip[4] = {0};
    sscanf(src, "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]);
    u32 result = ip[0] | ip[1] << 8 | ip[2] << 16 | ip[3] << 24;

    return result;
}

static struct connection *find_connection(int socket)
{
    /*
     * Check for a connection already using this socket.
     */
    for (size_t i = 0; i < countof(connections); i++) {
        if (connections[i].socket == socket)
            return connections + i;
    }
    /*
     * Or try to give a new usable connection.
     */
    for (size_t i = 0; i < countof(connections); i++) {
        /*
         * Skip if the connection is being used.
         */
        if (connections[i].socket)
            continue;

        connections[i] = (struct connection) {0};
        return connections + i;
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
    
    /*
     * The packet must be multiple of 8
     */
    u16 body_padded_size = ((size + 7) & (~7));
    u16 size_header = 2;
    /*
     * The final size of the packet consists of 
     * the padded size plus 2 bytes used to store how
     * big the packet is.
     */
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
    for (u16 i = 0; i < size; i += 8) {
        union {
            u32 ints[2];
            byte raw[8];
        } chunk = {0};

        chunk.ints[0] = htonl(*(u32 *) (start + i));
        chunk.ints[1] = htonl(*(u32 *) (start + i + 4));

        BF_ecb_encrypt(chunk.raw, chunk.raw, &conn->blowfish, BF_ENCRYPT);

        chunk.ints[0] = ntohl(chunk.ints[0]);
        chunk.ints[1] = ntohl(chunk.ints[1]);

        *(u32 *) (start + i) = chunk.ints[0];
        *(u32 *) (start + i + 4) = chunk.ints[1];
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
        {0xfd, 0x8a, 0x22, 0x00}, /* unknown? */
        {0x5a, 0x78, 0x00, 0x00}, /* c4 protocol */
        {0},
    };
    
    const BIGNUM *n = 0;
    RSA_get0_key(conn->rsa_key, &n, 0, 0);
    BN_bn2bin(n, init.modulus);
    
    /*
     * scramble modulus
     * credits: l2j
     */
    {
        byte *modulus = init.modulus;

        for (int i = 0; i < 4; i++) {
            byte temp = modulus[i];
            modulus[i] = modulus[0x4d + i];
            modulus[0x4d + i] = temp;
        };
        /*
         * step 2 xor first 0x40 bytes with last 0x40 bytes
         */
        for (int i = 0; i < 0x40; i++)
            modulus[i] = (byte) (modulus[i] ^ modulus[0x40 + i]);
        /*
         * step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38
         */
        for (int i = 0; i < 4; i++)
            modulus[0x0d + i] = (byte) (modulus[0x0d + i] ^ modulus[0x34 + i]);
        /*
         * step 4 xor last 0x40 bytes with first 0x40 bytes
         */
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
    trace("sending ignore gg packet" nl);
}

static void handle_auth_request(struct connection *conn, byte *request)
{
    assert(conn);
    assert(request);
    
    byte *content = request + 1;
    char *username = (char *) content + 0x62;
    char *password = (char *) content + 0x70;
    
    int account_exists = 0;
    int authenticated = 0;
    
    strncpy(conn->username, username, sizeof(conn->username) - 1);
    trace("user %s is trying to authenticate" nl, conn->username);
    
    directory_create("data");
    directory_create("data/accounts");
    
    in_directory("data/accounts") {
        if (strcmp(directory.name, conn->username) != 0)
            continue;
        
        account_exists = 1;
        char hash_password_path[1024] = {0};
        snprintf(
            hash_password_path,
            sizeof(hash_password_path) - 1,
            "%s/hash_password.txt",
            directory.full_path
        );

        byte stored_salt[16] = {0};
        byte stored_hash[32] = {0};

        FILE *hash_password = fopen(hash_password_path, "r");
        
        if (!hash_password)
            trace("ERROR: unable to read %s to check user's password" nl, hash_password_path);

        fread(stored_salt, 1, sizeof(stored_salt), hash_password);
        fread(stored_hash, 1, sizeof(stored_hash), hash_password);
        fclose(hash_password);
        
        byte hash_from_request[32] = {0};
        PKCS5_PBKDF2_HMAC(
            password, 
            (int) strnlen(password, 32), 
            stored_salt, 
            sizeof(stored_salt), 
            1000, 
            EVP_sha256(), 
            sizeof(hash_from_request), 
            hash_from_request
        );

        /*
         * Check if passwords match.
         */
        authenticated = 
            hash_password &&
            memcmp(stored_hash, hash_from_request, sizeof(stored_hash)) == 0;
        
        break;
    }
    
    if (!account_exists) {
        trace("the user %s doesn't exist. trying to create the account" nl, conn->username);
        
        char account_folder[256] = {0};
        snprintf(
            account_folder, 
            sizeof(account_folder) - 1, 
            "data/accounts/%s", 
            conn->username
        );
        directory_create(account_folder);
        
        char hash_password_path[256] = {0};
        snprintf(
            hash_password_path, 
            sizeof(hash_password_path) - 1, 
            "data/accounts/%s/hash_password.txt", 
            conn->username
        );
        
        FILE *hash_password = fopen(hash_password_path, "w");
        if (!hash_password)
            trace(
                "ERROR: unable to create or write to file %s." nl
                "check your permissions. the connection will be dropped." nl,
                hash_password_path
            );

        unsigned char salt[16] = {0};
        RAND_bytes(salt, sizeof(salt));

        unsigned char hash[32] = {0};
        PKCS5_PBKDF2_HMAC(
            password, 
            (int) strnlen(password, 32), 
            salt, 
            sizeof(salt), 
            1000, 
            EVP_sha256(), 
            sizeof(hash), 
            hash
        );

        fwrite(salt, 1, sizeof(salt), hash_password);
        fwrite(hash, 1, sizeof(hash), hash_password);
        fclose(hash_password);

        authenticated = hash_password != 0;

        if (authenticated)
            trace("account %s created successfully" nl, conn->username);
    }
    
    /*
     * TODO(fmontenegro): don't drop it, there is a correct packet
     * to send before dropping the connection (invalid password)
     */
    if (!authenticated) {
        trace("unable to authenticate %s, dropping connection" nl, conn->username);
        net_close(conn->socket);
        conn->socket = 0;
        return;
    }
    
    byte *start = conn->to_send + sizeof(u16);
    byte *end = start;
    
    /*
     * Assume success login.
     */
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
    
    directory_create("data");
    
    FILE *servers_file = fopen("data/servers.txt", "r");
    if (servers_file) {
        while (server_count < (u8) countof(servers)) {
            int id = 0;
            int max_players = 0;
            int status = 0;
            int matched = fscanf(
                servers_file, 
                "id=%d" nl
                "ip=%s" nl
                "port=%d" nl
                "max_players=%d" nl
                "status=%d" nl,
                &id,
                servers[server_count].text_ip,
                &servers[server_count].port,
                &max_players,
                &status
            );
            
            servers[server_count].id = (u8) id;
            servers[server_count].max_players = (u16) max_players;
            servers[server_count].status = (u8) status;

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
            trace("WARNING: i couldn't find any server in data/servers.txt." nl
                  "just in case, this is the format i'm expecting:" nl
                  nl
                  "id=1" nl
                  "ip=127.0.0.1" nl
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
        
        trace("WARNING! no data/servers.txt file found, i will try to create a default one" nl);
        
        FILE *default_servers_file = fopen("data/servers.txt", "w+");
        if (default_servers_file) {
            fprintf(
                default_servers_file, 
                "id=%d" nl
                "ip=0.0.0.0" nl
                "port=%d" nl
                "max_players=%d" nl
                "status=%d" nl,
                servers[0].id,
                servers[0].port,
                servers[0].max_players,
                servers[0].status
            );
            fclose(default_servers_file);
        } else {
            trace(
                "i was unable to create the default file." nl
                "please check the folder's permission or, " nl
                "create a data/servers.txt file manually with:" nl
                nl
                "id=%d" nl
                "ip=0.0.0.0" nl
                "port=%d" nl
                "max_players=%d" nl
                "status=%d" nl
                nl
                "in the meantime, i'll send the player the default server" nl
                "i would if you had the default data/servers.txt file i just" nl
                "showed you" nl,
                servers[0].id,
                servers[0].port,
                servers[0].max_players,
                servers[0].status
            );
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
    
    char access_path[256] = {0};
    snprintf(
        access_path, 
        sizeof(access_path) - 1, 
        "data/accounts/%s/access.txt", 
        conn->username
    );
    
    FILE *access_file = fopen(access_path, "w");
    if (!access_file) {
        trace(
            "unable to create the file %s." nl
            "this file is used to check if a connection to a game server" nl
            "actually went through the login server successfully." nl
            "the connection with %s will be dropped" nl,
            access_path,
            conn->username
        );
        net_close(conn->socket);
        conn->socket = 0;
        return;
    }
    time_t now = time(0);
    time_t created_at = now;
    /*
     * Make these ids valid for 1 minute.
     */
    time_t valid_until = now + 1 minute;
    struct tm created_at_tm = *gmtime(&created_at);
    struct tm valid_until_tm = *gmtime(&valid_until);
    char created_at_str[128] = {0};
    char valid_until_str[128] = {0};
    strftime(
        created_at_str, 
        sizeof(created_at_str) - 1, 
        "%Y-%m-%d %H:%M:%S", 
        &created_at_tm
    );
    strftime(
        valid_until_str, 
        sizeof(valid_until_str) - 1, 
        "%Y-%m-%d %H:%M:%S", 
        &valid_until_tm
    );
    /*
     * Save these ids so later the game server can check that the user
     * went through the login server successfully.
     * Save dates in utc format, in number (easier to check) and 
     * formatted, easier to read & debug :)
     */
    fprintf(
        access_file, 
        "login_ok1=%u" nl
        "login_ok2=%u" nl
        "created_at=%u" nl
        "valid_until=%u" nl
        "created_at(yyyy-mm-dd hh:mm:ss utc)=%s" nl
        "valid_until(yyyy-mm-dd hh:mm:ss utc)=%s" nl,
        conn->login_ok1,
        conn->login_ok2,
        (unsigned int) created_at,
        (unsigned int) valid_until,
        created_at_str,
        valid_until_str
    );
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
    
    trace("the user %s entered the game server successfully" nl, conn->username);
}

static void on_request(struct connection *conn)
{
    assert(conn);
    
    byte *request = conn->request;
    
    u16 size = 0;
    memcpy(&size, request, sizeof(size));
    
    /*
     * Check for incomplete packet.
     */
    if (size > conn->request_count)
        return;
    
    trace("new packet of size %d (mod 8 = %d)" nl, (int) size, size % 8);
    
    u16 body_size = size - (u16) sizeof(size);
    request += sizeof(size);
    trace("packet body size %d (mod 8 = %d)" nl, (int) body_size, body_size % 8);
    
    /*
     * Blowfish decrypt.
     */
    for (u16 i = 0; i < body_size; i += 8) {
        union {
            u32 ints[2];
            byte raw[8];
        } chunk = {0};
        chunk.ints[0] = htonl(*(u32 *) (request + i));
        chunk.ints[1] = htonl(*(u32 *) (request + i + 4));
        BF_ecb_encrypt(chunk.raw, chunk.raw, &conn->blowfish, BF_DECRYPT);
        chunk.ints[0] = ntohl(chunk.ints[0]);
        chunk.ints[1] = ntohl(chunk.ints[1]);
        *(u32 *) (request + i) = chunk.ints[0];
        *(u32 *) (request + i + 4) = chunk.ints[1];
    }
    
    /*
     * RSA decrypt.
     * +1 don't include the packet type, just the body of the packet.
     */
    RSA_private_decrypt(RSA_size(conn->rsa_key),
                        request + 1,
                        request + 1,
                        conn->rsa_key,
                        RSA_NO_PADDING);
    
    byte type = 0;
    memcpy(&type, request, sizeof(type));
    trace("received packet is of type %d" nl, (int) type);
    
    /*
     * Requests are in order of how they should happen in a normal connection, 
     * meaning, first 0x07, then 0x00, and so on...
     */
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
    
    memmove(conn->request, conn->request + size, conn->request_count - size);
    conn->request_count -= size;
}

static void handle_event(int socket, enum net_event event, void *read, unsigned long long len)
{
    struct connection *conn = find_connection(socket);
    switch (event) {
        case net_conn: {
            if (!conn) {
                trace("no more room. can't accept new connection (will be dropped)" nl);
                net_close(socket);
                return;
            }

            conn->socket = socket;
            
            /*
             * Blowfish key.
             */
            unsigned char key[] = "_;5.]94-31==-%xT!^[$";
            BF_set_key(&conn->blowfish, (int) (sizeof(key)), key);
            
            /*
             * Generate a new rsa key if required.
             */
            if (!conn->rsa_key) {
                conn->rsa_key = RSA_new();
                BN_dec2bn(&conn->rsa_e, "65537");
                RSA_generate_key_ex(conn->rsa_key, 1024, conn->rsa_e, 0);
            }
            
            send_init_packet(conn);
        } break;
        
        case net_closed: {
            conn->socket = 0;
            trace("client closed the connection" nl);
        } break;
        
        case net_read: {
            trace("bytes %d received from client" nl, (s32) len);
            memcpy(conn->request + conn->request_count, read, len);
            conn->request_count += len;
            on_request(conn);
        } break;
        
        default:
        break;
    }
    
    /*
     * Reply with data to the client.
     */
    void *head = conn->to_send + conn->sent;
    unsigned long long to_send = conn->to_send_count - conn->sent;
    trace("sending %d bytes of data" nl, (u32) to_send);
    conn->sent += net_send(conn->socket, head, to_send);

    /*
     * All data has been sent, reset the counters.
     */
    if (conn->sent >= conn->to_send_count) {
        conn->sent = 0;
        conn->to_send_count = 0;
    }
}

int main()
{
    unsigned short port = 2106;
    int socket = net_port(port);
    trace("login server, listening for connection on port %d" nl, port);
    net_listen(socket, handle_event);

    return 0;
}
