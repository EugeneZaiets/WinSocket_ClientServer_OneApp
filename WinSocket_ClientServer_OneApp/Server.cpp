#pragma warning(disable : 4996)
#pragma comment(lib, "WS2_32.lib")
#include "Server.h"

Server::Server() {};
Server::~Server() {};
bool Server::ServerStart() {
	std::cout << "Establishing server...\n";
	if (WSAStartup(MAKEWORD(2, 2), &m_WSAData) == NULL) {
		std::cout << "Server WSAStartup is successful\n";
		wsprintf(m_LogBuffer, "Server %s is %s", m_WSAData.szDescription, m_WSAData.szSystemStatus);
		m_WSAData.iMaxUdpDg = 512;
		std::cout << "iMaxUdpDg size : " << m_WSAData.iMaxUdpDg << " bytes." << std::endl;
		std::cout << m_LogBuffer << std::endl;
	}
	else {
		std::cout << "WSAStartUp Error : " << WSAGetLastError();
		std::cin.get();
		return false;
	}
	m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_server_socket == INVALID_SOCKET) {
		std::cout << "Error : INVALID_SOCKET| Last Error : " << WSAGetLastError() << "\n";
		return false;
	}
	else std::cout << "Creating of TCP-socket is successful.\n";

	m_server_addr.sin_family = AF_INET;
	m_server_addr.sin_port = htons(m_port);
	PHOSTENT phe;
	phe = gethostbyname("localhost");
	if (phe == NULL) {
		std::cout << "GetHostByName Error : " << WSAGetLastError() << std::endl;
		return false;
	}
	memcpy((char FAR*) &m_server_addr.sin_addr, phe->h_addr_list[0], phe->h_length);
	std::cout << "IP-Address of domain :: " << inet_ntoa(m_server_addr.sin_addr)
		<< " : "<< m_server_addr.sin_port << std::endl;

	if (bind(m_server_socket, (sockaddr*)&m_server_addr, sizeof(m_server_addr)) == SOCKET_ERROR) {
		std::cout << "Bind Error : " << WSAGetLastError() << std::endl;
		if (WSAGetLastError() == 10048) {
			std::cout << "Error : Server with IP :: " << inet_ntoa(m_server_addr.sin_addr) << " : " 
			<< m_server_addr.sin_port << " :: is established." << std::endl;
		}
		closesocket(m_server_socket);
		return false;
	}
	else std::cout << "Binding of TCP-socket is successful.\n";

	if (listen(m_server_socket, 1) == SOCKET_ERROR) {
		std::cout << "Listen Error : " << WSAGetLastError() << std::endl;
		closesocket(m_server_socket);
		return false;
	}
	else std::cout << "Listening...\n";
	return true;
};
bool Server::ServerStop() {
	if (closesocket(m_server_socket) == SOCKET_ERROR) {
		if (WSAGetLastError() == 10038) {
			std::cout << "Socket has already beed CLOSED (or invalid).\n";
		}
		std::cout << "Close Error : " << WSAGetLastError();
	}
	std::cin.get();
	WSACleanup();
	return true;
};
void Server::ServerStartAcception() {
	int addr_accept_len = sizeof(m_client_addr);
	m_client_socket = accept(m_server_socket, (sockaddr*)&m_client_addr, &addr_accept_len);
	if (m_client_socket == INVALID_SOCKET) {
		std::cout << "Accept Error : " << WSAGetLastError() << std::endl;
		closesocket(m_server_socket);
		return;
	}
	else {
		std::cout << "Accepted.\n";
		SendFile();
	}
	if (WSAAsyncSelect(m_server_socket, (HWND)GetCurrentProcess(), WSA_ACCEPT, FD_ACCEPT) > 0) {
		std::cout << "WSAAsyncSelect Error : " << WSAGetLastError() << std::endl;
		return;
	}
};

void Server::SendFile() {
	static std::streampos position = 0;
	static int sum = 0;
	int request_size = 0;
	file.open("text_server.txt", std::ios::binary | std::ios::ate | std::ios::in);
	if (file.is_open()) {
		// searcing begining of the file and its size
		file.seekg(0, file.end);
		long filesize = file.tellg();

		if (filesize % PARTSIZE == 0) partnum = filesize / PARTSIZE;
		else partnum = (filesize / PARTSIZE) + 1;

		//reading .png from file stream in buffer
		char* buffer = new char[PARTSIZE];
		for (unsigned int i = 0; i < partnum; ++i) {

			if (i != (partnum - 1)) request_size = PARTSIZE;
			else  request_size = filesize - (i * PARTSIZE);

			file.seekg(position);
			file.read(buffer, request_size);
			position += request_size;
			//sending binary to client
			m_iResult = send(m_client_socket, buffer, request_size, 0);

			if (m_iResult == SOCKET_ERROR) {
				std::cout << "Send is failed. Error :" << WSAGetLastError() << std::endl;
				delete[] buffer;
				return;
			}
			else {
				std::cout << "Sent bytes : " << m_iResult << std::endl;
				sum += m_iResult;
			}
		}
		std::cout << "Total bytes : " << sum << std::endl;
		m_iResult = shutdown(m_client_socket, SD_SEND);
		if (m_iResult == SOCKET_ERROR)
		{
			std::cout << "Shutdown is failed. Error : " << WSAGetLastError() << std::endl;
			closesocket(m_client_socket);
			delete[] buffer;
			return;
		}
		else delete[] buffer;
	}
	else std::cout << "File didn't open. Error : " << GetLastError() << std::endl;
	file.close();
}
bool Server::isRunning() {
	return this->m_Running;
}