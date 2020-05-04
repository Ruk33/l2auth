## L2 Auth
Simple Lineage 2 C4 server written as a learning exercise.
Currently working in WINDOWS & LINUX (thanks to Docker).

![L2Auth Demo](doc/screenshot/demo.gif)

## Running the server with Docker
- Install docker & docker-compose
- `docker-compose up`
- Server will start listening to address 127.0.0.1 (local) and port 2106.

## Setting up DEV environment
- Follow instructions to build log dependency from Dockerfile
- `sudo apt install libssl-dev`

## Hot code reload
Hot code reload is a WIP feature, implemented for gameserver development.
Just start the server up and execute `cd l2auth/gameserver && make lib`, this 
will rebuild part  of the code and it will automatically get 
injected in the running server without having to stop it, recompile and start 
again. The code related to this feature is located on `l2auth/game/handler/dynamic_client.c`.

## What to expect
Right now, the server will listen for up to two connections.
For the time being, the server is capable of:
- Create socket/connection
- Listen for connections
- Accept connections
- Encrypt connection with RSA & Blowfish
- Send Lineage 2 packets
- Receive Lineage 2 packets
- Log in
- Enter world
- Restart
- Very early multiple connection support

## Lineage 2 Protocol
Lineage 2 uses TCP in order to send packets.

All integers are little endian.

The first two bytes of the packet contain the packet size and the third one, usually the packet type.

Other than the first packet, all packets are encrypted with blowfish using the following hardcoded key: `;5.]94-31==-%xT!^[$\000`.

## Folder structure
- `core`: Core modules, used by both login server and game server
- `os`: OS related code (sockets, threads, etc.)
- `os/<implementation>`: For the time being only Linux implementation are being written down but more can be added by easily creating a new folder and implementing all the header files in the `os` folder. Make sure to update `game/Makefile` to use the new implementation instead of `os/linux` (replace `$(wildcard ../os/linux/*.c)` with new implementation, ie `$(wildcard ../os/windows/*.c)`).
- `game`: Game server code
- `game/db`: Persistence related code
- `game/db/<implementation>`: Currently I'm working with sqlite since it's the quickest/easiest to implement but, more can be added easily by adding a new folder, implementing all the header files residing in `game/db` and updating `game/Makefile` to include the new implementation instead of sqlite (replace `$(wildcard db/sqlite3/*.c)` with new implementation, ie `$(wildcard db/psql/*.c)`).
- `game/dto`: Data transfer objects
- `game/handler`: Handlers for each client request (ie, entering world)
- `game/handler/client`: Entry point for all game server requests
- `game/packet`: Packets sent by game server
- `login`: Login server code
- `login/dto`: Data transfer objects
- `login/handler`: Handlers for each client request
- `login/packet`: Packets sent by login server

## To do
- [ ] Correctly handle big/little endian
- [x] Display terms of service view
- [x] Display server list
- [x] Allow to enter game server
- [x] Create character
- [x] Enter world
- [x] Walk
- [ ] Lua integration? since we got hot reload I don't know if it makes sense
- [x] Handle multiple connections
- [x] See other players

## Screenshots
Please see doc/screenshot.

## Credits
- l2jserver (http://l2jserver.com/)
- http://fursoffers.narod.ru/Packets.htm
- https://code.google.com/archive/p/l2adenalib/wikis/L2LoginServerProtocol.wiki

## Licence
MIT
