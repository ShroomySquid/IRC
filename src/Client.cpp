#include "../inc/Client.hpp"

Client::Client() : _fd(-1), _username("invalid"), _nickname("invalid") {}
Client::~Client(){}

Client::Client(int fd) : _fd(fd), _username("username"), _nickname("nickname")
{
    this->_disconnected = false;
}

Client::Client(int fd, std::string username, std::string nickname) : _fd(fd), _username(username), _nickname(nickname)
{
    this->_disconnected = false;
}

Client::Client(const Client& src): _fd(src._fd), _username(src._username), _nickname(src._nickname), _disconnected(src._disconnected)
{
    *this = src;
}

Client& Client::operator=(const Client& src) {
	this->_disconnected = src._disconnected;
	return *this;
}

int Client::get_fd()
{
    return this->_fd;
}

std::string Client::get_username()
{
    return this->_username;
}

std::string Client::get_nickname()
{
    return this->_nickname;
}

bool Client::is_disconnected()
{
    return this->_disconnected;
}

void Client::disconnect()
{
    this->_disconnected = true;
}
