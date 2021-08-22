@echo off

@REM Flag reference
@REM /Gm-   : disable incremental builds
@REM /MT    : improves compatability
@REM /D     : define
@REM /WL    : one line diagnostic
@REM /O2    : optimization (favors speed)
@REM /FC    : use full path in messages

@REM Flags
set CFLAGS=/nologo /WL /O2 /MTd /FC
set CINCLUDES=
set CLIBS=

mkdir build
del build\*.pdb

@REM Game server library
@echo - building game server library
cl %CFLAGS% src/win32/deps/sqlite3/sqlite3.c src/gs_lib.c /Isrc/win32/deps/sqlite3 /Isrc/win32/deps/openssl/include /LD /link /LIBPATH:src/win32/deps/openssl/lib /EXPORT:gs_lib_load /EXPORT:gs_lib_unload /EXPORT:gs_lib_new_conn /EXPORT:gs_lib_new_req /EXPORT:gs_lib_disconnect /EXPORT:gs_lib_tick /incremental:no -opt:ref /OUT:build/game_server_lib.dll libcrypto.dll.a

@REM Login server
@echo - building login server
cl %CFLAGS% src/win32/login_server/main.c /Isrc/win32/deps/openssl/include /link /LIBPATH:src/win32/deps/openssl/lib /incremental:no /OUT:build/login_server.exe libcrypto.dll.a

@REM Game server
@echo - building game server
cl %CFLAGS% src/win32/game_server/main.c /link /incremental:no /OUT:build/game_server.exe
