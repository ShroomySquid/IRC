#include "../inc/Channel.hpp"

// -------------Channel Class --------------
Channel::Channel(std::string name)
{
    this->members = std::vector<Client*>();
    this->name = name;
}
Channel::~Channel(){}
Channel::Channel(const Channel& src)
{
    *this = src;
}

bool Channel::addClient(Client* c)
{
	std::vector<Client*>::iterator it = std::find(members.begin(), members.end(), c);
	if (members.empty())
	{
		this->members.push_back(c);
		return true;
	}
	if (it == members.end())
    	this->members.push_back(c);
	else
		return false; // Client is already in the Channel
	return true;
}

Channel& Channel::operator=(const Channel& src)
{
    this->members = src.members;
    this->name = src.name;
    return *this;
}

// TODO
// check si la personne fais partie du channel
// check si pas le meme
// recupere le client dans la map et envoi le message

void Channel::broadcastAll(Client &sender, std::string message)
{
	(void) sender; // test
	for (std::vector<Client*>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		Client* c = (*it);
		//if (c != &sender)
		{
			send(c->get_fd(), message.c_str(), message.size(), 0);
		}
	}
}