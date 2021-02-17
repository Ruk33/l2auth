CC	= gcc
CLIBS	= -ldatastructures -lm
CFLAGS	=			\
	-std=c99		\
	-Wall			\
	-Werror			\
	-Wextra			\
	-g			\
	-pedantic		\
	-Wfloat-equal		\
	-Wshadow		\
	-Wcast-align		\
	-Wswitch-default	\
	-Wswitch-enum		\
	-Wconversion		\
	-Wunreachable-code	\
	-I.

SOURCES	=					\
	$(wildcard client_request/*.c)		\
	$(wildcard server_packet/*.c)		\
	$(wildcard state_machine/character/*.c)	\
	$(wildcard state_machine/*.c)		\
	$(wildcard storage/memory/*.c)		\
	$(wildcard *.c)

OBJS	= $(SOURCES:.c=.o)

all: $(OBJS) game_server.so

$(OBJS) : %.o : %.c %.h
	@echo "🔧 File $< needs to be rebuilt"
	-$(CC) $(CFLAGS) -fPIC -c $< -o $@
	@echo ""

game_server.so : $(OBJS)
	@echo "⛏️ Building game server library..."
	$(CC) $(CFLAGS) -o $@ $^ -shared $(CLIBS)

.PHONY: clean
clean:
	@echo "🗑️ Removing .o files"
	@$(RM) $(OBJS)
