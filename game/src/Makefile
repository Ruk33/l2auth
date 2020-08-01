main = $(wildcard ./*.c) \
	$(wildcard ../shared/*.c) \
	$(wildcard ../../os/*.c) \
	$(wildcard ../../os/linux/*.c)
objmain = $(main:.c=.o)

CC = gcc
CLIBS = -lsqlite3 -lm -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,build
CFLAGS = $(CLIBS) -fPIC -I../.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

gameserver: $(objmain)
	mkdir -p ../build && $(CC) -o ../build/$@ $^ -ldl -llog -Wl,-rpath,build

.PHONY: clean
clean:
	rm -f $(objmain) gameserver
