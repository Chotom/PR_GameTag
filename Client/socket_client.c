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
    //send(newSocket,buffer,13,0);
    printf("Exit socketThread \n");
    close(socket);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

    /*
        Create client socket in kernel
        AF_INET     - Internet family of IPv4 addresses
        SOCK_STREAM - TCP

        return value - non-negative serv_socket descriptor or -1 on error
     */
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (client_sock < 0) { 
        printf("Socket creation error\n"); 
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
    struct sockaddr_in serv_addr;       // server address
   
    serv_addr.sin_family = AF_INET;                                 // IPv4
    serv_addr.sin_port = htons(PORT);                               // port - htons ensures byte order (BE/LE)
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");             // localhost
    //serverAddr.sin_addr.s_addr = INADDR_ANY                       // bind to all available interfaces
    memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero);    // set to zero

    // connect client socket to server
    if (connect(client_sock, (struct sockaddr *) &serv_addr, sizeof serv_addr) < 0) {
        printf("Socket connect error\n");
        return -1;
    }

    char buffer[1024];

    // receive message from socket
    if (recv(client_sock, buffer, 1024, 0) < 0) {
       printf("Receive failed\n");
    }

    printf("Data received!\n%s\n", buffer);

    return 0;
}
