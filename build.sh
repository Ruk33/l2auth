#!/bin/bash

CFLAGS="-Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wcast-align -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter"

mkdir build

echo ""
echo "- building game server library"
echo ""
gcc $CFLAGS src/gs_lib.c -lm -lcrypto -lsqlite3 -shared -fPIC -o build/game_server_lib.so

echo ""
echo "- building login server"
echo ""
gcc $CFLAGS src/linux/login_server/main.c -lcrypto -lsqlite3 -o build/login_server

echo ""
echo "- building game server"
echo ""
gcc $CFLAGS src/linux/game_server/main.c -ldl -o build/game_server
