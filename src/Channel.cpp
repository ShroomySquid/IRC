#include "../inc/Channel.hpp"

// -------------Channel Class --------------
Channel::Channel(std::string name)
{
	this->topic = "";
    this->members = std::vector<Client*>();
    this->operators = std::vector<Client*>();
    this->name = name;
	this->topic_protection = true;
	this->on_invite = false;
	this->password = "";
	this->limit = -1;
}
Channel::~Channel(){}
Channel::Channel(const Channel& src)
{
    *this = src;
}

bool Channel::promote(Client* c) {	
	if (members.empty() || !c)
		return (false);
	std::vector<Client*>::iterator it = std::find(members.begin(), members.end(), c);
	if (it == members.end())
		return (false);
	this->operators.push_back(c);	
	this->members.erase(it);
	return true;
}

bool Channel::demote(Client* c) {	
	if (operators.empty() || !c)
		return (false);
	std::vector<Client*>::iterator it = std::find(operators.begin(), operators.end(), c);
	if (it == operators.end())
		return (false);
	this->members.push_back(c);	
	this->operators.erase(it);
	return true;
}

bool Channel::is_member(Client* c) 
{
	if (members.empty() || !c)
		return (false);
	std::vector<Client*>::iterator it = std::find(members.begin(), members.end(), c);
	if (it == members.end())
		return (false);
	return (true);
}

bool Channel::is_operator(Client* c) 
{
	if (operators.empty() || !c)
		return (false);
	std::vector<Client*>::iterator it = std::find(operators.begin(), operators.end(), c);
	if (it == operators.end())
		return (false);
	return (true);
}

bool Channel::addClient(Client* c, bool ope)
{
	if (is_member(c) || is_operator(c))
		return false;
	if (!ope) 
	{
		this->members.push_back(c);
		return true;
	}
	this->operators.push_back(c);
	return true;
}

Client * Channel::getMember_by_name(std::string name)
{
	std::vector<Client*>::iterator it;
	for (it = this->members.begin(); it != this->members.end(); it++)
	{
		Client *c = (*it);
		if (c->get_username() == name)
			return c;
	}
	for (it = this->operators.begin(); it != this->operators.end(); it++)
	{
		Client *o = (*it);
		if (o->get_username() == name)
			return o;
	}
	return NULL;
}

void Channel::removeClient(Client *c)
{
	if (!members.empty()) {
		std::vector<Client*>::iterator it = std::find(members.begin(), members.end(), c);
		if (it != members.end())
			this->members.erase(it);
	}
	if (!operators.empty()) {
		std::vector<Client*>::iterator i = std::find(operators.begin(), operators.end(), c);
		if (i != operators.end())
			this->operators.erase(i);
	}
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
		return;
	}
	for (std::vector<Client*>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		if (*it != &sender)
			send((*it)->get_fd(), message.c_str(), message.size(), 0);
	}
	for (std::vector<Client*>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{
		if (*it != &sender)
			send((*it)->get_fd(), message.c_str(), message.size(), 0);
	}
	//std::cout << "Message sent to channel: " << arguments.at(2) << std::endl;
}

void Channel::broadcastCmd(std::string cmd, std::string arg) {
	for (std::vector<Client*>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		send((*it)->get_fd(), cmd.c_str(), cmd.size(), 0);
		send((*it)->get_fd(), ": ", 2, 0);
		send((*it)->get_fd(), arg.c_str(), arg.size(), 0);
		send((*it)->get_fd(), "\n", 1, 0);
	}
	for (std::vector<Client*>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{
		send((*it)->get_fd(), cmd.c_str(), cmd.size(), 0);
		send((*it)->get_fd(), ": ", 2, 0);
		send((*it)->get_fd(), arg.c_str(), arg.size(), 0);
		send((*it)->get_fd(), "\n", 1, 0);
	}
}

std::string Channel::get_topic() {
	return (topic);
}

void Channel::set_topic(std::string new_topic) {
	topic = new_topic;
}

bool Channel::is_topic_protected(void) {
	return (topic_protection);
}

void Channel::set_topic_protected(bool protect_val) {
	topic_protection = protect_val;
}

bool Channel::is_on_invite(void) {
	return (on_invite);
}

void Channel::set_invite(bool invite_val) {
	on_invite = invite_val;
}

std::string Channel::get_password(void) {
	return (password);
}

void Channel::set_password(std::string new_password) {
	password = new_password;
}

int Channel::get_limit(void) {
	return (limit);
}

void Channel::set_limit(int new_limit) {
	limit = new_limit;
}
