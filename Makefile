CC	= gcc
CFLAGS	= -std=c99 -Wall -Werror -Wextra -g -pedantic
CLIBS	= 
OS	= linux
SOURCES	= $(wildcard src/$(OS)/*.c) $(wildcard src/*.c)
OBJS	= $(SOURCES:.c=.o)

.PHONY	: clean

src/$(OS)/%.o : src/$(OS)/%.c
	@echo "🔧 OS file $< needs to be rebuilt"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo ""

src/%.o : src/%.c src/include/%.h
	@echo "🔧 File $< needs to be rebuilt"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo ""

login_server : $(OBJS) src/linux/login_server/main.c
	@echo "⛏️ Building login server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS) -lcrypto

game_server : $(OBJS) src/linux/game_server/main.c
	@echo "⛏️ Building game server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

# lib : ...

clean :
	@echo "🗑️ Removing .o files"
	@$(RM) $(OBJS)
