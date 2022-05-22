#!/bin/bash
gcc -Wall -Wextra login_server.c -o login_server -lcrypto -lpthread
gcc -Wall -Wextra game_server.c -o game_server -lm -lpthread
