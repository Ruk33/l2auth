#!/bin/bash

pushd bin
gcc -Wall -o login_server ../login_server.c ../asocket_linux.c -I../openssl-1.1/x64/include -lcrypto
popd
