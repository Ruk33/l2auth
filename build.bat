@echo off

call vcvarsall x64 >nul

pushd bin

del *.pdb

set flags=/Z7 /W4 /wd4996 /wd4201 /wd4477 /nologo /diagnostics:column -FC -Gm- -GR-

cl %flags% ../login_server.c ../directory.c ../net_windows.c ../wqueue.c /Fe:login_server.exe -I../openssl-1.1/x64/include ../openssl-1.1/x64/lib/libcrypto.lib Ws2_32.lib

cl %flags% /LD ../game_server.c ../directory.c ../net_windows.c /Fe:game_server.dll /link -incremental:no /PDB:game_server_%random%.pdb /EXPORT:on_init /EXPORT:on_connection /EXPORT:on_request /EXPORT:on_response /EXPORT:on_disconnect /EXPORT:on_tick Ws2_32.lib

cl %flags% ../runner.c ../net_windows.c ../wqueue.c /Fe:game_server.exe /link /PDB:game_server.pdb Ws2_32.lib

popd
