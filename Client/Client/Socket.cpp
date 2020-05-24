#include "Socket.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#endif

#ifdef linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#endif

#include <iostream>
#include <cstring>


Socket::Socket() {

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        std::cout << "Initialization error" << std::endl;
        exit(-1);
    }
#endif

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {
        printf("Socket creation error\n");
        exit(-1);
    }

    server_addr.sin_family = AF_INET;                                          // IPv4
    server_addr.sin_port = htons(PORT);                                       // port - htons ensures byte order (BE/LE)
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);                       // ip where server listen
    //inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);                 // localhost
    std::memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));      // set to zero
	
    // connect client socket to server
    if (connect(tcp_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cout << "TCP socket connect error" << std::endl;
        exit(-1);
    }

    printf("PORT %hd\n", ntohs(server_addr.sin_port));
	
	// create udp socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_socket < 0) {
        printf("UDP socket creation error\n");
        exit(-1);
    }

	// associate udp socket with only one address - only accept datagrams form server
	if (connect(udp_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
		std::cout << "UDP socket connect error" << std::endl;
		exit(-1);
	}
	printf("PORT %hd\n", ntohs(server_addr.sin_port));
	send/*to*/(udp_socket, (char*) "dupa\0", 5, 0/* reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)*/);
	
    in_message = new InMessage();
    out_message = new OutMessage();
}

Socket::~Socket() {
    delete in_message;
    delete out_message;
}

void Socket::receive_message() {
#ifdef _WIN32
	int len;
	
#endif

#ifdef linux
	unsigned int len;
#endif
	sockaddr_in sender_addr;
    // Save message from server
	
	//do {
		//if (
	int bytelen = recv/*from*/(udp_socket, reinterpret_cast<char*>(in_message), sizeof(InMessage), 0/*, reinterpret_cast<struct sockaddr*>(&sender_addr), &len*/);// < 0) {
	printf("got message: %d bytes\n", bytelen);
			//

			//printf("Client receive message error\n");
			//exit(-1);
		//}
	//} while(memcmp(&sender_addr, &server_addr, sizeof(sockaddr_in)) != 0);

}

void Socket::send_message(char key) {
    out_message->direction = key;
    if (send(tcp_socket, (char *) out_message, sizeof(OutMessage), 0) < 0) {
        printf("Client sending message error\n");
        exit(-1);
    }
}

InMessage *Socket::get_message() {
    return in_message;
}
