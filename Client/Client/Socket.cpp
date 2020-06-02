#include "Socket.h"

#ifdef _WIN32
	#pragma comment(lib, "Ws2_32.lib")
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <winsock2.h>
#else
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

    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_descriptor < 0) {
        printf("Socket creation error\n");
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;                                          // IPv4
    serv_addr.sin_port = htons(PORT);                                       // port - htons ensures byte order (BE/LE)
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);                       // ip where server listen
    //inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);                 // localhost
    std::memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero));      // set to zero

    // connect client socket to server
    if (connect(socket_descriptor, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        printf("Socket connect error\n");
        exit(-1);
    }

    printf("Connected to server\n");

    in_message = new InMessage();
    out_message = new OutMessage();
}

Socket::~Socket() {
    delete in_message;
    delete out_message;

	#ifdef _WIN32
	    int status = shutdown(socket_descriptor, SD_BOTH);
	    if (status == 0) {
		    status = closesocket(socket_descriptor);
	    }
	#else
	    int status = shutdown(socket_descriptor, SHUT_RDWR);
	    if (status == 0) {
		    status = close(socket_descriptor);
	    }
	#endif

    std::cout << "~Socket: " << status << std::endl;
}

bool Socket::receive_message(char key) {
    // Save message from server
    if (recv(socket_descriptor, reinterpret_cast<char*>(in_message), sizeof(InMessage), 0) < 0) {
        printf("Client receive message error\n");
        return false;
    }
    out_message->direction = key;
    if (send(socket_descriptor, reinterpret_cast<char*>(out_message), sizeof(OutMessage), 0) < 0) {
        printf("Client sending message error\n");
        return false;
    }

    return true;
}

//void Socket::send_message(char key) {
//    out_message->direction = key;
//    if (send(socket_descriptor, (char *) out_message, sizeof(OutMessage), 0) < 0) {
//        printf("Client sending message error\n");
//        exit(-1);
//    }
//}

InMessage *Socket::get_message() {
    return in_message;
}
