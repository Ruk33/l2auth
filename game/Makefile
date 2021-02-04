C_FILES=\
	game_server_lib.c \
	main.c \
	memory.c \
	server.c \
	socket.c
H_FILES=$(C_FILES:.c=.h)
O_FILES=$(C_FILES:.c=.o)

CC=gcc
CFLAGS=-std=c99 -Wall -Werror -Wextra -g -pedantic
CLIBS=-ldl

%.o: %.c $(H_FILES)
	$(CC) $(CFLAGS) -c -o $@ $<

game_server: $(O_FILES)
	$(CC) $(CFLAGS) $(CLIBS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(O_FILES)
