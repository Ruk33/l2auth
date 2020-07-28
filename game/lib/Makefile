gameserverlib = $(wildcard ./*.c) \
	$(wildcard cache/memory/*.c) \
	$(wildcard entity/*.c) \
	$(wildcard request_handler/action/*.c) \
	$(wildcard request_handler/auth_login/*.c) \
	$(wildcard request_handler/d0/*.c) \
	$(wildcard request_handler/enter_world/*.c) \
	$(wildcard request_handler/move/*.c) \
	$(wildcard request_handler/npc_info/*.c) \
	$(wildcard request_handler/protocol_version/*.c) \
	$(wildcard request_handler/quest_list/*.c) \
	$(wildcard request_handler/restart/*.c) \
	$(wildcard request_handler/say/*.c) \
	$(wildcard request_handler/select_character/*.c) \
	$(wildcard request_handler/validate_position/*.c) \
	$(wildcard storage/sqlite3/*.c) \
	$(wildcard ../shared/*.c) \
	$(wildcard ../../core/*.c) \
	$(wildcard ../../os/linux/*.c)
objgameserverlib = $(gameserverlib:.c=.o)

CC = gcc
CLIBS = -lsqlite3 -lm -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,build
CFLAGS = $(CLIBS) -fPIC -I../.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

lib: $(objgameserverlib)
	$(CC) -o ../build/libgameserver.so -shared -fPIC $^ $(CLIBS)

.PHONY: clean
clean:
	rm -f $(objgameserverlib) lib
