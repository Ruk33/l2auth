#!/bin/bash

# Exit if any command fails
set -e

echo "----------------------------"
echo "Building game server library"
echo "----------------------------"
cd game/lib
make clean
make
cd ..

echo "----------------------------"
echo "Building game server"
echo "----------------------------"
make clean
make

echo "----------------------------"
echo "Running game server"
echo "----------------------------"
./game_server
