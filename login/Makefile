C_FILES=\
	client_request/auth_login.c \
	client_request/gg_auth.c \
	client_request/handle.c \
	client_request/login_server.c \
	client_request/server_list.c \
	handler/disconnect.c \
	handler/new_connection.c \
	handler/new_request.c \
	server_packet/gg_auth.c \
	server_packet/init.c \
	server_packet/ok.c \
	server_packet/play_ok.c \
	server_packet/server_list.c \
	storage/game_server_manager.c \
	storage/server_manager.c \
	storage/session_manager.c \
	blowfish.c \
	decrypt_packet.c \
	encrypt_packet.c \
	main.c \
	packet.c \
	rsa.c \
	server.c \
	socket.c

H_FILES=\
	$(C_FILES:.c=.h) \
	client_packet/type.h \
	storage/game_server.h \
	storage/session.h \
	byte_write.h

O_FILES=$(C_FILES:.c=.o)

CC=gcc
CFLAGS=-std=c99 -Wall -Werror -Wextra -g -pedantic
CLIBS=-lssl -lcrypto -ldatastructures

%.o: %.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

login_server: $(O_FILES)
	$(CC) -o $@ $^ $(CFLAGS) $(CLIBS)

.PHONY: clean
clean:
	rm -f $(O_FILES)
