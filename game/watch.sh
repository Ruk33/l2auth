#!/bin/sh
dir1=./
while inotifywait -re modify "$dir1"; do
    make lib 
done
