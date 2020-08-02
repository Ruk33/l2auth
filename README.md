## L2 Auth
Simple Lineage 2 C4 server written as a learning exercise.

![L2Auth Demo](doc/screenshot/demo.gif)

## Running the server with Docker
- Install docker
- Install docker-compose
- `docker-compose up`
- Server will start listening for connections on localhost (`127.0.0.1`) ports `2106` (loginserver) and `7777` (gameserver)
- Use any user and password (all are valid) and you should be able to log in

## Gameserver hot-code reload
- Run the gameserver
- Make a change in the code
- `cd game/lib && make` or `docker-compose exec gameserver bash -c "cd game/lib && make"` if using docker
- Check the Lineage 2 client, the change should be reflected live

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
- `game/lib`: Server implementation. All of this gets loaded dynamically as a library
- `game/lib/cache`: Code related to cache, saving the state of the world and players
- `game/lib/dto`: Data transfer objects (raw structures)
- `game/lib/entity`: Entities (player, character, etc.)
- `game/lib/request_handler`: Handlers for requests (move, protocol version, etc.)
- `game/lib/storage`: Code related to storage (saving chars, etc.)
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
