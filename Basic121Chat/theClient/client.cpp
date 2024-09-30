#include "client.h"

Client::Client() : ConnectSocket(INVALID_SOCKET){
	//Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		exit(1);
	}
}

Client::~Client() {
	//Cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

int Client::start() {
	struct addrinfo* result = NULL, hints;

	//Setup the hints structure
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//Resolve the server address and port
	int iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cerr << "getaddrinfo failed: " << iResult << std::endl;
		WSACleanup();
		exit(1);
	}

	//Create a socket for the client to connect to the server
	ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	//Connect to the server 
	iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		freeaddrinfo(result);
		WSACleanup();
		return 2;
	}

	freeaddrinfo(result);

	std::cout << "Connected to the server. " << std::endl;

	/*std::cout << "please enter your username: ";
	std::getline(std::cin, username);*/

	//start a thread to receive messages
	std::thread(&Client::receiveMessages, this).detach();

	//Main loop to send messages
	std::string message;
	std::string lastMessage;
	while (true) {
		if (lastMessage.size() != 0) {
			std::cout << username << ": ";
		}
		std::getline(std::cin, message);
		lastMessage = message;
		if (message == "sudoexit") {
			sendMessage("### " + username + " disconected. ###", ConnectSocket);
			return 1;
		}
		if (message.size() != 0) {
			sendMessage(username + ": " + message, ConnectSocket);
		}
	}
}

void Client::sendMessage(const std::string& message, SOCKET ConnectSocket) {
	int iResult = send(ConnectSocket, message.c_str(), (int)message.size(), 0);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		exit(1);
	}
}

void Client::receiveMessages() {
	char recvbuf[BUFFER_SIZE];
	int recvbuflen = BUFFER_SIZE;

	while (true) {
		int iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			recvbuf[iResult] = '\0';
			std::cout << "\n" << recvbuf << std::endl;
			std::cout << username << ": ";
			fflush(stdout);
		}
		else if (iResult == 0) {
			std::cout << "Connection closed. " << std::endl;
			break;
		}
		else {
			std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
			break;
		}
	}
}

int Client::chat() {
	std::string message;
	std::string lastMessage;
	while (true) {
		if (lastMessage.size() != 0) {
			std::cout << username << ": ";
		}
		std::getline(std::cin, message);
		lastMessage = message;
		if (message == "sudoexit") {
			sendMessage("### " + username + " disconected. ###", ConnectSocket);
			return 1;
		}
		if (message.size() != 0) {
			sendMessage(username + ": " + message, ConnectSocket);
		}
	}
}

void Client::getName() {
	std::cin>> username;
}