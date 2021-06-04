CC	= gcc
CFLAGS	= -std=c99 -Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -g -pedantic -fPIC
CLIBS	= -lcrypto -lsqlite3 -ldl
OS	= linux
STORAGE	= sqlite
HEADERS = $(wildcard src/include/*.h)
SOURCES	= $(wildcard src/$(OS)/*.c) src/storage/$(STORAGE).c $(wildcard src/*.c)
OBJS	= $(SOURCES:.c=.o)

.PHONY	: clean

game_server_lib.so : $(OBJS) $(HEADERS)
	@echo "⛏️ Building game server library..."
	$(CC) $(CFLAGS) -o $@ $^ -shared $(CLIBS)

login_server : $(OBJS) $(HEADERS) src/$(OS)/login_server/main.c
	@echo "⛏️ Building login server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

game_server : game_server_lib.so src/$(OS)/socket.c src/util.c src/$(OS)/game_server/main.c
	@echo "⛏️ Building game server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean :
	@echo "🗑️ Removing .o files"
	@$(RM) $(OBJS) game_server login_server game_server_lib.so
