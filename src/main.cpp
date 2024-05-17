#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"
#include "../inc/Command.hpp"

void finish_server(std::map<std::string, Client*> &clients, std::map<std::string, Command*> &commands) {
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
	Channel::free_channel();

}

void remove_client(std::map<std::string, Client*> &clients) {
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

int server(int socketD, struct sockaddr_in *address, std::string password) {
	std::map<std::string, Command*> commands;
	commands["JOIN"] = new Cmd_join();
	commands["KICK"] = new Cmd_kick();
	commands["PRIVMSG"] = new Cmd_privmsg();

	bool is_online = true;
	bool need_to_remove_client = false;
	std::map<std::string, Client*> clients;
	pollfd pfd;
	int infd;
	char buffer[1024];
	bzero(buffer, 1024);
	struct sockaddr_in clientAddress;

	fcntl(socketD, F_SETFL, O_NONBLOCK);
	unsigned int clientAddressSize = sizeof(clientAddress);
	int bind_result = bind(socketD, (struct sockaddr *)address, sizeof(*address));
	if (bind_result != 0) {
		cout << "bind failed." << endl;
		return (1);
	}
	cout << "Waiting for clients to connect..." << endl;
	if (listen(socketD, 10) == -1) {
		cout << "listen failed." << endl;
		return (1);
	}

	while (is_online)
	{
		infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
		if (infd > 0)
			login_attempt(clients, infd, password);
		if (clients.empty())
			continue ;
		for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (it->second->is_disconnected())
				continue;
			pfd.fd = it->second->get_fd();
			pfd.events = POLLIN | POLLOUT;
			poll(&pfd, 1, 0);
			if (!pfd.revents || recv(pfd.fd, buffer, 1024, 0) == -1)
				continue ;
			if (buffer[0] != '\0')
			{
				cout << "Client send: " << buffer;
				//broadcastAll(clients, it->first, buffer);
				process_message(*(it->second) ,commands, buffer);
				bzero(buffer, 1024);
			}
			else
			{
				it->second->disconnect();
				need_to_remove_client = true;
				cout << "closing client on fd" << it->second->get_fd() << endl;
				close(it->second->get_fd());
			}
		}
		if (need_to_remove_client)
			remove_client(clients);
		need_to_remove_client = false;
	}
	finish_server(clients, commands);
	return (0);
}

int check_port(std::string port) {
	int i = 0;
	while (port[i]) {
		if (!isdigit(port[i])) {
			cout << "Invalid character in port. Please only put digits." << endl;
			return (-1);
		}
		i++;
	}
	if (i > 5) {
		cout << "Invalid port : port too big" << endl;
		return (-1);
	}
	try {
		int port_int = std::stoi(port);
		if (port_int > 65535 || port_int < 1001) {
			cout << "Invalid port." << endl;
			return (-1);
		}
		return (port_int);
	}
	catch (...) {
		cout << "error with stoi." << endl;
		return (-1);
	}
	return (-1);
}

bool invalid_password(std::string password) {
	if (password.length() < 3 || password.length() > 30) {
		cout << "Invalid password size. Please provide a password with more";
		cout << " than 3 character and less than 30." << endl;
		return (true);
	}
	return (false);
}

int main(int argc, char** argv)
{
	
	if (argc != 3)
	{
		cerr << "Invalid amount of arguments provided. Please provide a port";
		cerr << " and a server password." << endl;
	 	return (1);
	}
	int port = check_port(argv[1]);
	if (port < 0 || invalid_password(argv[2])) {
		return (1);
	}
	int socketD = create_socket_descriptor();
	if (socketD == -1)
		return (1);
	char address_str[] = "";
	struct sockaddr_in* address = set_address(address_str, port);
	if (address == NULL)
		return (1);
	server(socketD, address, "");
	free(address);
	close(socketD);
	return (0);
}
