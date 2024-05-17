#include "../inc/Channel.hpp"


std::map <std::string, Channel*> Channel::channels = std::map <std::string, Channel*>();

Channel* Channel::getChannel(std::string name)
{
	if (channels.find(name) != channels.end())
		return channels[name];
	else
		return NULL;
}

Channel::Channel(std::string name)
{
    this->clients = std::vector<std::string>();
    this->name = name;

	channels[name] = this;
}

Channel::~Channel(){}

Channel::Channel(const Channel& src)
{
    *this = src;
}

bool Channel::addClient(std::string name)
{
	std::vector<std::string>::iterator it = std::find(clients.begin(), clients.end(), name);
	if (clients.empty())
	{
		this->clients.push_back(name);
		return true;
	}
	if (it != clients.end())
    	this->clients.push_back(name);
	else
		return false; // Client is already in the Channel
	return true;
}

Channel& Channel::operator=(const Channel& src)
{
    this->clients = src.clients;
    this->name = src.name;
    return *this;
}

void Channel::broadcastAll(Client &sender, std::string message)
{
	for (std::vector<std::string>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		std::string c = (*it);
		if (c != sender.getName())
		{
			send(sender.get_fd(), message.c_str(), 1024, 0);
		}
	}
}