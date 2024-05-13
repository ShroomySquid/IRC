#include "../inc/IRC.hpp"
#include <fcntl.h>
//#include "Client.hpp"
#include <vector>

int test_server(int socketD, struct sockaddr_in *address) {
	bool is_online = true;
	std::vector<int> client_FDS;

	int infd;
	char buffer[1024];
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
			//Client c = Client(infd);
			client_FDS.push_back(infd);
			std::cout << "Client connect !!!!" << std::endl;
		}
		if (!client_FDS.empty())
		{
			for (std::vector<int>::iterator it = client_FDS.begin(); it != client_FDS.end(); it++)
			{
				//cout << "for loop looping..." << endl;
				if (recv(*it, buffer, 1024, 0) == -1)
				{
					//cout << ""
					//cerr << "recv failed: " << std::strerror(errno) << endl;
					//close((*it));
				}
				else
				{
					if (buffer[0] != '\0')
					{
						cout << "Client send: " << buffer << endl;
						//cout << "Client send: " << buffer << endl;
						bzero(buffer, 1024);
					}
					else
					{
						close(*it);
						client_FDS.erase(it);
					}
				}
			}
		}
		
	}
	for (std::vector<int>::iterator it = client_FDS.begin(); it != client_FDS.end(); it++)
	{
		close(*it);
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
