#!/bin/bash

# Exit if any command fails
set -e

echo "----------------------------"
echo "Building login server"
echo "----------------------------"
cd login
make

echo "----------------------------"
echo "Running login server"
echo "----------------------------"
./login_server
