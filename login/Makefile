csrc = $(wildcard *.c) \
	$(wildcard dto/*.c) \
	$(wildcard handler/*.c) \
	$(wildcard packet/*.c) \
	$(wildcard ../core/*.c) \
	$(wildcard ../packet/*.c) \
	$(wildcard ../packet/client/*.c) \
	$(wildcard ../packet/server/*.c) \
	$(wildcard ../os/*.c) \
	$(wildcard ../os/linux/*.c)
obj = $(csrc:.c=.o)

CC = gcc
CLIBS = -lpthread -llog -lcrypto -lssl
CFLAGS = $(CLIBS) -I.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

loginserver: $(obj)
	$(CC) -o build/$@ $^ $(CLIBS)

.PHONY: clean
clean:
	rm -f $(obj) build/loginserver
