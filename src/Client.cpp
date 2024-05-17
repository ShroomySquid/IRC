#include "../inc/Client.hpp"

Client::Client(){}
Client::~Client(){}

Client::Client(int fd)
{
    this->fd = fd;
    this->disconnected = false;
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