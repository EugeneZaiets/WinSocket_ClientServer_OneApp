#pragma once
#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <winsock.h>
#define PARTSIZE 200
class Client {
	WSAData m_WSAData;
	SOCKET m_client_socket;
	sockaddr_in m_addr_client;
	char recvbuffer[PARTSIZE] = "";
	char m_LogBuffer[256];
public:
	int m_iResult;
	std::ofstream file;
	Client();
	~Client();
	bool ClientStart();
	bool RecieveFile();
	bool Connect(int port, const char* ipAddress = "localhost");
	bool Disconnect();
};