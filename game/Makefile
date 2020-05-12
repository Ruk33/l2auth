csrc = $(wildcard *.c) \
	$(wildcard action/auth_login/*.c) \
	$(wildcard action/character_info/*.c) \
	$(wildcard action/create_character/*.c) \
	$(wildcard action/d0/*.c) \
	$(wildcard action/enter_world/*.c) \
	$(wildcard action/entry_point/*.c) \
	$(wildcard action/hot_code_reload_entry_point/*.c) \
	$(wildcard action/move/*.c) \
	$(wildcard action/new_character/*.c) \
	$(wildcard action/protocol_version/*.c) \
	$(wildcard action/request_quests/*.c) \
	$(wildcard action/restart/*.c) \
	$(wildcard action/say/*.c) \
	$(wildcard action/select_character/*.c) \
	$(wildcard action/validate_position/*.c) \
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
