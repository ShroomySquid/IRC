#include "../inc/Client.hpp"

Client::Client() : _fd(-1), _username("invalid"), _nickname("invalid") {}
Client::~Client(){}

Client::Client(int fd, std::string ip, int port) : _fd(fd), _ip(ip), _port(port)
{
    this->_disconnected = false;
    this->_authentified = false;
    this->_registered = false;
}

Client::Client(int fd, std::string ip, int port, std::string username, std::string nickname) : _fd(fd), _ip(ip), _port(port), _username(username), _nickname(nickname)
{
    this->_disconnected = false;
    this->_authentified = false;
    this->_registered = false;
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

void Client::set_username(std::string user)
{
	if (_username.empty())
		_username = user;
    sendServerMsg("%s:%d set username to: %s", _ip.c_str(), _port, user.c_str());
}

void Client::set_nickname(std::string nick)
{
	if (_nickname.empty())
		_nickname = nick;
    sendServerMsg("%s:%d set nickname to: %s", _ip.c_str(), _port, nick.c_str());
}

bool Client::is_authentified()
{
    return this->_authentified;
}

bool Client::is_disconnected()
{
    return this->_disconnected;
}

bool Client::is_registered()
{
    return this->_registered;
}

void Client::disconnect()
{
    this->_disconnected = true;
}

void Client::authentify()
{
    this->_authentified = true;
}

void Client::regist()
{
    sendReplyMsg(this->_fd, RPL_WELCOME, get_nickname().c_str(), RPL_WELCOME_MSG, NULL);
    this->_registered = true;
}

std::string Client::get_client() {
	if (!_nickname.length())
		return _username;
	return _nickname;
}

std::string Client::get_ip() const { return _ip; }
int Client::get_port() const { return _port; }
