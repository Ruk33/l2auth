CC = gcc

.PHONY: all ls gs gslib

all: ls gs gslib

migrations:
	sqlite3 build/db < migrations/sqlite/00_create_characters_table.sql

ls: src/linux/login_server/main.c
	$(CC) $^ -lcrypto -o build/login_server

gs: src/linux/game_server/main.c
	$(CC) $^ -ldl -o build/game_server

gslib: src/gs_lib.c
	$(CC) $^ -lm -lcrypto -lsqlite3 -shared -fPIC -o build/game_server_lib.so

clean:
	$(RM) -rf build/*
