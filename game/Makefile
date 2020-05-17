csrc = $(wildcard *.c) \
	$(wildcard request/auth_login/*.c) \
	$(wildcard request/character_info/*.c) \
	$(wildcard request/create_character/*.c) \
	$(wildcard request/d0/*.c) \
	$(wildcard request/enter_world/*.c) \
	$(wildcard request/entry_point/*.c) \
	$(wildcard request/hot_code_reload_entry_point/*.c) \
	$(wildcard request/move/*.c) \
	$(wildcard request/new_character/*.c) \
	$(wildcard request/protocol_version/*.c) \
	$(wildcard request/request_quests/*.c) \
	$(wildcard request/restart/*.c) \
	$(wildcard request/say/*.c) \
	$(wildcard request/select_character/*.c) \
	$(wildcard request/validate_position/*.c) \
	$(wildcard entity/*.c) \
	$(wildcard service/character/*.c) \
	$(wildcard service/crypt/*.c) \
	$(wildcard service/crypt/packet/*.c) \
	$(wildcard storage/sqlite3/*.c) \
	$(wildcard ../core/*.c) \
	$(wildcard ../os/*.c) \
	$(wildcard ../os/linux/*.c)
obj = $(csrc:.c=.o)

CC = gcc
CLIBS = -lsqlite3 -lm -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,build
CFLAGS = $(CLIBS) -fPIC -I.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

gameserver: $(obj)
	$(CC) -o build/$@ $^ $(CLIBS)

lib: $(obj)
	$(CC) -o build/libgameserver.so -shared -fPIC $^

.PHONY: clean
clean:
	rm -f $(obj) gameserver
