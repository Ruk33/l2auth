#!/bin/bash

make -C build
mv build/libgame_server_lib.so build/game_server_lib.so
cp build/game_server_lib.so ../build
