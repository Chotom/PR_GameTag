#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>      // getaddrinfo()
#include <arpa/inet.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <pthread.h>
#include <sys/time.h>

#include "../consts/consts.h"
#include "../consts/InMessage.h"
#include "../consts/OutMessage.h"
#include "player.h"

pthread_mutex_t semaphore;

int get_time() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int) 1000 + currentTime.tv_usec / 1000;
}

char direction = 's';
int tagged_id = 0;

struct Player players[CLIENTS_COUNT];

struct thread_data {
    int socket;
    int player_id;
} clients_thread_data[CLIENTS_COUNT];

void *client_thread(void *args) {
    struct thread_data* client_thread_data = (struct thread_data*) args;
    int socket = client_thread_data->socket;
    int player_id = client_thread_data->player_id;
	
    struct InMessage server_message;
    struct OutMessage client_message;

    // once every frame
    while (1) {
        int time_start = get_time();

        // start of critical section
        LOCK

        // prepare message with players positions
        for (int i = 0; i < CLIENTS_COUNT; i++) {
            server_message.pos_x[i] = players[i].pos_x;
            server_message.pos_y[i] = players[i].pos_y;
            server_message.is_tagged[i] = players[i].is_tagged;
        }

        UNLOCK
        // end of critical section

        printf("sending to player %d\n", player_id);  
        // send current players positions
        if (send(socket, &server_message, sizeof server_message, 0) < 0) {
            printf("Server sending message error\n");
            exit(-1);
        }
        printf("sendt to player %d\n", player_id);

        printf("receiving from player %d\n", player_id);
        // receive player's direction
        if (recv(socket, &client_message, sizeof client_message, 0) < 0) {
            printf("Server receive message error\n");
            exit(-1);
        }
        printf("received from player %d\n", player_id);

        // start of critical section
        LOCK
        players[player_id].direction = client_message.direction;
        UNLOCK
        // end of critical section
    	
        int time_end = get_time();
        int duration = time_end - time_start;

        if (duration < FRAME_TIME) {
            usleep((FRAME_TIME - duration) * 1000);
        }
    }

    printf("Exit thread \n");
    close(socket);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

    // initialize semaphore
    pthread_mutex_init(&semaphore, NULL);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * Create server socket in kernel
     *
     *    AF_INET     - Internet family of IPv4 addresses
     *    SOCK_STREAM - TCP
     *    return value - non-negative serv_socket descriptor or -1 on error
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);    // server socket
    int client_sock;                                    // client socket - for later

    if (serv_sock < 0) {
        printf("Socket creation error \n");
        return -1;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  connection address data struct
     *  struct sockaddr_in
     *      sa_family_t     sin_family      address family
     *      in_port_t       sin_port        port in network
     *      struct in_addr  sin_addr        internet address
     *      char            sin_zero[8]     padding field - must be zero
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  internet address struct
     *  struct in_addr
     *      uint32_t        s_addr     address in network byte order (BE/LE)
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    struct sockaddr_in serv_addr;     // server address
    struct sockaddr_in client_addr;   // client address - for later

    serv_addr.sin_family = AF_INET;                                 // IPv4
    serv_addr.sin_port = htons(PORT);                               // port - htons ensures byte order (BE/LE)
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);               // ip where server listen
    //serverAddr.sin_addr.s_addr = INADDR_ANY                       // bind to all available interfaces
    memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);    // set to zero

    // bind server socket to address
    if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Socket binding error\n");
        return -1;
    }

    // listen for max 2 connections queued (2 - number of queued connections we want on a socket)
    if (listen(serv_sock, 2) < 0) {
        printf("Socket listening error\n");
        return -1;
    }

    pthread_t threads_id[CLIENTS_COUNT];

    printf("Server is now listening on port %d\n", PORT);

    // Set tagged player
    players[0].is_tagged = 0;
    players[1].is_tagged = 1;
    for  (int i = 0; i < CLIENTS_COUNT; ++i) {
        if (players[i].is_tagged ==  1){
            tagged_id = i;
        }
    }

    for (int i = 0; i < CLIENTS_COUNT; ++i) {
        int len;
        Player__init(&players[i], i);   // Set start position for players

        // accept client - creates socket for comunication
        client_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &len);

        if (client_sock < 0) {
            printf("Accept error\n");
            return -1;
        }

        /*
        // print user network

        char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
		printf("%s\n", str); // prints "192.0.2.33"
        */

        printf("Accepted client number %d. ", i);

        char host[NI_MAXHOST];
        char service[NI_MAXSERV];
        memset(host, '\0', NI_MAXHOST);
        memset(service, '\0', NI_MAXSERV);

        if (getnameinfo((struct sockaddr *) &client_addr, sizeof client_addr, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            printf("IP %s PORT %s\n", host, service);
        }
    	
        // create new thread for each client to handle communication
        clients_thread_data[i].socket = client_sock;
        clients_thread_data[i].player_id = i;
    	
        if (pthread_create(&threads_id[i], NULL, client_thread, &clients_thread_data) != 0) {
            printf("Failed to create thread for client\n");
            return -1;
        }
    }

    // main game loop - all players connected
    // once every frame
    while (1) {
        int time_start = get_time();

        // start of critical section
        LOCK

        // move players
        for (int i = 0; i < CLIENTS_COUNT; i++) {
            switch (players[i].direction) {
                case 'w':
                    //players[i].pos_y--;
                    Player__AddPos(&players[i], 0, -1);
                    break;
                case 's':
                    //players[i].pos_y++;
                    Player__AddPos(&players[i], 0, 1);
                    break;
                case 'a':
                    //players[i].pos_x--;
                    Player__AddPos(&players[i], -1, 0);
                    break;
                case 'd':
                    //players[i].pos_x++;
                    Player__AddPos(&players[i], 1, 0);
                    break;
            }
        }

        // Check for collision
        for (int i = 0; i < CLIENTS_COUNT; i++) {
            Player__CheckCollision(&players[i], &players[tagged_id]);
        }

        UNLOCK
        // end of critical section

        int time_end = get_time();
        int duration = time_end - time_start;

        if (duration < FRAME_TIME) {
            usleep((FRAME_TIME - duration) * 1000);
        }
    }

    for (int i = 0; i < CLIENTS_COUNT; ++i) {
        pthread_join(threads_id[i], NULL);
    }

    return 0;
}
