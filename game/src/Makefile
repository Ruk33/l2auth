# Recursive wildcard
#
# Example:
#	$(call rwildcard,src,*.c *.h)
#	Finds all the .c and .h files in src
#
# Credits to https://stackoverflow.com/a/18258352
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

main = $(call rwildcard,.,*.c) \
	$(call rwildcard,../../os/linux,*.c)

objmain = $(main:.c=.o)

debuggameserver = $(call rwildcard,.,*.expand *.vregs *.into_cfglayout *.jump *.reginfo *.outof_cfglayout *.split1 *.dfinit *.asmcons *.ira *.reload *.split2 *.pro_and_epilogue *.jump2 *.stack *.alignments *.mach *.barriers *.shorten *.nothrow *.dwarf2 *.final *.dfinish *.mode_sw)

CC = gcc
CFLAGS = -std=c99 -Wall -g -O0 -I../..
CLIBS = -ldl -lpthread -llog

gameserver: $(objmain)
	mkdir -p ../build && $(CC) $(CLIBS) $(CFLAGS) -o ../build/$@ $^

.PHONY: clean
clean:
	rm -f $(objmain) $(debuggameserver) gameserver
