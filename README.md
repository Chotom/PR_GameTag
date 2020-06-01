# PR_GameTag
Gra multiplayer zrealizowana w ramach projektu z przedmiotu przetwarzanie rozproszone

## Table of contents
* [Description](#description)
* [Technologies](#technologies)
* [Setup](#setup)
* [Links](#links)
* [Credits](#credits)

## Description

Inspirowana PacManem gra w berka - widok z góry. Dwóch graczy na planszy - jeden goniący, drugi uciekający. Aby wygrać, goniący musi dotknąć uciekającego w wyznaczonym czasie (najprostsza wersja). Możliwe mechaniki wyłaniania zwycięzcy - zamiana ról, podział na rundy lub zbieranie puntków za czas, który się ucieka itp.

MUST:
- ~~komunikacja sieciowa (przesyłanie współrzędnych graczy, kierunku poruszania)~~
- ~~poruszanie za pomocą klawiatury~~
- ~~niewychodzenie graczy za krawędzie~~
- ~~wykrywanie kolizji/dotknięcia między graczami~~

SHOULD:
- ~~mapa (labirynt) z kolizjami/ścianami/przeszkodami~~
- determinowanie zwycięzcy (gra na czas lub gra na punkty)

COULD:
- bonusy/powerupy na planszy
- ~~różne plansze~~
- system punktów
- menu
- ~~więcej niż 2 graczy na planszy~~

WON'T:
- tryb solo + AI
- dobra grafika

## Technologies
C++ (SDL)

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
Other:
https://www.bogotobogo.com/cplusplus/sockets_server_client.php
http://www.crypto-it.net/pl/teoria/protokoly-tcp-ip.html
https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading
https://linux.die.net/man/7/ip


## Credits
- Michał Matczak
- Tomasz Czochański
