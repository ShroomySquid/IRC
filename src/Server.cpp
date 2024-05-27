#include "../inc/Server.hpp"
#include "../inc/Command.hpp"

Server::Server(int socketD, struct sockaddr_in *address, std::string password)
{
	channels = std::map <std::string, Channel*>();
    need_to_remove_client = false;
    this->socketD = socketD;
    this->address = address;
    this->password = password;
    initializeCommands();
    initializeBindings(socketD, address);
	process_called = 0;
}

void Server::initializeCommands() {
    DEBUG_PRINT("Initializing commands");
    commands["JOIN"] = new Cmd_join();
    commands["KICK"] = new Cmd_kick();
    commands["PRIVMSG"] = new Cmd_privmsg();
    commands["PASS"] = new Cmd_pass();
    commands["USER"] = new Cmd_user();
    commands["NICK"] = new Cmd_nick();
    commands["TOPIC"] = new Cmd_topic();
    commands["MODE"] = new Cmd_mode();
	commands["INVITE"] = new Cmd_invite();
}

void Server::initializeBindings(int socketD, struct sockaddr_in *address) {
    DEBUG_PRINT("Initializing bindings");
    bzero(buffer, 1024);
    fcntl(socketD, F_SETFL, O_NONBLOCK);
    clientAddressSize = sizeof(clientAddress);
    int bind_result = bind(socketD, (struct sockaddr *)address, sizeof(*address));
    if (bind_result != 0) {
        cout << "bind failed." << endl;
        throw std::exception();
    }
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
	{
		return true; 	
	}
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

Client* Server::find_client(std::string client_name) {
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (it->second->get_nickname().compare(client_name))
			return (it->second);
	}
	return (NULL);
}
