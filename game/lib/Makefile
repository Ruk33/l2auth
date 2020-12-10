C_FILES=\
	client_request/auth_request.c \
	client_request/create_char.c \
	client_request/d0.c \
	client_request/enter_world.c \
	client_request/handle.c \
	client_request/logout.c \
	client_request/move.c \
	client_request/new_char.c \
	client_request/protocol_version.c \
	client_request/quest_list.c \
	client_request/restart.c \
	client_request/say.c \
	client_request/select_character.c \
	client_request/show_map.c \
	client_request/validate_position.c \
	server_packet/auth_login.c \
	server_packet/create_char.c \
	server_packet/d0.c \
	server_packet/enter_world.c \
	server_packet/logout.c \
	server_packet/move.c \
	server_packet/new_char.c \
	server_packet/protocol_version.c \
	server_packet/quest_list.c \
	server_packet/restart.c \
	server_packet/say.c \
	server_packet/select_character.c \
	server_packet/show_map.c \
	server_packet/validate_position.c \
	storage/character.c \
	storage/server.c \
	storage/session.c \
	character.c \
	decrypt_packet.c \
	encrypt_packet.c \
	game_server.c \
	l2_string.c \
	packet.c \
	session.c

H_FILES=\
	$(C_FILES:.c=.h) \
	client_packet/type.h \
	byte_read.h \
	byte_write.h \
	headers.h \
	host.h \
	types.h \
	vec3.h

O_FILES=$(C_FILES:.c=.o)

CC=gcc
CFLAGS=\
	-std=c99 \
	-Wall \
	-Werror \
	-Wextra \
	-g \
	-pedantic \
	-Wfloat-equal \
	-Wshadow \
	-Wcast-align \
	-Wswitch-default \
	-Wswitch-enum \
	-Wconversion \
	-Wunreachable-code
CLIBS=-ldatastructures

%.o: %.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

game_server: $(O_FILES)
	$(CC) -shared -o $@.so $^ $(CFLAGS) $(CLIBS)

.PHONY: clean
clean:
	rm -f $(O_FILES)
