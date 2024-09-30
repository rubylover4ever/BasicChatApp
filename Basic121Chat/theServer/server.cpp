#include "menu.h"


Server::Server() : ListenSocket(INVALID_SOCKET) {
	//Initialize winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult<<std::endl;
		exit(1);
	}
}

Server::~Server() {
	closesocket(ListenSocket);
	WSACleanup();
}
int Server::start() {
	struct addrinfo* result = nullptr, hints;
	//Setup the hints structure
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cerr << "getaddrinfo failed: " << iResult << std::endl;
		WSACleanup();
		exit(1);
	}

	//Create a socket for the server to listen for incoming connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	//Bind the socket to the network address and port
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult != 0) {
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	freeaddrinfo(result);

	//Start listening for incoming connections
	iResult = listen(ListenSocket, 32);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	//std::cout << "Wait for user/s to connect\n";
	
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
	else {
		std::cout << "User connected.\n";
	}
	
	std::thread(&Server::handleMessages, this, ClientSocket).detach();

	std::string message;
	std::string lastMessage;
	while (true) {
		if (lastMessage.size() != 0) {
			std::cout << myName << ": ";
		}
		std::getline(std::cin, message);
		lastMessage = message;
		if (message == "sudoexit") {
			broadcastMessage("### " + myName + " disconected. ###", ClientSocket);
			sendMessage(myName + " disconected.", ClientSocket);
			return 1;
		}
		if (message.size() != 0){
			sendMessage(myName + ": " + message, ClientSocket);
		}
	}
}


	

//Messaging Functions
void Server::sendMessage(const std::string& message, SOCKET ClientSocket)
{
	int iResult = send(ClientSocket, message.c_str(), (int)message.size(), 0);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
}

void Server::handleMessages(SOCKET ClientSocket) {
	char recvbuf[BUFFER_SIZE];
	int recvbuflen = BUFFER_SIZE;
	//Handle incoming messages from client
	while (true) {

		int iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			recvbuf[iResult] = '\0';// '\0' is used to terminate the message;
			std::string message = recvbuf;

			std::cout << std::endl << message.c_str() << std::endl;
			std::cout << myName<<": ";
			//Broadcast the message to all other clients
			broadcastMessage(message, ClientSocket);
		}
	}

	closesocket(ClientSocket);

	//Remove client from the list
	{
		clients.erase(std::remove(clients.begin(), clients.end(), ClientSocket), clients.end());

	}
	//Announce the client has left the chat
	std::string leaveMessage = users[clientN] + " has left the chat.\n";
	broadcastMessage(leaveMessage, ClientSocket);
	clientN--;
}

void Server::broadcastMessage(const std::string& message, SOCKET senderSocket = INVALID_SOCKET) {
	for (auto client : clients) {
			if (client != senderSocket) { // Don't send the message back to the sender 
				std::cout<<std::endl;
				send(client, message.c_str(), (int)message.length(), 0);
			}
	
	} 
}

int Server::chat() {

	//std::thread(&Server::handleMessages, this, ClientSocket).detach();

	std::string message;
	std::string lastMessage;
	while (true) {
		if (lastMessage.size() != 0) {
			std::cout << myName << ": ";
		}
		std::getline(std::cin, message);
		lastMessage = message;
		if (message == "sudoexit") {
			broadcastMessage("### " + myName + " disconected. ###", ClientSocket);
			sendMessage(myName + " disconected.", ClientSocket);
			return 1;
		}
		if (message.size() != 0) {
			sendMessage(myName + ": " + message, ClientSocket);
		}
	}
}

//Connections and identifications functions
void Server::acceptClient(SOCKET ClientSocket){//this function is for future optimization* 
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
	else {
		std::cout << "User online.\n";
	}
	identify(ClientSocket);
}

void Server::identify(SOCKET ClientSocket) { 
	clientN++;
	char recvbuf[BUFFER_SIZE]; 
	int recvbuflen = BUFFER_SIZE; 
	int iResult;

	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0); 
	if (iResult > 0) { 
		recvbuf[iResult] = '\0';// '\0' is used to terminate the message;
		users.push_back(recvbuf);//Store the client's username  
		printf("User connected: %s\n", users[clientN].c_str()); 
		 
		//Announce to everyone that a new user has joined
		std::string joinMessage = users[clientN] +  " has joined the chat.\n";
		broadcastMessage(joinMessage, ClientSocket); 

		//Add the client to the list
		{
			clients.push_back(ClientSocket);
		}
	}
}



//Getter Functions
void Server::getName() {
	std::cin >> myName;
}

SOCKET Server::getSocket() {
	return ClientSocket;
}
