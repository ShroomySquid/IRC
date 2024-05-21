#include "../inc/Server.hpp"
#include "../inc/Command.hpp"


Server::Server(int socketD, struct sockaddr_in *address, std::string password)
{
	channels = std::map <std::string, Channel*>();
    need_to_remove_client = false;
    this->socketD = socketD;
    this->address = address;
    this->password = password;

	commands["JOIN"] = new Cmd_join();
	commands["KICK"] = new Cmd_kick();
	commands["PRIVMSG"] = new Cmd_privmsg();
	bzero(buffer, 1024);
	fcntl(socketD, F_SETFL, O_NONBLOCK);
	clientAddressSize = sizeof(clientAddress);
	int bind_result = bind(socketD, (struct sockaddr *)address, sizeof(*address));
	if (bind_result != 0) {
		cout << "bind failed." << endl;
		throw std::exception();
	}

}

void Server::process_message(Server &server, Client &sender, std::map<std::string, Command*>& commands, std::string input)
{
	std::string cmd; // nom de la commande.
	std::vector<std::string> args; // arguments de la commande.
    char *token = std::strtok((char *)input.c_str(), " ");
	cmd = token;
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        args.push_back(token);
        token = std::strtok(NULL, " ");
    }
	if (commands.find(cmd) != commands.end())
		commands[cmd]->execute(server, sender, args); // execute la commande
	else
		std::cout << "This command doesnt exist !" << std::endl;
}

void Server::login_attempt(std::map<int, Client*> &clients, int infd)
{
	Client* received_client = new Client(infd);
	// error if new fail ?
	if (received_client != NULL) {
		clients.insert(std::pair<int, Client*> 
				(received_client->get_fd(), received_client));
	}
}

bool Server::is_IRC_message(const std::string& message)
{
    // Vérifie si la chaîne se termine par "\r\n"
	// avec netcat (ctrl v ctrl m enter)
	std::string end = message.substr(message.length() - 2,2);

    if (message.length() >= 2 && end == "\r\n") {
        return true;
    } else {
        return false;
    }
	return false;
}

void Server::remove_client(std::map<int, Client*> &clients) {
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end();)
	{
		if (it->second->is_disconnected()) {
			close(it->second->get_fd());
			delete it->second;
			std::map<int, Client*>::iterator to_erase = it;
			++it;
			clients.erase(to_erase);
		}
		else
			++it;
	}
}


void Server::Run()
{
	cout << "Waiting for clients to connect..." << endl;
	if (listen(socketD, 10) == -1) {
		cout << "listen failed." << endl;
		throw std::exception();
	}

	while (true)
	{
		infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
		if (infd > 0)
			login_attempt(clients, infd);
		if (clients.empty())
			continue ;
		for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (it->second->is_disconnected())
				continue;
			pfd.fd = it->second->get_fd();
			pfd.events = POLLIN | POLLOUT;
			poll(&pfd, 1, 0);
			int bytesReceived = recv(pfd.fd, buffer, 1024, 0);
			if (!pfd.revents || bytesReceived <= 1)
				continue ;
			if (buffer[0] != '\0')
			{
				std::string receivedData;
				receivedData.append(buffer, bytesReceived);
				if (!is_IRC_message(receivedData))
				{
					cout << " client send: " << receivedData;
					std::cout << "Not an IRC message !" << std::endl;
					bzero(buffer, 1024);
					continue;
				}
				receivedData.pop_back();
				receivedData.pop_back();


				//if (!it->second->is_registered()) 
				//	registration((*(it->second)), password, buffer, clients);
				//else 
				//{
				cout << "Client " << it->second->get_username();
				cout << " send: " << buffer;
				//broadcastAll(clients, it->first, buffer);
				process_message(*this ,*(it->second),commands, receivedData);
				//}
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
}

// --------------- Channels methods ----------------
Channel* Server::getChannel(std::string name)
{
	if (channels.find(name) != channels.end())
		return channels[name];
	else
		return NULL;
}

void Server::addChannel(std::string name,Channel *c)
{
	channels.at(name) = c;
}

void Server::free_channel()
{
	//deleting all channel instances
	std::map<std::string, Channel*>::iterator it = channels.begin();
	while (it != channels.end())
	{
		delete (*it).second;
		it++;
	}
}
// -------------------------------------------

Server::~Server()
{
	if (!clients.empty())
	{
		for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
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
	free_channel();
}