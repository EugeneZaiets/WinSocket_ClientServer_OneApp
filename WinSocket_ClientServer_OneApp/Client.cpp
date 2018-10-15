#include "Client.h"
#pragma warning(disable : 4996)
#pragma comment(lib, "WS2_32.lib")

Client::Client() {}
Client::~Client() {}

bool Client::ClientStart() {
	if (WSAStartup(MAKEWORD(2, 2), &m_WSAData) == NULL) {
		std::cout << "Client WSAStartup is successful\n";
		wsprintf(m_LogBuffer, "Client %s is %s", m_WSAData.szDescription, m_WSAData.szSystemStatus);
		m_WSAData.iMaxUdpDg = 512;
		std::cout << "iMaxUdpDg size : " << m_WSAData.iMaxUdpDg << " bytes." << std::endl;
		std::cout << m_LogBuffer << std::endl;
	}
	else {
		std::cout << "WSAStartUp Error : " << WSAGetLastError();
		std::cin.get();
		return false;
	}
	return true;
}

bool Client::Connect(int port, const char* ipAddress)
{
	m_client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_client_socket == INVALID_SOCKET) {
		std::cout << "Error : INVALID_SOCKET | Last Error : " << WSAGetLastError() << "\n";
		return 0;
	} 
	else std::cout << "Creating of TCP-socket is successful.\n";
	m_addr_client.sin_family = AF_INET;
	m_addr_client.sin_port = htons(port);
	if (inet_addr(ipAddress) == INADDR_NONE) {
		std::cout << "Error "<<  WSAGetLastError() << ": Invalid Address ::" << ipAddress << std::endl;
		return 0;
	}
	else m_addr_client.sin_addr.S_un.S_addr = inet_addr(ipAddress);
	if (connect(m_client_socket, (sockaddr*)&m_addr_client, sizeof(m_addr_client)) < 0) {
		std::cout << "Connection Error : " << WSAGetLastError() << std::endl;
		return 0;
	}
	else {
		std::cout << "Connected to Server. IP - " << inet_ntoa(m_addr_client.sin_addr) << " : "
			<< m_addr_client.sin_port << std::endl;
			return 1;
	}
}

bool Client::RecieveFile() {
	static std::streampos position = 0;
	static int sum = 0;
	file.seekp(position);
	m_iResult = recv(m_client_socket, recvbuffer, PARTSIZE, 0);
	if (m_iResult > 0) {
		file.open("text_client.txt", std::ios::binary | std::ios::app | std::ios::out);
		if (file.is_open()) {
			if (m_iResult >= PARTSIZE) file.write(recvbuffer, PARTSIZE);
			else file.write(recvbuffer, m_iResult);
			file.flush();
			std::cout << "Recieve bytes : " << m_iResult << std::endl;
			position += PARTSIZE;
			sum += m_iResult;
			file.close();
		}
		else {
			std::cout << "File didn't open. Error :" << GetLastError() << std::endl;
			return 0;
		}
	}
	else if (m_iResult == 0) {
		std::cout << "Connection is closing." << std::endl;
		std::cout << "Total bytes : " << sum << std::endl;
		position = 0;
		sum = 0;
		return 0;
	}
	else {
		std::cout << "Recieve is failed. Error :" << WSAGetLastError() << std::endl;
		return 0;
	}
	return 1;
}

bool Client::Disconnect() {
	closesocket(m_client_socket);
	std::cout << "Disconnected.\n";
	WSACleanup();
	std::cin.get();
	return 1;
}