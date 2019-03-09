l2auth: main.c
	gcc -o build/l2auth main.c -std=c99 -I. -lcrypto -lssl -Wextra -Wall -Wfloat-equal -Wshadow -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
