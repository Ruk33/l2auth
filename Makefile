.PHONY	: all conf build clean

all : clean build

conf : CMakeLists.txt
	cmake -S . -B build

build : conf
	cd build && make && mv libgame_server_lib.so game_server_lib.so

clean :
	$(RM) -rf build/*
