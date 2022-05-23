#include <assert.h>
#include <stdio.h>
#include <string.h>	
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../../include/util.h"
#include "include/types.h"
#include "include/db.h"

struct account {
	// TODO: check limits for both, account name
	// and characters in lobby.
	char name[16];
	struct character characters[10];
	size_t character_count;
};

struct db {
	struct account accounts[128];
	size_t account_count;
};

struct conn {
	struct db *db;
	int fd;
	struct stat file_stat;
};

static size_t init_empty_db(int fd)
{
	struct db empty_db = { 0 };
	return write(fd, &empty_db, sizeof(empty_db));
}

static int open_conn(struct conn *dest)
{
	assert(dest);
	
	dest->fd = open("unix_db", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (dest->fd == -1) {
		printf("unable to open unix db file.\n");
		goto abort;
	}

	if (fstat(dest->fd, &dest->file_stat) == -1) {
		printf("unable to fstat unix db file.\n");
		goto abort;
	}

	if ((size_t) dest->file_stat.st_size != sizeof(struct db))
		dest->file_stat.st_size = init_empty_db(dest->fd);

	dest->db = mmap(
		0,
		dest->file_stat.st_size,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		dest->fd,
		0
	);
	if (dest->db == MAP_FAILED) {
		printf("failed to mmap db unix file.\n");
		printf("error: %s.\n", strerror(errno));
		goto abort;
	}
	return 1;

abort:
	close(dest->fd);
	*dest = (struct conn) { 0 };
	return 0;
}

static void close_conn(struct conn *src)
{
	assert(src);

	if (!src->db)
		return;

	close(src->fd);
	munmap(src->db, src->file_stat.st_size);
}

size_t db_get_account_characters(struct character *dest, struct username *username, size_t max)
{
	struct conn conn = { 0 };
	size_t found = 0;

	assert(dest);
	assert(username);

	if (!open_conn(&conn))
		return 0;

	for (size_t i = 0; i < conn.db->account_count; i += 1) {
		if (strncmp(username->buf, conn.db->accounts[i].name, sizeof(username->buf)) != 0)
			continue;
		
		found = conn.db->accounts[i].character_count;

		for (size_t n = 0; n < max; n += 1)
			dest[n] = conn.db->accounts[i].characters[n];
	}

	close_conn(&conn);
	return found;
}
