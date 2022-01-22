#!/bin/bash

# -Wfloat-equal disabled due to cglm warnings.
CFLAGS="-Wextra -Wall -Wundef -Wshadow -Wcast-align -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter"

mkdir -p build

echo "- building game server library"
gcc $CFLAGS src/gs_lib.c -lcglm -lcrypto -lsqlite3 -shared -fPIC -o build/game_server_lib.so

echo "- building login server"
gcc $CFLAGS src/linux/login_server/main.c -lcrypto -lsqlite3 -o build/login_server

echo "- building game server"
gcc $CFLAGS src/linux/game_server/main.c -ldl -o build/game_server
