#include "../inc/Server.hpp"
#include "../inc/Command.hpp"

bool online = true;

Server::Server(int socketD, struct sockaddr_in *address, std::string password)
{
	channels = std::map <std::string, Channel*>();
    need_to_remove_client = false;
    this->socketD = socketD;
    this->address = address;
    this->password = password;
	buffer_len = 0;
    initializeCommands();
    initializeBindings(socketD, address);
	initSignals();
	bzero(buffer, 1024);
	bzero(recv_buffer, 1024);
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
    commands["PART"] = new Cmd_part();
	commands["INVITE"] = new Cmd_invite();
	commands["CAP"] = new Cmd_cap();
	commands["PING"] = new Cmd_ping();
	commands["QUIT"] = new Cmd_quit();
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

	if (message.length() < 2)
		return false;
	std::string end = message.substr(message.length() - 2,2);
    if (end == "\r\n")
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
std::map <std::string, Channel*> Server::get_all_channels()
{
	return this->channels;
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
		if (!it->second->get_client().compare(client_name))
			return (it->second);
	}
	return (NULL);
}

void Server::initSignals()
{
	signal(SIGINT, Server::handleSignal);
	signal(SIGTERM, Server::handleSignal);
	signal(SIGQUIT, Server::handleSignal);
}

void Server::handleSignal(int signal)
{
	if (signal == SIGINT || signal == SIGTERM || signal == SIGQUIT)
		online = false;
}
