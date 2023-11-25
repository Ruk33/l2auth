@echo off
rem tcc login_server.c asocket_windows.c blowfish.c -Iopenssl-1.1/x64/include Ws2_32.lib
cl login_server.c asocket_windows.c blowfish.c -Iopenssl-1.1/x64/include openssl-1.1/x64/lib/libcrypto.lib Ws2_32.lib 
