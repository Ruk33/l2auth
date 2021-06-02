CC	= gcc
CFLAGS	= -std=c99 -Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -g -pedantic
CLIBS	= -lcrypto -lsqlite3
OS	= linux
STORAGE	= sqlite
SOURCES	= $(wildcard src/$(OS)/*.c) src/storage/$(STORAGE).c $(wildcard src/*.c)
OBJS	= $(SOURCES:.c=.o)

.PHONY	: clean

login_server : $(OBJS) src/$(OS)/login_server/main.c
	@echo "⛏️ Building login server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

game_server : $(OBJS) src/$(OS)/game_server/main.c
	@echo "⛏️ Building game server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

# lib : ...

clean :
	@echo "🗑️ Removing .o files"
	@$(RM) $(OBJS)
