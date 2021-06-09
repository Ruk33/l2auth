CC	= gcc
CFLAGS	= -std=c99 -Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -O2 -g -pedantic
LDFLAGS	= -shared
CLIBS	= -lcrypto -lsqlite3 -ldl
OS	= linux
STORAGE	= sqlite

# Files used by both, login server and game server.
HEADERS		= src/os_socket.h src/include/config.h src/include/util.h src/include/conn.h src/include/log.h src/include/packet.h src/include/l2_string.h
SRC		= src/config.c src/util.c src/conn.c src/packet.c src/l2_string.c
OBJ		= $(SRC:.c=.o)

# Operating system related filed.
OS_HEADERS	= $(wildcard src/include/os_*.h)
OS_SRC		= $(wildcard src/$(OS)/*.c)
OS_OBJ		= $(OS_SRC:.c=.o)

# Storage/database files.
STG_HEADERS	= src/include/storage.h
STG_SRC		= src/storage/$(STORAGE).c
STG_OBJ		= $(STG_SRC:.c=.o)

# Login server files.
LS_HEADERS	= $(wildcard src/include/ls_*.h) src/include/server.h
LS_SRC		= $(wildcard src/ls_*.c) src/server.c
LS_OBJ		= $(LS_SRC:.c=.o)

# Game server files.
GS_HEADERS	= $(wildcard src/include/gs_*.h)
GS_SRC		= $(wildcard src/gs_*.c)
GS_OBJ		= $(GS_SRC:.c=.o)

.PHONY	: clean

# Note: Do NOT include header files as dependencies
# for game_server_lib.so target. For some reason, it
# makes the game_server executable seg fault when the
# library gets loaded.
game_server_lib.so : CFLAGS += -fPIC
game_server_lib.so : $(OBJ) $(STG_OBJ) $(GS_OBJ)
	@echo "⛏️ Building game server library..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(CLIBS)

game_server : game_server_lib.so $(OBJ) $(OS_OBJ) src/$(OS)/game_server/main.c
	@echo "⛏️ Building game server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

login_server : $(OBJ) $(OS_OBJ) $(LS_OBJ) src/$(OS)/login_server/main.c
	@echo "⛏️ Building login server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean :
	@echo "🗑️ Removing .o files, executables and library"
	@$(RM) $(OBJ) $(OS_OBJ) $(STG_OBJ) $(LS_OBJ) $(GS_OBJ) game_server login_server game_server_lib.so
