csrc = $(wildcard *.c) \
	$(wildcard dto/*.c) \
	$(wildcard handler/*.c) \
	$(wildcard packet/*.c) \
	$(wildcard ../core/byte_builder.c) \
	$(wildcard ../core/byte_reader.c) \
	$(wildcard ../core/circular_memory_alloc.c) \
	$(wildcard ../core/endian.c) \
	$(wildcard ../core/hex.c) \
	$(wildcard ../core/l2_blowfish_key.c) \
	$(wildcard ../core/l2_packet.c) \
	$(wildcard ../core/l2_raw_packet.c) \
	$(wildcard ../core/l2_rsa_key.c) \
	$(wildcard ../core/l2_string.c) \
	$(wildcard ../core/memory.c) \
	$(wildcard ../core/session_key.c) \
	$(wildcard ../os/*.c) \
	$(wildcard ../os/linux/*.c)
obj = $(csrc:.c=.o)

CC = gcc
CLIBS = -lpthread -llog -lcrypto -lssl
CFLAGS = $(CLIBS) -I.. -fPIC -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

loginserver: $(obj)
	$(CC) -o build/$@ $^ $(CLIBS)

.PHONY: clean
clean:
	rm -f $(obj) build/loginserver
