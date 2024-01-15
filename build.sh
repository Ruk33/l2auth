#!/bin/bash

pushd bin

gcc -o login_server ../login_server.c ../directory.c ../net_linux.c ../wqueue.c -I../openssl-1.1/x64/include -lcrypto -lpthread

gcc -o game_server.so -fPIC -rdynamic -shared ../game_server_lib.c ../directory.c ../net_linux.c ../wqueue.c  -lpthread

gcc -o game_server ../game_server.c ../net_linux.c ../lib.c -lpthread -ldl

popd
