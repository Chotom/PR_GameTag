# PR_GameTag
Multiplayer game with multi-threading.

## Table of contents
* [Description](#description)
* [Technologies](#technologies)
* [Setup](#setup)
* [Links](#links)
* [Credits](#credits)

## Setup
### (Linux Only) Server
To run the server, you have to run following command in \Server directory:
```
make run
```
### Client
#### (Linux)
First you have to install SDL, SDL dev and SDL_image. To do it simply run:
```
make install
```
and then to compile and run:
```
make run
```
#### (Windows)
To run the client you have to compile with Visual Studio - solution is included with all dlls and binaries in \Client folder.

## Links
Socket tutorial - http://www.linuxhowtos.org/C_C++/socket.htm <br>
TCP/IP Multiprocessing - https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading <br>
Another socket tutorial - https://www.binarytides.com/socket-programming-c-linux-tutorial/ <br>
Textures - https://www.spriters-resource.com/arcade/pacman/<br>

## Credits
- Michał Matczak
- Tomasz Czochański
