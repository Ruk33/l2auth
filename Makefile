l2auth: main.c
	gcc -o build/l2auth main.c -std=c99 -I. -lcrypto -lssl