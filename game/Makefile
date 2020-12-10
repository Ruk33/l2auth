C_FILES=\
	game_server_lib.c \
	main.c \
	memory.c \
	request_manager.c \
	response_manager.c \
	server.c \
	socket.c
H_FILES=$(C_FILES:.c=.h)
O_FILES=$(C_FILES:.c=.o)

CC=gcc
CFLAGS=-std=c99 -Wall -Werror -Wextra -g -pedantic
CLIBS=-ldl -lpthread -ldatastructures

%.o: %.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

game_server: $(O_FILES)
	$(CC) -o $@ $^ $(CFLAGS) $(CLIBS)

.PHONY: clean
clean:
	rm -f $(O_FILES)
