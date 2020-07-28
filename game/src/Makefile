main = $(wildcard ./*.c) \
	$(wildcard ../shared/*.c) \
	$(wildcard ../../os/*.c) \
	$(wildcard ../../os/linux/*.c)
objmain = $(main:.c=.o)

CC = gcc
CLIBS = -lsqlite3 -lm -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,build
CFLAGS = $(CLIBS) -v -da -Q -g -O0 -fPIC -I../.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

gameserver: $(objmain)
	$(CC) -o ../build/$@ $^ -v -da -Q -g -O0 -ldl -llog -Wl,-rpath,build

.PHONY: clean
clean:
	rm -f $(objmain) gameserver
