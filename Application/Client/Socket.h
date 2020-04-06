#pragma once
#include "InMessage.h"
#include "OutMessage.h"

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
#endif

#ifdef linux
#include <netinet/in.h>
#endif

class Socket {
private:
	int socket_descriptor;
	sockaddr_in serv_addr;
	InMessage* in_message;
	OutMessage* out_message;

public:
	Socket();
	~Socket();

	void receive_message();
	void send_message(char key);
	InMessage* get_message();
};

