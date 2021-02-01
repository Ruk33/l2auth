#!/bin/bash

# Exit if any command fails
set -e

echo "----------------------------"
echo "Building game server library"
echo "----------------------------"
cd game/lib
make
