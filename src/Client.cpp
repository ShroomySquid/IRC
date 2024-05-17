#include "../inc/Client.hpp"

Client::Client(){}
Client::~Client(){}

Client::Client(std::string name ,int fd)
{
    this->fd = fd;
    this->disconnected = false;
    this->name = name;
}

Client::Client(const Client& src)
{
    *this = src;
}

Client& Client::operator=(const Client& src)
{
    this->fd = src.fd;
    return *this;
}

int Client::get_fd()
{
    return this->fd;
}
bool Client::is_disconnected()
{
    return this->disconnected;
}

std::string Client::getName()
{
    return this->name;
}

void Client::disconnect()
{
    this->disconnected = true;
}