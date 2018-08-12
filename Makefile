l2auth: main.c
	gcc main.c -std=c99 -I. -lwsock32 -lws2_32 -llibcrypto -llibssl -llibtomcrypt -llibtommath $(CFLAGS)