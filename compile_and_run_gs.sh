#!/bin/bash

# Exit if any command fails
set -e

echo "----------------------------"
echo "Building game server library"
echo "----------------------------"
cd game/lib
make
cd ..

echo "----------------------------"
echo "Building game server"
echo "----------------------------"
make

echo "----------------------------"
echo "Running game server"
echo "----------------------------"
./game_server
