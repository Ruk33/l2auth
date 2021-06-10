cmake_minimum_required(VERSION 3.16.3)

project(l2auth)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -O2 -g -pedantic")

add_library(
        l2auth
        STATIC
        ../src/linux/os_socket.c # Todo: Should be set depending on OS
        ../src/config.c
        ../src/conn.c
        ../src/l2_string.c
        ../src/packet.c
        ../src/util.c
)
