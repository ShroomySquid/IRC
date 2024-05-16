#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"

void broadcastAll(std::map<std::string, Client*>& clients, std::string except, char *buffer)
{
	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->first != except)
			send(it->second->get_fd(), buffer, 1024, 0);
	}
}

Client* new_client(int fd, std::string username, std::string nickname) {
	Client* new_client = new Client(fd, username, nickname);
	return (new_client);
}

int server(int socketD, struct sockaddr_in *address) {
	bool is_online = true;
	std::map<std::string, Client*> client_container;
	int infd;
	char buffer[1024];
	Client* received_client;
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
		// registration qui va retourner un nouveau client?
		infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
		if (infd > 0)
		{
			received_client = new_client(infd, "user", "nick");
			if (received_client != NULL) {
				client_container.insert(std::pair<std::string, Client*> (received_client->get_username(), received_client));
				std::cout << "Client " << received_client->get_username() << " connected." << std::endl;
			}
		}
		if (client_container.empty())
			continue ;
		for (std::map<std::string, Client*>::iterator it = client_container.begin(); it != client_container.end(); it++)
		{
			if (it->second->is_disconnected())
				continue;
			pollfd pfd;
			pfd.fd = it->second->get_fd();
			pfd.events = POLLIN | POLLOUT;
			poll(&pfd, 1, 0);
			if (!pfd.revents || recv(pfd.fd, buffer, 1024, 0) == -1)
				continue ;
			cout << "salut" << endl;
			if (buffer[0] != '\0')
			{
				cout << "Client send: " << buffer;
				broadcastAll(client_container, it->first, buffer);
				bzero(buffer, 1024);
			}
			else
			{
				it->second->disconnect();
				std::cout << "closing client on fd" << it->second->get_fd() << std::endl;
				close(it->second->get_fd());
			}
		}
	}
	for (std::map<std::string, Client*>::iterator it = client_container.begin(); it != client_container.end(); it++)
	{
		close(it->second->get_fd());
		delete it->second;
	}
	return (0);
}

int main(int argc, char** argv) {
	
	// if (argc != 3)
	// {
	// 	cerr << "Invalid amount of arguments provided. Please provide a port";
	// 	cerr << " and a server password." << endl;
	// 	return (1);
	// }
	
	(void)argc;
	(void)argv;
	int socketD = create_socket_descriptor();
	if (socketD == -1)
		return (1);
	char address_str[] = "";
	struct sockaddr_in* address = set_address(address_str, 2000);
	if (address == NULL)
		return (1);
	server(socketD, address);
	free(address);
	close(socketD);
	return (0);
}
