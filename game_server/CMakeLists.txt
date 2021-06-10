cmake_minimum_required(VERSION 3.16.3)

project(game_server)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wshadow -Wdouble-promotion -Wundef -O2 -g -pedantic")

add_executable(
        game_server
        ../src/linux/game_server/main.c
        ../src/linux/os_socket.c
)

# shared library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../shared ${CMAKE_CURRENT_BINARY_DIR}/shared)

# dlopen, close, etc. functions
target_link_libraries(game_server ${CMAKE_DL_LIBS})
target_link_libraries(game_server shared)
