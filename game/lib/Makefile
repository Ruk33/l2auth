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

# == Storage strategy
# Change here if you want to use
# another storage strategy.
# For the time being only "in memory"
# is implemented, meaning, all characters
# and all information related to the
# server gets stored in memory and will
# persist as long as the server is running.
# Adding a new driver should be as easy as
# copying the "memory" folder and adapting
# it to the desired driver (ie mysql, psql, etc.)
STORAGE	= memory

SGE_SRC	=					\
	$(wildcard storage/$(STORAGE)/*.c)

SOURCES	=					\
	$(wildcard client_request/*.c)		\
	$(wildcard server_packet/*.c)		\
	$(wildcard state_machine/character/*.c)	\
	$(wildcard state_machine/*.c)		\
	$(wildcard *.c)

SGE_OBJS= $(SGE_SRC:.c=.o)
OBJS	= $(SOURCES:.c=.o)

all	: $(OBJS) $(SGE_OBJS) game_server.so
.PHONY	: clean

$(OBJS) : %.o : %.c %.h
	@echo "🔧 File $< needs to be rebuilt"
	-$(CC) $(CFLAGS) -fPIC -c $< -o $@
	@echo ""

$(SGE_OBJS) : storage/$(STORAGE)/%.o : storage/$(STORAGE)/%.c storage/%.h
	@echo "🔧 Storage strategy file $< needs to be rebuilt"
	-$(CC) $(CFLAGS) -fPIC -c $< -o $@
	@echo ""

game_server.so : $(OBJS) $(SGE_OBJS)
	@echo "⛏️ Building game server library..."
	$(CC) $(CFLAGS) -o $@ $^ -shared $(CLIBS)

clean :
	@echo "🗑️ Removing .o and .so files"
	@$(RM) $(OBJS) $(SGE_OBJS) game_server.so
