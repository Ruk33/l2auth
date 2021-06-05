CC	= gcc
CFLAGS	= -std=c99 -Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -g -pedantic -fPIC
CLIBS	= -lcrypto -lsqlite3 -ldl
OS	= linux
STORAGE	= sqlite

# Files used by both, login server and game server.
HEADERS		= src/include/config.h src/include/util.h src/include/conn.h src/include/log.h src/include/packet.h src/include/l2_string.h
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

game_server_lib.so : $(OBJ) $(HEADERS) $(STG_OBJ) $(STG_HEADERS) $(GS_OBJ) $(GS_HEADERS)
	@echo "⛏️ Building game server library..."
	$(CC) $(CFLAGS) -o $@ $^ -shared $(CLIBS)

game_server : game_server_lib.so $(OBJ) $(HEADERS) $(OS_OBJ) $(OS_HEADERS) src/$(OS)/game_server/main.c
	@echo "⛏️ Building game server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

login_server : $(OBJ) $(HEADERS) $(OS_OBJ) $(OS_HEADERS) $(LS_OBJ) $(LS_HEADERS) src/$(OS)/login_server/main.c
	@echo "⛏️ Building login server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean :
	@echo "🗑️ Removing .o files"
	@$(RM) $(OBJS) game_server login_server game_server_lib.so
