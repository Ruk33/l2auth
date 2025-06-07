@echo off

del bin\*.pdb

zig cc -Wall -Wextra -Werror login_server.c -o bin\login_server.exe -Iopenssl-1.1\x64\include openssl-1.1\x64\lib\libcrypto.lib -lws2_32

zig cc -shared -Wall -Wextra -Werror game_server_lib.c -o bin\game_server_lib.dll -lws2_32

zig cc -Wall -Wextra -Werror game_server.c -o bin\game_server.exe -lws2_32
