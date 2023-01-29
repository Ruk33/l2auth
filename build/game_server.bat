@echo off
rem the server uses stderr to log messages instead of stdout. the reason
rem for this is that stderr doesn't require to be flushed and thus,
rem messages are always recorded (even if the program crashes)
rem 
rem 2>&1 redirects stderr to stdout
rem tee file.txt write output to file.txt
rem meaning, print output to console but also, to a file, this way,
rem if the server crashes we still have the logs.
wsl bash -c "./game_server 2>&1 | tee --append ./game_server.txt"
