# Recursive wildcard
#
# Example:
#	$(call rwildcard,src,*.c *.h)
#	Finds all the .c and .h files in src
#
# Credits to https://stackoverflow.com/a/18258352
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

gameserverlib = $(call rwildcard,.,*.c) \
	$(call rwildcard,../shared,*.c) \
	$(call rwildcard,../../core,*.c) \
	$(call rwildcard,../../os/linux,*.c)

objgameserverlib = $(gameserverlib:.c=.o)

debuggameserver = $(call rwildcard,.,*.expand *.vregs *.into_cfglayout *.jump *.reginfo *.outof_cfglayout *.split1 *.dfinit *.asmcons *.ira *.reload *.split2 *.pro_and_epilogue *.jump2 *.stack *.alignments *.mach *.barriers *.shorten *.nothrow *.dwarf2 *.final *.dfinish *.mode_sw)

CC = gcc
CLIBS = -I../.. `pkg-config --cflags --libs glib-2.0` -lsqlite3 -lm -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,build
CFLAGS = $(CLIBS) -shared -fPIC -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

lib: $(objgameserverlib)
	mkdir -p ../build && $(CC) -o ../build/libgameserver.so -shared -fPIC $(CFLAGS) $^

.PHONY: clean
clean:
	rm -f $(objgameserverlib) $(debuggameserver) lib
