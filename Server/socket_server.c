#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>

#define PORT 9990
#define CLIENTS_COUNT 2

void * socket_thread(void *arg) {
    int socket = *((int *)arg);
    //recv(newSocket , client_message , 2000 , 0);
    // Send message to the client socket 
    //pthread_mutex_lock(&lock);
    //pthread_mutex_unlock(&lock);
    sleep(1);

    char message[1024] = "dupa dupa gowno cycki";
    send(socket, message, strlen(message), 0);

    //send(newSocket,buffer,13,0);
    printf("Exit socketThread \n");
    close(socket);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

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
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");             // localhost
    //serverAddr.sin_addr.s_addr = INADDR_ANY                       // bind to all available interfaces
    memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);    // set to zero

    // bind server socket to address
    if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Socket binding error\n");
        return -1;
    }

    // listen for max 2 connections queued
    if (listen(serv_sock, 2) < 0) {
        printf("Socket listening error\n");
        return -1;
    }

    pthread_t tid[CLIENTS_COUNT];

    printf("Server is now listening on port %d\n", PORT);

    for (int i = 0; i < CLIENTS_COUNT; ++i) {
        int clilen;

        // accept client - creates socket for comunication
        client_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &clilen);
        if (client_sock < 0) {
            printf("Accept error\n");
            return -1;
        }

        printf("Accepted first client\n");

        sleep(1);

        // create new thread for each client
        if (pthread_create(&tid[0], NULL, socket_thread, &client_sock) != 0) {
            printf("Failed to create thread for client\n");
            return -1;
        }
    }

    sleep(10);

    return 0;
}
