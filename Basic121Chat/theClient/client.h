#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define BUFFER_SIZE 256

class Client
{
public:
	Client();
	~Client();

	int start();
	void sendMessage(const std::string& messag, SOCKET ConnectSocket);
	void receiveMessages();
	void getName();
	int chat();
private:
	WSADATA wsaData;
	SOCKET ConnectSocket;
	std::string username;
};