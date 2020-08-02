# Recursive wildcard
#
# Example:
#	$(call rwildcard,src,*.c *.h)
#	Finds all the .c and .h files in src
#
# Credits to https://stackoverflow.com/a/18258352
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

main = $(call rwildcard,.,*.c) \
	$(call rwildcard,../shared,*.c) \
	$(call rwildcard,../../os/linux,*.c)

objmain = $(main:.c=.o)

debuggameserver = $(call rwildcard,.,*.expand *.vregs *.into_cfglayout *.jump *.reginfo *.outof_cfglayout *.split1 *.dfinit *.asmcons *.ira *.reload *.split2 *.pro_and_epilogue *.jump2 *.stack *.alignments *.mach *.barriers *.shorten *.nothrow *.dwarf2 *.final *.dfinish *.mode_sw)

CC = gcc
CLIBS = -lsqlite3 -lm -ldl -lpthread -llog -lcrypto -lssl -Wl,-rpath,build
CFLAGS = $(CLIBS) -fPIC -I../.. -std=c99 -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

gameserver: $(objmain)
	mkdir -p ../build && $(CC) -o ../build/$@ $^ -ldl -llog -Wl,-rpath,build

.PHONY: clean
clean:
	rm -f $(objmain) $(debuggameserver) gameserver
