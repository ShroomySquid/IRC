#include "../inc/Channel.hpp"

Channel::Channel(std::string name)
{
    this->clients = std::vector<std::string>();
    this->name = name;
}

Channel::~Channel(){}

Channel::Channel(const Channel& src)
{
    *this = src;
}

void Channel::addClient(std::string name)
{
    this->clients.push_back(name);
}

Channel& Channel::operator=(const Channel& src)
{
    this->clients = src.clients;
    this->name = src.name;
    return *this;
}