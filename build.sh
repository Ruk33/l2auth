#!/bin/bash

# -Wfloat-equal disabled due to cglm warnings.
CFLAGS="-Wextra -Wall -Wundef -Wshadow -Wcast-align -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter"

mkdir -p build

echo ""
echo "- building game server library"
echo ""
gcc $CFLAGS src/gs_lib.c -lcglm -lm -lcrypto -lsqlite3 -shared -fPIC -o build/game_server_lib.so

echo ""
echo "- building login server"
echo ""
gcc $CFLAGS src/linux/login_server/main.c -lpthread -lcrypto -lsqlite3 -o build/login_server

echo ""
echo "- building game server"
echo ""
gcc $CFLAGS src/linux/game_server/main.c -lpthread -ldl -o build/game_server
