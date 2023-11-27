@echo off

call vcvarsall x64 >nul

pushd bin
cl /Zi /W4 /wd4996 /wd4201 /wd4477 /nologo -FC -Gm- -GR- ../login_server.c ../asocket_windows.c -I../openssl-1.1/x64/include ../openssl-1.1/x64/lib/libcrypto.lib Ws2_32.lib 
popd
