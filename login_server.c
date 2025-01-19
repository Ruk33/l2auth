#define WIN32_LEAN_AND_MEAN

#include <stddef.h>
#include <time.h>

#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/blowfish.h>
#include <openssl/evp.h>

#include "utils.h"
#include "directory.h"
#include "net.h"

u32 htonl(u32 x);
u32 ntohl(u32 x);

struct connection {
    int socket;
    char username[16];
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
    byte to_send[512];
    u64 to_send_count;
    u64 sent;
    /*
     * Buffer reserved for the request of client.
     */
    byte request[512];
    u64 request_count;
    
    BF_KEY blowfish;
    BIGNUM *rsa_e;
    RSA *rsa_key;
};

static struct connection connections[32];

u32 ip_to_u32(char *src)
{
    check(src);

    u32 ip[4] = {0};
    sscanf(src, "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]);
    u32 result = ip[0] | ip[1] << 8 | ip[2] << 16 | ip[3] << 24;

    return result;
}

struct connection *find_connection(int socket)
{
    /*
     * Check for a connection already using this socket.
     */
    for (u64 i = 0; i < array_length(connections); i++) {
        if (connections[i].socket == socket)
            return connections + i;
    }
    /*
     * Or try to give a new usable connection.
     */
    for (u64 i = 0; i < array_length(connections); i++) {
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

u16 checksum(byte *dest, byte *start, byte *end)
{
    check(dest);
    check(start);
    check(end);
    check(start < end);
    check(end - start < 65535);
    
    u16 size = (u16) (end - start);
    u32 result = 0;
    for (u16 i = 0; i < size; i += 4) {
        u32 ecx = *start++ & 0xff;
        ecx |= (*start++ <<  0x8) & 0xff00;
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
    copy_memory(dest, &final_size, sizeof(final_size));
    
    return final_size;
}

void encrypt_packet(struct connection *conn, byte *start, byte *end)
{
    check(conn);
    check(start);
    check(end);
    check(start < end);
    check(end - start < 65535);

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

void push_init_packet(struct connection *conn)
{
    check(conn);
    
    struct {
        byte session_id[4];
        byte protocol[4];
        byte modulus[128];
    } init = {
        {0xfd, 0x8a, 0x22, 0x00}, // unknown?
        {0x5a, 0x78, 0x00, 0x00}, // c4 protocol
        {0},
    };
    
    const BIGNUM *n = 0;
    RSA_get0_key(conn->rsa_key, &n, 0, 0);
    BN_bn2bin(n, init.modulus);
    
    // scramble modulus
    // credits: l2j
    {
        byte *modulus = init.modulus;

        for (int i = 0; i < 4; i++) {
            byte temp = modulus[i];
            modulus[i] = modulus[0x4d + i];
            modulus[0x4d + i] = temp;
        }

        for (int i = 0; i < 0x40; i++)
            modulus[i] = (byte) (modulus[i] ^ modulus[0x40 + i]);

        for (int i = 0; i < 4; i++)
            modulus[0x0d + i] = (byte) (modulus[0x0d + i] ^ modulus[0x34 + i]);

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

void push_ignore_gg_packet(struct connection *conn)
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

void handle_auth_request(struct connection *conn, byte *request)
{
    assert(conn);
    assert(request);
    
    byte *content = request + 1;
    char *username = (char *) content + 0x62;
    char *password = (char *) content + 0x70;
    
    int account_exists = 0;
    int authenticated = 0;
    
    copy_string(conn->username, username, sizeof(conn->username) - 1);
    trace("user %s is trying to authenticate" nl, conn->username);
    
    directory_create("data");
    directory_create("data/accounts");
    
    in_directory("data/accounts") {
        if (!same_string(directory.name, conn->username))
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
            same_memory(stored_hash, hash_from_request, sizeof(stored_hash));
        
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
            trace("ERROR: unable to create or write to file %s." nl
                  "check your permissions. the connection will be dropped." nl,
                  hash_password_path);

        byte salt[16] = {0};
        RAND_bytes(salt, sizeof(salt));
        
        byte hash[32] = {0};
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
    
    RAND_bytes((byte *) &conn->login_ok1, sizeof(conn->login_ok1));
    end = append(end, conn->login_ok1);
    
    RAND_bytes((byte *) &conn->login_ok2, sizeof(conn->login_ok2));
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

void handle_server_list_request(struct connection *conn)
{
    check(conn);
    
    struct server {
        u32 ip;
        u32 port;
        u32 extra;

        u16 players;
        u16 max_players;

        u8 id;
        u8 age_limit;
        u8 pvp;
        u8 status;
        u8 brackets;
    } servers[8] = {0};

    u8 server_count = 0;
    
    trace("%s requested the servers list" nl, conn->username);
    
    directory_create("data");
    
    FILE *servers_file = fopen("data/servers.txt", "r");

    /*
     * Try to create a default servers config file if we can't open the file.
     */
    if (!servers_file) {
        trace("no data/servers.txt file found, will try to create a new one with a default server." nl);

        FILE *default_servers_file = fopen("data/servers.txt", "w+");

        if (default_servers_file) {
            write_config(default_servers_file, "id", "%d", 1);
            write_config(default_servers_file, "ip", "%s", "0.0.0.0");
            write_config(default_servers_file, "port", "%d", 7777);
            write_config(default_servers_file, "max_players", "%d", 1000);
            write_config(default_servers_file, "status", "%d", 1);

            fclose(default_servers_file);

            /*
             * Once we have written the default configuration, try to re-open
             * the file to be parsed later.
             */
            servers_file = fopen("data/servers.txt", "r");
        } else {
            trace("i was not able to create the data/servers.txt file (check permissions of perhaps you are out of space)" nl);
        }
    }

    if (servers_file) {
        while (server_count < array_length(servers)) {
            char formatted_ip[sizeof("255.255.255.255")] = {0};

            struct server server = {0};

            int parsed_correctly =
                read_config(servers_file, "id", "%d", &server.id) &&
                read_config(servers_file, "ip", "%s", formatted_ip) &&
                read_config(servers_file, "port", "%d", &server.port) &&
                read_config(servers_file, "max_players", "%d", &server.max_players) &&
                read_config(servers_file, "status", "%d", &server.status);

            if (!parsed_correctly)
                break;

            server.id = 1;
            server.ip = ip_to_u32(formatted_ip);
            server.age_limit = 18;
            server.pvp = 1;

            servers[server_count] = server;
            server_count++;
        }

        fclose(servers_file);
    }

    trace("%d servers found and will be sent to the client." nl, server_count);

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

void handle_enter_game_server(struct connection *conn)
{
    check(conn);
    
    char access_path[256] = {0};
    snprintf(access_path, 
             sizeof(access_path) - 1, 
             "data/accounts/%s/access.txt", 
             conn->username);
    
    FILE *access_file = fopen(access_path, "w");
    if (!access_file) {
        trace("unable to create the file %s." nl
              "this file is used to check if a connection to a game server" nl
              "actually went through the login server successfully." nl
              "the connection with %s will be dropped" nl,
              access_path,
              conn->username);
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
    strftime(created_at_str, 
             sizeof(created_at_str) - 1, 
             "%Y-%m-%d %H:%M:%S", 
             &created_at_tm);
    strftime(valid_until_str, 
             sizeof(valid_until_str) - 1, 
             "%Y-%m-%d %H:%M:%S", 
             &valid_until_tm);
    /*
     * Save these ids so later the game server can check that the user
     * went through the login server successfully.
     * Save dates in utc format, in number (easier to check) and 
     * formatted, easier to read & debug :)
     */
    fprintf(access_file, 
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
    
    trace("the user %s entered the game server successfully" nl, conn->username);
}

void on_request(struct connection *conn)
{
    check(conn);
    
    byte *request = conn->request;
    
    u16 size = 0;
    copy_memory(&size, request, sizeof(size));
    
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
    copy_memory(&type, request, sizeof(type));
    trace("received packet is of type %d" nl, (int) type);
    
    /*
     * Requests are in order of how they should happen in a normal connection, 
     * meaning, first 0x07, then 0x00, and so on...
     */
    switch (type) {
        case 0x07:
        push_ignore_gg_packet(conn);
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

void send_queued_packets(struct connection *conn)
{
    check(conn);

    byte *head = conn->to_send + conn->sent;
    u64 to_send = conn->to_send_count - conn->sent;

    if (!to_send)
        return;

    trace("sending %d bytes of data" nl, (u32) to_send);
    conn->sent += net_send(conn->socket, head, to_send);

    if (conn->sent >= conn->to_send_count) {
        conn->sent = 0;
        conn->to_send_count = 0;
    }
}

void handle_event(int socket, enum net_event event, void *read, unsigned long long len)
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
            byte key[] = "_;5.]94-31==-%xT!^[$";
            BF_set_key(&conn->blowfish, sizeof(key), key);
            
            /*
             * Generate a new rsa key if required.
             */
            if (!conn->rsa_key) {
                conn->rsa_key = RSA_new();
                BN_dec2bn(&conn->rsa_e, "65537");
                RSA_generate_key_ex(conn->rsa_key, 1024, conn->rsa_e, 0);
            }
            
            push_init_packet(conn);
        } break;
        
        case net_closed: {
            conn->socket = 0;
            trace("client closed the connection" nl);
        } break;
        
        case net_read: {
            trace("bytes %d received from client" nl, (s32) len);
            copy_memory(conn->request + conn->request_count, read, len);
            conn->request_count += len;
            on_request(conn);
        } break;
        
        default:
        break;
    }

    send_queued_packets(conn);
}

int main()
{
    u16 port = 2106;
    int socket = net_port(port);
    trace("login server, listening for connection on port %d" nl, port);
    net_listen(socket, handle_event);

    return 0;
}
