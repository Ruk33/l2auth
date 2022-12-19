// this is just a quick storage method intended
// to be easy to use and for development only.
// the way it works is, write the bytes of a
// to-be-saved struct in a file. in order to 
// retrieve data, just read those bytes back
// and cast it back to the struct.
// note: this particular implementation is
// made for linux only but should be fairly easy
// to adapt for windows (if that ever happens)

#include <errno.h>      // errno
#include <string.h>     // strerror, memcpy
#include <fcntl.h>      // open, O_CREAT, O_RDWR
#include <unistd.h>     // write
#include <sys/stat.h>   // fstat
#include <sys/mman.h>   // mmap
#include "include/l2auth.h"

#define max_accounts (128)

struct storage {
    struct account accounts[max_accounts];
    size_t accounts_count;
    struct server servers[8];
    size_t servers_count;
};

struct conn {
    struct storage *storage;
    int fd;
    struct stat file_stat;
};

static size_t init_empty_storage(int fd)
{
    static struct storage empty_storage = {0};
    size_t result = write(fd, &empty_storage, sizeof(empty_storage));
    return result;
}

static int open_conn(struct conn *dest)
{
    assert(dest);
    dest->fd = open("linux_storage", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (dest->fd == -1) {
        log("unable to open linux storage file.");
        goto abort;
    }

    if (fstat(dest->fd, &dest->file_stat) == -1) {
        log("unable to fstat linux storage file.");
        goto abort;
    }

    if ((size_t) dest->file_stat.st_size != sizeof(struct storage))
        dest->file_stat.st_size = init_empty_storage(dest->fd);

    dest->storage = mmap(
        0,
        dest->file_stat.st_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        dest->fd,
        0
    );
    if (dest->storage == MAP_FAILED) {
        log("failed to mmap linux storage file.");
        log("error: %s.", strerror(errno));
        goto abort;
    }
    return 1;

abort:
    close(dest->fd);
    zero(dest);
    return 0;
}

static void close_conn(struct conn *src)
{
    assert(src);
    if (!src->storage)
        return;
    close(src->fd);
    munmap(src->storage, src->file_stat.st_size);
}

int storage_get_account(struct account *dest, struct username *username)
{
    assert(username);
    int result = 0;

    struct conn conn = {0};
    open_conn(&conn);

    for_each(struct account, account, conn.storage->accounts) {
        if (!str_matches(account->username.buf, username->buf))
            continue;
        if (dest)
            memcpy(dest, account, sizeof(*dest));
        result = 1;
        break;
    }

    close_conn(&conn);
    return result;
}

int storage_create_account(struct username *username, struct password *password)
{
    assert(username);
    assert(password);

    int result = 0;
    
    struct conn conn = {0};
    open_conn(&conn);

    if (storage_get_account(0, username)) {
        log("account with username '%s' already exists!", username->buf);
        goto close;
    }

    if (conn.storage->accounts_count + 1 >= max_accounts) {
        log("you have reached the maximum amount of accounts.");
        goto close;
    }

    struct account new_account = {0};
    new_account.username = *username;
    log("todo: remember to encrypt the password!");
    str_cpy(new_account.password.buf, password->buf);

    conn.storage->accounts[conn.storage->accounts_count] = new_account;
    conn.storage->accounts_count++;
    result = 1;

close:
    close_conn(&conn);
    return result;
}

int storage_get_servers(struct server *dest, u8 *found, size_t max)
{
    assert(dest);
    assert(found);
    assert(max > 0);
    assert(max <= UINT8_MAX);

    struct conn conn = {0};
    int result = open_conn(&conn);

    *found = 0;
    for_each(struct server, server, conn.storage->servers) {
        if (!server->id)
            break;
        dest[*found] = *server;
        *found = *found + 1;
        if (max <= *found)
            break;
    }

    close_conn(&conn);
    return result;
}

int storage_create_server(struct server *src)
{
    assert(src);
    struct conn conn = {0};
    int result = open_conn(&conn);

    // make sure we don't save the same server id twice.
    for_each(struct server, server, conn.storage->servers) {
        if (server->id != src->id)
            continue;
        log("server with id %d already exists. it will not be created again.", server->id);
        result = 0;
        goto close;
    }

    conn.storage->servers[conn.storage->servers_count] = *src;
    conn.storage->servers_count++;

close:
    close_conn(&conn);
    return result;
}

int storage_create_character(struct username *username, struct l2_character *src)
{
    assert(username);
    assert(src);
    struct conn conn = {0};
    int result = open_conn(&conn);

    for_each(struct account, account, conn.storage->accounts) {
        if (!str_matches(account->username.buf, username->buf))
            continue;
        // the account should always exists, so there is no need to 
        // perform extra checks.
        account->characters[account->characters_count] = *src;
        account->characters_count++;
        break;
    }

    close_conn(&conn);
    return result;
}

int storage_get_characters(struct l2_character *dest, struct username *username, int max, int *found)
{
    assert(dest);
    assert(username);
    assert(found);
    struct conn conn = {0};
    int result = open_conn(&conn);

    *found = 0;
    for_each(struct account, account, conn.storage->accounts) {
        if (!str_matches(account->username.buf, username->buf))
            continue;
        // don't copy more than what's required.
        max = min(max, account->characters_count);
        for (int i = 0; i < max; i++) {
            dest[*found] = account->characters[i];
            *found = *found + 1;
        }
        break;
    }

    close_conn(&conn);
    return result;
}

int storage_get_character(struct l2_character *dest, struct username *username, int index)
{
    assert(dest);
    assert(username);
    struct conn conn = {0};
    int result = open_conn(&conn);

    for_each(struct account, account, conn.storage->accounts) {
        if (!str_matches(account->username.buf, username->buf))
            continue;
        if (index >= 0 && index < account->characters_count)
            *dest = account->characters[index];
        break;
    }

    close_conn(&conn);
    return result;
}
