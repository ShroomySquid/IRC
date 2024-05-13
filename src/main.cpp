#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"
#include <fcntl.h>
//#include "Client.hpp"
#include <vector>

int test_server(int socketD, struct sockaddr_in *address) {
	bool is_online = true;
	std::vector<Client> client_container;
	//std::vector<int>::iterator to_erase;

	int infd;
	char buffer[1024];
	bzero(buffer, 1024);
	struct sockaddr_in clientAddress;

	//int flags = fcntl(socketD, F_GETFL, 0);
	fcntl(socketD, F_SETFL, O_NONBLOCK);

	unsigned int clientAddressSize = sizeof(clientAddress);
	int bind_result = bind(socketD, (struct sockaddr *)address, sizeof(*address));

	if (bind_result != 0) {
		cout << "bind failed." << endl;
		return (1);
	}
	cout << "Waiting for client to connect..." << endl;
	if (listen(socketD, 10) == -1) {
		cout << "listen failed." << endl;
		return (1);
	}

	while (is_online)
	{
		infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
		if (infd > 0)
		{
			Client c = Client(infd);
			client_container.push_back(c);
			std::cout << "Client connect !!!!" << std::endl;
		}
		if (!client_container.empty())
		{
			for (std::vector<Client>::iterator it = client_container.begin(); it != client_container.end(); it++)
			{
				//cout << "for loop looping..." << endl;
				if (recv((*it).get_fd(), buffer, 1024, 0) != -1)
				{
					if (buffer[0] != '\0')
					{
						cout << "Client send: " << buffer << endl;
						send((*it).get_fd(), buffer, 1024, 0);
						bzero(buffer, 1024);
					}
					else
					{
						(*it).disconnect();
						std::cout << "closing client on fd" << (*it).get_fd() << std::endl;
						close((*it).get_fd());
					}
				}
			}
			for (std::vector<Client>::reverse_iterator it = client_container.rbegin(); it != client_container.rend();) {
				if ((*it).is_disconnected()) {
					// Supprimer l'élément et mettre à jour l'itérateur
					it = std::vector<Client>::reverse_iterator(client_container.erase(std::next(it).base()));
				} else {
					// Passer à l'élément suivant
					++it;
				}
			}
		}

	}
	for (std::vector<Client>::iterator it = client_container.begin(); it != client_container.end(); it++)
	{
		close((*it).get_fd());
	}
	return (0);
}

int main(int argc, char** argv) {
	/*
	if (argc != 3)
	{
		cerr << "Invalid amount of arguments provided. Please provide a port";
		cerr << " and a server password." << endl;
		return (1);
	}
	*/
	(void)argc;
	(void)argv;
	int socketD = create_socket_descriptor();
	if (socketD == -1)
		return (1);
	char address_str[] = "";
	struct sockaddr_in* address = set_address(address_str, 2000);
	if (address == NULL)
		return (1);
	test_server(socketD, address);
	free(address);
	close(socketD);
	return (0);
}
