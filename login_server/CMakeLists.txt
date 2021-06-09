cmake_minimum_required(VERSION 3.16.3)

project(login_server)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -O2 -g -pedantic")

set(OPENSSL_USE_STATIC_LIBS true)
find_package(OpenSSL REQUIRED)

add_executable(
        login_server
        ../src/linux/login_server/main.c
        ../src/linux/os_socket.c
        ../src/config.c
        ../src/conn.c
        ../src/ls_blowfish.c
        ../src/ls_crypt.c
        ../src/ls_lib.c
        ../src/ls_packet_gg_auth.c
        ../src/ls_packet_init.c
        ../src/ls_packet_ok.c
        ../src/ls_packet_play_ok.c
        ../src/ls_packet_server_list.c
        ../src/ls_request.c
        ../src/ls_rsa.c
        ../src/ls_session.c
        ../src/packet.c
        ../src/server.c
        ../src/util.c
)

target_link_libraries(login_server OpenSSL::Crypto)
