#!/bin/bash
gcc -Wall -Wextra -Werror login_server.c -o login_server -lcrypto # -lpthread#
gcc -Wall -Wextra -Werror game_server.c -o game_server -lm # -lpthread#
