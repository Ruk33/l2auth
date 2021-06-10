cmake_minimum_required(VERSION 3.16.3)

project(game_server)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -O2 -g -pedantic")

include_directories(../build)
link_directories(../build)

add_executable(
        game_server
        ../src/linux/game_server/main.c
)

# dlopen, close, etc. functions
target_link_libraries(game_server ${CMAKE_DL_LIBS})
target_link_libraries(game_server l2auth)
