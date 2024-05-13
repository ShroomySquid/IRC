#include "Client.hpp"

Client::Client(){}
Client::~Client(){}

Client::Client(int fd)
{
    this->fd = fd;
}

Client::Client(const Client& src)
{
    *this = src;
}

Client& Client::operator=(const Client& src)
{
    this->fd = src.fd;
}
