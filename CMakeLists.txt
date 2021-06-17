cmake_minimum_required(VERSION 3.16.3)

project(login_server)
project(game_server)
project(game_server_lib)

set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -O2 -g -pedantic")

include(FindSQLite3)

find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(
        login_server
        src/linux/login_server/main.c
        src/linux/os_io.c # Todo: Should be set depending on OS
        src/ls_blowfish.c
        src/ls_crypt.c
        src/ls_lib.c
        src/ls_packet_gg_auth.c
        src/ls_packet_init.c
        src/ls_packet_ok.c
        src/ls_packet_play_ok.c
        src/ls_packet_server_list.c
        src/ls_request.c
        src/ls_rsa.c
        src/ls_session.c
        src/conn.c
        src/packet.c
        src/server.c
        src/util.c
)

add_executable(
        game_server
        src/linux/game_server/main.c
        src/linux/os_io.c # Todo: Should be set depending on OS
)

add_library(
        game_server_lib
        SHARED
        src/storage/sqlite.c
        src/gs_character_template.c
        src/gs_character.c
        src/gs_crypt.c
        src/gs_lib.c
        src/gs_packet_action_request.c
        src/gs_packet_attack.c
        src/gs_packet_auth_login.c
        src/gs_packet_auth_request.c
        src/gs_packet_auto_attack.c
        src/gs_packet_char_info.c
        src/gs_packet_char_select_request.c
        src/gs_packet_char_select.c
        src/gs_packet_create_char_request.c
        src/gs_packet_create_char.c
        src/gs_packet_d0.c
        src/gs_packet_leave_world.c
        src/gs_packet_move_request.c
        src/gs_packet_move.c
        src/gs_packet_new_char.c
        src/gs_packet_npc_info.c
        src/gs_packet_protocol_version.c
        src/gs_packet_quest_list.c
        src/gs_packet_target_selected.c
        src/gs_packet_user_info.c
        src/gs_packet_validate_pos_request.c
        src/gs_packet_validate_pos.c
        src/gs_random_id.c
        src/gs_request.c
        src/gs_session.c
        src/conn.c
        src/l2_string.c
        src/packet.c
        src/util.c
)

# Blowfish and RSA
target_link_libraries(login_server OpenSSL::Crypto)

# dlopen, dlclose, etc. functions
target_link_libraries(game_server ${CMAKE_DL_LIBS})

target_link_libraries(game_server_lib OpenSSL::Crypto)
target_link_libraries(game_server_lib SQLite::SQLite3)
