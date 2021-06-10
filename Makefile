.PHONY	: clean all

all : clean login_server game_server

libl2auth.a :
	cd shared && sh build.sh

game_server_lib.so : libl2auth.a
	cd game_server_lib && sh build.sh

game_server : libl2auth.a game_server_lib.so
	cd game_server && sh build.sh

login_server : libl2auth.a
	cd login_server && sh build.sh

clean :
	$(RM) build/libl2auth.a build/game_server_lib.so build/game_server build/login_server
