#include "Client.h"
#include "Server.h"
#include <iostream>

int main() {
	Server server;
	Client client;
	int user = 0;
	std::cout << "What type of service do you need to provide(Server(1)/Client(2)):";
	std::cin >> user;
	char user_access = 'A';
	switch (user) {
	case 1:
		if (server.ServerStart()) {
			std::cout << "Server established.\n";
			std::cout << "Starting accessing.\n";
			server.ServerStartAcception();
		}
		else std::cout << "Error : Establishing is aborted.\n";
		break;
	case 2:
		if (client.ClientStart()) {
			std::cout << "Client started.\n";
			char user_address[20] = "";
			int user_port = 0;
			std::cout << "Enter IP-Address: ";
			std::cin >> user_address;
			std::cout << "Enter Port: ";
			std::cin >> user_port;
			std::cout << "Trying to connect(" << user_address << " : " << user_port << ")...\n";
			if (client.Connect(user_port, user_address)) {
				std::cout << "Connection(" << user_address << " : " << user_port << ") is established.\n";
				std::cout << "Recieving datas...\n";
				while (client.RecieveFile());
			} else std::cout << "Connection(" << user_address << " : " << user_port << ") is aborted.\n";
		}
		else std::cout << "Error : Creating is aborted.\n";
		break;
	default:
		std::cout << "This type of service is not provided.";
		break;
	}
	system("pause");
	if (user == 1) server.ServerStop();
	else if (user == 2) client.Disconnect();
	else return 1;
	return 0;
}