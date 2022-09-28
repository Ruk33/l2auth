#!/bin/bash
echo "Building..."
gcc \
    -DMAX_CONNECTIONS=32 \
    src2/endian.c \
    src2/l2_string.c \
    src2/linux_login_server.c \
    src2/linux_socket.c \
    src2/login_server.c \
    src2/login_request.c \
    src2/login_response.c \
    src2/login_session.c \
    src2/packet.c \
    -lcrypto \
    -o login_server
