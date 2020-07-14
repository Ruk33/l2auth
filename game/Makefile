main = $(wildcard main.c) \
	$(wildcard code.c) \
	$(wildcard handle_client.c) \
	$(wildcard ../os/*.c) \
	$(wildcard ../os/linux/*.c)
objmain = $(main:.c=.o)

gameserverlib = $(wildcard client.c) \
	$(wildcard server.c) \
	$(wildcard client/*.c) \
	$(wildcard request/action/*.c) \
	$(wildcard request/auth_login/*.c) \
	$(wildcard request/character_info/*.c) \
	$(wildcard request/create_character/*.c) \
	$(wildcard request/d0/*.c) \
	$(wildcard request/enter_world/*.c) \
	$(wildcard request/entry_point/*.c) \
	$(wildcard request/hot_code_reload_entry_point/*.c) \
	$(wildcard request/move/*.c) \
	$(wildcard request/new_character/*.c) \
	$(wildcard request/npc_info/*.c) \
	$(wildcard request/protocol_version/*.c) \
	$(wildcard request/quest_list/*.c) \
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
objgameserverlib = $(gameserverlib:.c=.o)

CC = clang
CLIBS = -lsqlite3 -lm -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,build
CFLAGS = $(CLIBS) -fPIC -I.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

gameserver: $(objmain)
	$(CC) -o build/$@ $^ -ldl -llog -Wl,-rpath,build

lib: $(objgameserverlib)
	$(CC) -o build/libgameserver.so -shared -fPIC $^ $(CLIBS)

.PHONY: clean
clean:
	rm -f $(objmain) gameserver && rm -f $(objgameserverlib) lib
