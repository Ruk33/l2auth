# -DNOCRYPT https://stackoverflow.com/questions/21473039/sqlcipher-on-windows-using-cygwin
l2auth: main.c
	gcc main.c -std=c99 -DNOCRYPT -I. -lwsock32 -lws2_32 -llibcrypto -llibssl $(CFLAGS)