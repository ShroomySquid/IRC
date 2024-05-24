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
	commands["PASS"] = new Cmd_pass();
	commands["USER"] = new Cmd_user();
	commands["NICK"] = new Cmd_nick();
	commands["TOPIC"] = new Cmd_topic();
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
		std::cout << cmd << " : Command not found" << std::endl;
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

    if (message.length() >= 2 && end == "\r\n") 
        return true; 
	return false;
}

void Server::remove_client(std::map<int, Client*> &clients) {
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end();)
	{
		if (!it->second->is_disconnected()) {
			++it;
			continue;
		}
		for (std::map<std::string, Channel*>::iterator i = channels.begin(); i != channels.end(); i++) {
			i->second->removeClient(it->second);
		}
		close(it->second->get_fd());
		delete it->second;
		std::map<int, Client*>::iterator to_erase = it;
		++it;
		clients.erase(to_erase);
	}
}


// void Server::Run()
// {
// 	cout << "Waiting for clients to connect..." << endl;
// 	if (listen(socketD, 10) == -1) {
// 		cout << "listen failed." << endl;
// 		throw std::exception();
// 	}

// 	while (true)
// 	{
// 		infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
// 		if (infd > 0)
// 			login_attempt(clients, infd);
// 		if (clients.empty())
// 			continue ;
// 		for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
// 		{
// 			if (it->second->is_disconnected())
// 				continue;
// 			pfd.fd = it->second->get_fd();
// 			pfd.events = POLLIN | POLLOUT;
// 			poll(&pfd, 1, 0);
// 			int bytesReceived = recv(pfd.fd, buffer, 1024, 0);
// 			if (!pfd.revents || bytesReceived <= 2)
// 				continue ;
// 			if (buffer[0] != '\0')
// 			{
// 				std::string receivedData;
// 				receivedData.append(buffer, bytesReceived);
// 				if (!is_IRC_message(receivedData))
// 				{
// 					cout << " client send invalid data: " << receivedData;
// 					std::cout << "Not an IRC message !" << std::endl;
// 					bzero(buffer, 1024);
// 					continue;
// 				}
// 				receivedData.erase(receivedData.length() - 2, 2);
// 				cout << "Client " << it->second->get_username();
// 				cout << " send: " << buffer;
// 				process_message(*this ,*(it->second),commands, receivedData);
// 				bzero(buffer, 1024);
// 			}
// 			else
// 			{
// 				it->second->disconnect();
// 				need_to_remove_client = true;
// 				cout << "closing client on fd" << it->second->get_fd() << endl;
// 				close(it->second->get_fd());
// 			}
// 		}
// 		if (need_to_remove_client)
// 			remove_client(clients);
// 		need_to_remove_client = false;
// 	}
// }


void Server::AcceptClients() {
    DEBUG_PRINT("Accepting clients...");
    int infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (infd > 0)
        login_attempt(clients, infd);
}


void Server::CreatePollfds(std::vector<pollfd>& pfds) {
    pollfd listeningSocket;
    listeningSocket.fd = socketD;
    listeningSocket.events = POLLIN;
    listeningSocket.revents = 0;
    pfds.push_back(listeningSocket);

    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (!it->second->is_disconnected()) {
            pollfd pfd;
            pfd.fd = it->second->get_fd();
            pfd.events = POLLIN | POLLOUT;
            pfd.revents = 0;
            pfds.push_back(pfd);
        }
    }
}

void Server::PollAndProcessClients() {
    std::vector<pollfd> pfds;
    CreatePollfds(pfds);
    int poll_count = poll(&pfds[0], pfds.size(), 0); // 0 seconds means poll will return immediately without blocking.

    if (poll_count < 0) {
        perror("poll");
        return;
    }

    for (std::vector<pollfd>::iterator pfd_it = pfds.begin(); pfd_it != pfds.end(); ++pfd_it) {
        if (pfd_it->fd == socketD && pfd_it->revents & POLLIN) {
            AcceptClients();
        } else if (pfd_it->revents & POLLIN) {
            ProcessClientMessage(*pfd_it);
        }
    }
}

void Server::ProcessClientMessage(const pollfd& pfd) {
    int bytesReceived = recv(pfd.fd, buffer, 1024, 0);
    if (bytesReceived <= 0) {
        std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            it->second->disconnect();
            need_to_remove_client = true;
        }
        return;
    }

    if (buffer[0] != '\0') {
        std::string receivedData(buffer, bytesReceived);
        if (!is_IRC_message(receivedData)) {
            cout << "Client sent invalid data: " << receivedData << endl;
            bzero(buffer, 1024);
            return;
        }
        receivedData.erase(receivedData.length() - 2, 2);
        std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            cout << buffer;
            process_message(*this, *(it->second), commands, receivedData);
        }
        bzero(buffer, 1024);
    }
}

void Server::MarkAndRemoveDisconnectedClients() {
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ) {
        if (it->second->is_disconnected()) {
            DEBUG_PRINT("Closing client on fd " << it->second->get_fd());
            close(it->second->get_fd());
            delete it->second;
            std::map<int, Client*>::iterator to_erase = it++;
            clients.erase(to_erase);
        } else {
            ++it;
        }
    }
}

void Server::ListenClients() {
    DEBUG_PRINT("Waiting for clients to connect...");
    if (listen(socketD, 10) == -1) {
        cout << "listen failed." << endl;
        throw std::exception();
    }
}

void Server::Run() {
    DEBUG_PRINT("Running server...");
    ListenClients();
    while (true) {
        PollAndProcessClients();
        MarkAndRemoveDisconnectedClients();
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
	channels[name] = c;
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

std::string Server::get_password() const
{
	return (password);
}

std::map<int, Client*>& Server::get_clients()
{
	return (clients);
}
