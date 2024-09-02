#!/bin/bash

pushd bin

gcc -Wall -Wextra -o login_server ../login_server.c ../directory.c ../net_linux.c -I../openssl-1.1/x64/include -lcrypto

gcc -Wall -Wextra -o game_server.so -fPIC -rdynamic -shared ../game_server_lib.c ../directory.c ../net_linux.c -lm

gcc -Wall -Wextra -o game_server ../game_server.c ../net_linux.c ../library.c -ldl

popd
