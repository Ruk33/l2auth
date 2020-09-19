#!/bin/bash

cd src && make clean && make && cd .. && valgrind --exit-on-first-error=yes --leak-check=full --track-origins=yes build/gameserver
