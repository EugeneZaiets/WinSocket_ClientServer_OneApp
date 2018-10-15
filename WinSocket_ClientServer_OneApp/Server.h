#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <WinSock2.h>
#include <winsock.h>
#define PARTSIZE 200
#define WSA_ACCEPT (WM_USER + 1)
class Server {
	WSADATA m_WSAData;
	HWND hWnd;
	SOCKET m_server_socket, m_client_socket;
	sockaddr_in m_server_addr, m_client_addr;
	unsigned short m_port = 8888;
	char m_LogBuffer[128];
	char recvbuffer[512] = "";
	bool m_Running = 0;
	int partnum = 0;
	int m_iResult;
	std::ifstream file;

	/*Server(Server const&) = delete;
	Server& operator= (Server const&) = delete;
*/
public:
	Server();
	~Server();
	bool ServerStart();
	bool ServerStop();
	bool isRunning();
	void ServerStartAcception();
	void SendFile();
};