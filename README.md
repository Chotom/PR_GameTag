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
- komunikacja sieciowa (przesyłanie współrzędnych graczy, kierunku poruszania)
- poruszanie za pomocą klawiatury
- niewychodzenie graczy za krawędzie
- wykrywanie kolizji/dotknięcia między graczami

SHOULD:
- mapa (labirynt) z kolizjami/ścianami/przeszkodami
- determinowanie zwycięzcy (gra na czas lub gra na punkty)

COULD:
- bonusy/powerupy na planszy
- różne plansze
- system punktów
- menu
- więcej niż 2 graczy na planszy

WON'T:
- tryb solo + AI
- dobra grafika

## Technologies
C++

## Setup

## Links
Socket tutorial - http://www.linuxhowtos.org/C_C++/socket.htm <br>
TCP/IP Multiprocessing - https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading <br>
Kolejny socket tutorial - https://www.binarytides.com/socket-programming-c-linux-tutorial/ <br>
Więcej o TCP/IP - https://www.thegeekstuff.com/2011/12/c-socket-programming/ <br>
Diagram sekwencji - http://zasoby.open.agh.edu.pl/~09sbfraczek/diagram-sekwencji%2C1%2C13.html <br>
O TCP i problemach - http://polishlinux.wikidot.com/sockety:teoriasockety:tcp <br>

## Credits
- Michał Matczak
- Tomasz Czochański
