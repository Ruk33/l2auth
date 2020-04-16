csrc = $(wildcard *.c) \
	$(wildcard client/*.c) \
	$(wildcard client/packet/*.c) \
	$(wildcard handler/*.c) \
	$(wildcard packet/*.c) \
	$(wildcard ../login/helper.c) \
	$(wildcard ../login/dto/*.c) \
	$(wildcard ../login/handler/*.c) \
	$(wildcard ../login/packet/*.c) \
	$(wildcard ../core/*.c) \
	$(wildcard ../packet/*.c) \
	$(wildcard ../packet/client/*.c) \
	$(wildcard ../packet/server/*.c) \
	$(wildcard ../socket_strategy/*.c)
obj = $(csrc:.c=.o)

CC = gcc
CLIBS = -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,/home/ruke/l2auth/game/build
CFLAGS = $(CLIBS) -fPIC -I.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

gameserver: $(obj)
	$(CC) -o build/$@ $^ $(CLIBS)

lib: $(obj)
	$(CC) -o build/libgameserver.so -shared -fPIC $^

.PHONY: clean
clean:
	rm -f $(obj) gameserver
