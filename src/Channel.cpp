#include "../inc/Channel.hpp"

// -------------Channel Class --------------
Channel::Channel(std::string name)
{
    this->members = std::vector<Client*>();
    this->operators = std::vector<Client*>();
    this->name = name;
}
Channel::~Channel(){}
Channel::Channel(const Channel& src)
{
    *this = src;
}

bool Channel::promote(Client* c) {	
	if (members.empty())
		return (false);
	std::vector<Client*>::iterator it = std::find(members.begin(), members.end(), c);
	if (it == members.end())
		return (false);
	this->operators.push_back(c);	
	this->members.erase(it);
	return true;
}

bool Channel::is_member(Client* c) 
{
	if (members.empty())
		return (false);
	std::vector<Client*>::iterator it = std::find(members.begin(), members.end(), c);
	if (it == members.end())
		return (false);
	return (true);
}

bool Channel::is_operator(Client* c) 
{
	if (operators.empty())
		return (false);
	std::vector<Client*>::iterator it = std::find(operators.begin(), operators.end(), c);
	if (it == operators.end())
		return (false);
	return (true);
}

bool Channel::addClient(Client* c, bool ope)
{
	if (!ope) 
	{
		if (!is_member(c)) {
			this->members.push_back(c);
			return true;
		}
		return false;
	}
	if (!is_operator(c)) {
		this->operators.push_back(c);
		return true;
	}
	return false;
}

Channel& Channel::operator=(const Channel& src)
{
    this->members = src.members;
    this->operators = src.operators;
    this->name = src.name;
    return *this;
}

// TODO
// check si pas le meme
// recupere le client dans la map et envoi le message

void Channel::broadcastAll(Client &sender, std::string message)
{
	if (!is_member(&sender) && !is_operator(&sender)) {
		cout << "Not part of the channel.\n" << endl;
	}
	for (std::vector<Client*>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		if (*it != &sender)
			send(sender.get_fd(), message.c_str(), message.size(), 0);
	}
	for (std::vector<Client*>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{
		if (*it != &sender)
			send(sender.get_fd(), message.c_str(), message.size(), 0);
	}
}
