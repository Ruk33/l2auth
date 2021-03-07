CC	= gcc
CFLAGS	= -std=c99 -Wall -Werror -Wextra -g -pedantic
CLIBS	= -ldl -lpthread -ldatastructures
SOURCES	= $(wildcard *.c)
OBJS	= $(filter-out main.o, $(SOURCES:.c=.o))

%.o : %.c %.h
	@echo "🔧 File $< needs to be rebuilt"
	-$(CC) $(CFLAGS) -c $< -o $@
	@echo ""

game_server : $(OBJS) main.c
	@echo "⛏️ Building game server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

.PHONY: clean
clean:
	@echo "🗑️ Removing .o files"
	@$(RM) *.o
