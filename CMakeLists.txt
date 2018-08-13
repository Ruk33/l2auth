cmake_minimum_required(VERSION 3.12)
project(l2auth C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-DNOCRYPT")

add_executable(l2auth main.c)
include_directories(/)
include_directories(C:/openssl-1.1.0c-64bit-release-DLL-VS2015/include/openssl)
target_link_libraries(l2auth -LC:/openssl-1.1.0c-64bit-release-DLL-VS2015/lib)
target_link_libraries(
        l2auth
        wsock32
        ws2_32
        libcrypto
        libssl
)