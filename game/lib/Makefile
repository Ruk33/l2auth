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

STORAGE_HEADERS	= $(wildcard storage/*.h)

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
STORAGE_STG	= memory

SOURCES	=					\
	$(wildcard client_request/*.c)		\
	$(wildcard server_packet/*.c)		\
	$(wildcard state_machine/character/*.c)	\
	$(wildcard state_machine/*.c)		\
	$(wildcard storage/$(STORAGE_STG)/*.c)	\
	$(wildcard *.c)

OBJS	= $(SOURCES:.c=.o)

all	: $(OBJS) game_server.so
.PHONY	: clean

$(OBJS) : %.o : %.c %.h
	@echo "🔧 File $< needs to be rebuilt"
	-$(CC) $(CFLAGS) -fPIC -c $< -o $@
	@echo ""

game_server.so : $(OBJS) $(STORAGE_HEADERS)
	@echo "⛏️ Building game server library..."
	$(CC) $(CFLAGS) -o $@ $^ -shared $(CLIBS)

clean :
	@echo "🗑️ Removing .o and .so files"
	@$(RM) $(OBJS) game_server.so
