#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <sys/time.h>

#define PORT 9990
#define CLIENTS_COUNT 2

#define FPS 60
#define FRAME_TIME (1000/FPS)

int get_time(){
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1000 + currentTime.tv_usec/1000;
}

struct message {
    int pos_x;
    int pos_y;
};

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

    printf("Connected to server\n");

    struct message mess;

    while(1) {
        int time_start = get_time();

        if (recv(client_sock, &mess, sizeof mess, 0) < 0) {
            return -1;
        }

        printf("%d %d\n", mess.pos_x, mess.pos_y);

        int time_end = get_time();
        int duration = time_end - time_start;

        if (duration < FRAME_TIME) {
            usleep((FRAME_TIME - duration) * 1000);
        }
    }

    return 0;
}
