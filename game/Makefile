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
CLIBS = -lpthread -llog -lcrypto -lssl
CFLAGS = $(CLIBS) -I.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

gameserver: $(obj)
	$(CC) -o build/$@ $^ $(CLIBS)

.PHONY: clean
clean:
	rm -f $(obj) gameserver
