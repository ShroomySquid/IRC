#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"
#include "../inc/Command.hpp"

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

void process_message(std::map<std::string, Command*>& commands, char *buffer)
{
	std::string cmd; // nom de la commande.
	std::vector<std::string> args; // arguments de la commande.
	//cout << "Client send: " << buffer << std::endl;
	int i = 0;
	while (buffer[i] && buffer[i] != ' ' && buffer[i] != '\n')
	{
		cmd += buffer[i];
		i++;
	}
	if (buffer[i] == ' ')
		i++;
	while(buffer[i] && buffer[i] != '\n')
	{
		std::string arg;
		while (buffer[i] && buffer[i] != ' ')
		{
			arg += buffer[i];
			i++;
		}
		i++;
		args.push_back(arg);
	}

	if (commands.find(cmd) != commands.end())
		commands[cmd]->execute(args); // execute la commande
	else
		std::cout << "This command doesnt exist !" << std::endl;
}

int server(int socketD, struct sockaddr_in *address, std::string password) {

	std::map<std::string, Command*> commands;
	commands["JOIN"] = new Cmd_join();
	commands["KICK"] = new Cmd_kick();
	commands["PRIVMSG"] = new Cmd_privmsg();
	(void)password;

	bool is_online = true;
	std::map<std::string, Client*> clients;
	int infd;
	char buffer[1024];
	Client* received_client;
	bzero(buffer, 1024);
	struct sockaddr_in clientAddress;

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
				clients.insert(std::pair<std::string, Client*> 
					(received_client->get_username(), received_client));
				cout << "Client " << received_client->get_username();
				cout << " connected." << endl;
			}
		}
		if (clients.empty())
			continue ;
		for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
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
				broadcastAll(clients, it->first, buffer);
				//process_message(commands, buffer);
				bzero(buffer, 1024);
			}
			else
			{
				it->second->disconnect();
				cout << "closing client on fd" << it->second->get_fd() << endl;
				close(it->second->get_fd());
			}
		}
		for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end();)
		{
			if (it->second->is_disconnected()) {
				close(it->second->get_fd());
				delete it->second;
				it = clients.erase(it);
			}
			else
				it++;
		}
	}
	if (!clients.empty())
	{
		for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			close(it->second->get_fd());
			delete it->second;
		}
	}
	//deleting all command instances
	std::map<std::string, Command*>::iterator it = commands.begin();
	while (it != commands.end())
	{
		delete (*it).second;
		it++;
	}
	commands.clear();
	return (0);
}

int main(int argc, char** argv)
{
	
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
	server(socketD, address, "");
	free(address);
	close(socketD);
	return (0);
}
