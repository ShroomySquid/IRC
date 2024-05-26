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
}

void Server::initializeCommands() {
    DEBUG_PRINT("Initializing commands");
    commands["JOIN"] = new Cmd_join();
    commands["KICK"] = new Cmd_kick();
    commands["PRIVMSG"] = new Cmd_privmsg();
    commands["PASS"] = new Cmd_pass();
    commands["USER"] = new Cmd_user();
    commands["NICK"] = new Cmd_nick();
	commands["CAP"] = new Cmd_cap();
	// commands["PING"] = new Cmd_ping();
	// commands["QUIT"] = new Cmd_quit();
}

void Server::initializeBindings(int socketD, struct sockaddr_in *address) {
    DEBUG_PRINT("Initializing bindings");
    bzero(buffer, 1024);
    fcntl(socketD, F_SETFL, O_NONBLOCK);
    clientAddressSize = sizeof(clientAddress);

    int bind_result = bind(socketD, (struct sockaddr *)address, sizeof(*address));
    if (bind_result != 0) {
        std::ostringstream error_msg;
		error_msg << "Error binding socket: " << strerror(errno);
        close(socketD);
        throw std::runtime_error(error_msg.str());
    }
}

// login_attempt replaced with registerClient
// void Server::login_attempt(std::map<int, Client*> &clients, int infd)
// {
// 	Client* received_client = new Client(infd);
// 	if (received_client != NULL) {
// 		clients.insert(std::pair<int, Client*>
// 				(received_client->get_fd(), received_client));
// 	}
// }

void Server::registerClient(std::map<int, Client*> &clients, Client* newClient)
{
	if (newClient != NULL) {
		clients.insert(std::pair<int, Client*>
				(newClient->get_fd(), newClient));
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
