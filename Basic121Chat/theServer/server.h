#pragma once

#include "WinSock2.h"
#include "WS2tcpip.h"
#include "algorithm"
#include "iostream"
#include "stdio.h"
#include "string"
#include "vector"
#include "thread"
#include "mutex"
#include "map"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define BUFFER_SIZE 256


class Server
{
public:
	Server();
	~Server();

	//Main Function
	int start();

	//Messaging Functions
	void sendMessage(const std::string& message, SOCKET ClientSocket);
	void handleMessages(SOCKET ClientSocket);;
	void broadcastMessage(const std::string& message, SOCKET senderSocket);
	int chat();

	//Connections and identifications functions
	void acceptClient(SOCKET ClientSocket);
	void identify(SOCKET ClientSocket);
	
	//Getter Functions
	void getName();
	SOCKET getSocket();
	
private:
	WSADATA wsaData;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	std::vector<SOCKET> clients;
	std::vector<std::string> users;
	std::string myName;
	int clientN = -1;
};


