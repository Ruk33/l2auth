CC	= gcc
CFLAGS	= -std=c99 -Wall -Werror -Wextra -g -pedantic
CLIBS	= -lssl -lcrypto -ldatastructures

SOURCES	=				\
	$(wildcard client_request/*.c)	\
	$(wildcard handler/*.c)		\
	$(wildcard server_packet/*.c)	\
	$(wildcard storage/*.c)		\
	$(wildcard *.c)

OBJS	= $(filter-out main.o, $(SOURCES:.c=.o))

all: $(OBJS) login_server

$(OBJS) : %.o : %.c %.h
	@echo "🔧 File $< needs to be rebuilt"
	-$(CC) $(CFLAGS) -c $< -o $@
	@echo ""

login_server : $(OBJS) main.c
	@echo "⛏️ Building login server..."
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

.PHONY: clean
clean:
	@echo "🗑️ Removing .o files"
	@$(RM) $(OBJS)
