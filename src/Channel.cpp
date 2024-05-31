#include "../inc/Channel.hpp"

// -------------Channel Class --------------
Channel::Channel(std::string name)
{
	this->topic = "";
    this->members = std::vector<Client*>();
    this->operators = std::vector<Client*>();
	this->invited = std::vector<Client*>();
    this->name = "#" + name;
	this->topic_protection = true;
	this->on_invite = false;
	this->password = "";
	this->limit = 0;
	this->clients_in_channel = 1;
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
	if (this->on_invite)
	{
		std::vector<Client*>::iterator it = std::find(invited.begin(), invited.end(), c);
		if (it == invited.end())
		{
			std::cout << "cannot join beacause client is not invited !" << std::endl;
			return false;
		}
	}
	if (!ope) 
	{
		this->members.push_back(c);
		return true;
	}
	this->operators.push_back(c);
	this->clients_in_channel++;

	// erase from invited 
	if (!invited.empty()) {
		std::vector<Client*>::iterator it = std::find(invited.begin(), invited.end(), c);
		if (it != invited.end())
			this->invited.erase(it);
	}
	return true;
}

bool Channel::addInvited(Client *c)
{
	if (!this->on_invite)
		return false;
	if (is_member(c) || is_operator(c))
		return false;
	std::vector<Client*>::iterator it = std::find(invited.begin(), invited.end(), c);
	if (it == invited.end())
	{
		this->invited.push_back(c);
		return true;
	}
	return false;
}

bool Channel::is_Invited(Client* c)
{
	std::vector<Client*>::iterator it = std::find(invited.begin(), invited.end(), c);
	if (it == invited.end())
		return false;
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
		if (it != members.end()) {
			this->members.erase(it);
			this->clients_in_channel--;
		}
	}
	if (!operators.empty()) {
		std::vector<Client*>::iterator i = std::find(operators.begin(), operators.end(), c);
		if (i != operators.end()) {
			this->operators.erase(i);	
			this->clients_in_channel--;
		}
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

void Channel::broadcastAll(int count, ...) {
    va_list args;
    va_start(args, count);
	int i = 0;
	std::stringstream ss;
	ss << PREFIX;
	ss << " " << get_name();
	while (i < count) {
		const char *arg = va_arg(args, const char *);
		if (arg == NULL) {
			break;
		}
		ss << " " << arg;
		i++;
	}
	ss << "\r\n";
	std::string response = ss.str();
	for (std::vector<Client*>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		send((*it)->get_fd(), response.c_str(), response.size(), 0);
	}
	for (std::vector<Client*>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{
		send((*it)->get_fd(), response.c_str(), response.size(), 0);
	}
	va_end(args);
}

void Channel::broadcastAlmostAll(Client* sender, int count, ...) {
    va_list args;
    va_start(args, count);
	int i = 0;
	std::stringstream ss;
	ss << ":" << sender->get_client();
	ss << " " << get_name();
	while (i < count) {
		const char *arg = va_arg(args, const char *);
		//cout << arg << endl;
		if (arg == NULL) {
			break;
		}
		ss << " " << arg;
		i++;
	}
	ss << "\r\n";
	std::string response = ss.str();
	for (std::vector<Client*>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		if ((*it)->get_fd() != sender->get_fd())
			send((*it)->get_fd(), response.c_str(), response.size(), 0);
	}
	for (std::vector<Client*>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{
		if ((*it)->get_fd() != sender->get_fd())
			send((*it)->get_fd(), response.c_str(), response.size(), 0);
	}
	va_end(args);
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

int Channel::get_clients_nbr(void) {
	return (clients_in_channel);
}

std::string Channel::get_name(void) {
	return (name);
}

int Channel::get_limit(void) {
	return (limit);
}

void Channel::set_limit(int new_limit) {
	limit = new_limit;
}

std::vector<Client*> Channel::get_members()
{
	return this->members;
}
