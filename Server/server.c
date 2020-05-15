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

pthread_mutex_t semaphore;

int get_time() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int) 1000 + currentTime.tv_usec / 1000;
}

char direction = 's';

struct player {
    char direction;
    int pos_x;
    int pos_y;
} players[CLIENTS_COUNT];

struct in_thread_data {
    int socket;
    int player_id;
} in_thread_player_data[CLIENTS_COUNT];


void *out_thread(void *arg) {
    int socket = *((int *) arg);

    struct InMessage mess;

    // once every frame
    while (1) {
        int time_start = get_time();

        // start of critical section
        LOCK

        // prepare message with players positions
        for (int i = 0; i < CLIENTS_COUNT; i++) {
            mess.pos_x[i] = players[i].pos_x;
            mess.pos_y[i] = players[i].pos_y;
        }
        UNLOCK
        // end of critical section

        // send current players positions
        if (send(socket, &mess, sizeof mess, 0) < 0) {
            printf("Server sending message error\n");
            exit(-1);
        }

        int time_end = get_time();
        int duration = time_end - time_start;

        if (duration < FRAME_TIME) {
            usleep((FRAME_TIME - duration) * 1000);
        }
    }

    printf("Exit out_thread \n");
    close(socket);
    pthread_exit(NULL);
}

void *in_thread(void *args) {
    struct in_thread_data *current_thread_data = (struct in_thread_data *) args;
    int socket = current_thread_data->socket;
    int player_id = current_thread_data->player_id;

    while (1) {
        struct OutMessage mess;

        // wait for player's direction to change
        if (recv(socket, &mess, sizeof mess, 0) < 0) {
            printf("Server receive message error\n");
            exit(-1);
        }

        // start of critical section
        LOCK
        //direction = dir;
        players[player_id].direction = mess.direction;
        UNLOCK
        // end of critical section
    }

    printf("Exit in_thread \n");
    close(socket);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

    // initialize semaphore
    pthread_mutex_init(&semaphore, NULL);

    /*
        Create server socket in kernel
        AF_INET     - Internet family of IPv4 addresses
        SOCK_STREAM - TCP

        return value - non-negative serv_socket descriptor or -1 on error
     */
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0),    // server socket
    client_sock;                                    // client socket - for later

    if (serv_sock < 0) {
        printf("Socket creation error \n");
        return -1;
    }

    /*
        connection address data struct
        struct sockaddr_in
            sa_family_t     sin_family      address family
            in_port_t       sin_port        port in network
            struct in_addr  sin_addr        internet address
            char            sin_zero[8]     padding field - must be zero

    
        internet address struct
        struct in_addr
            uint32_t        s_addr     address in network byte order (BE/LE)
        
     */
    struct sockaddr_in serv_addr,       // server address
    client_addr;                    // client address - for later

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

    // listen for max 2 connections queued
    // 2 - number of queued connections we want on a socket
    if (listen(serv_sock, 2) < 0) {
        printf("Socket listening error\n");
        return -1;
    }

    pthread_t out_threads_id[CLIENTS_COUNT];
    pthread_t in_threads_id[CLIENTS_COUNT];

    printf("Server is now listening on port %d\n", PORT);

    for (int i = 0; i < CLIENTS_COUNT; ++i) {
        int len;

        // accept client - creates socket for comunication
        client_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &len);
        /*char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
		printf("%s\n", str); // prints "192.0.2.33"
        printf("Accepted client number %d: ", i);*/
        if (client_sock < 0) {
            printf("Accept error\n");
            return -1;
        }

        printf("Accepted client number %d. ", i);

        char host[NI_MAXHOST];
        char service[NI_MAXSERV];
        memset(host, '\0', NI_MAXHOST);
        memset(service, '\0', NI_MAXSERV);

        if (getnameinfo((struct sockaddr *) &client_addr, sizeof client_addr, host, NI_MAXHOST, service, NI_MAXSERV,
                        0) == 0) {
            printf("IP %s PORT %s\n", host, service);
        }

        // create new thread for each client to handle outgoing messages
        if (pthread_create(&out_threads_id[i], NULL, out_thread, &client_sock) != 0) {
            printf("Failed to create thread for client\n");
            return -1;
        }

        // create new thread for each client to handle incoming messages
        in_thread_player_data[i].socket = client_sock;
        in_thread_player_data[i].player_id = i;

        if (pthread_create(&in_threads_id[i], NULL, in_thread, &in_thread_player_data[i]) != 0) {
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
                    players[i].pos_y--;
                    break;
                case 's':
                    players[i].pos_y++;
                    break;
                case 'a':
                    players[i].pos_x--;
                    break;
                case 'd':
                    players[i].pos_x++;
                    break;
            }
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
        pthread_join(out_threads_id[i], NULL);
        pthread_join(in_threads_id[i], NULL);
    }

    return 0;
}
