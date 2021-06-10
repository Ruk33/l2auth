cmake_minimum_required(VERSION 3.16.3)

project(game_server_lib)

set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -O2 -g -pedantic")
set(OPENSSL_USE_STATIC_LIBS true)

include(FindSQLite3)

find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(../build)
link_directories(../build)

add_library(
        game_server_lib
        SHARED
        ../src/storage/sqlite.c
        ../src/gs_character_template.c
        ../src/gs_character.c
        ../src/gs_crypt.c
        ../src/gs_lib.c
        ../src/gs_packet_auth_login.c
        ../src/gs_packet_auth_request.c
        ../src/gs_packet_char_select_request.c
        ../src/gs_packet_char_select.c
        ../src/gs_packet_create_char_request.c
        ../src/gs_packet_create_char.c
        ../src/gs_packet_d0.c
        ../src/gs_packet_new_char.c
        ../src/gs_packet_protocol_version.c
        ../src/gs_packet_quest_list.c
        ../src/gs_packet_user_info.c
        ../src/gs_request.c
        ../src/gs_session.c
)

target_link_libraries(game_server_lib OpenSSL::Crypto)
target_link_libraries(game_server_lib SQLite::SQLite3)
target_link_libraries(game_server_lib l2auth)
