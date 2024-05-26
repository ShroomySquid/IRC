#include "../inc/Client.hpp"

Client::Client() : _fd(-1), _username("invalid"), _nickname("invalid") {}
Client::~Client(){}

Client::Client(int fd) : _fd(fd), _username(""), _nickname("")
{
    this->_disconnected = false;
    this->_authentified = false;
    this->_registered = false;
}

Client::Client(int fd, std::string username, std::string nickname) : _fd(fd), _username(username), _nickname(nickname)
{
    this->_registered = false;
    this->_authentified = false;
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

void Client::set_username(std::string user)
{
	if (_username.empty())
		_username = user;
    sendServerMsg("User username set to %s", user.c_str());
}

void Client::set_nickname(std::string nick)
{
	if (_nickname.empty())
		_nickname = nick;
    sendServerMsg("User nickname set to %s", nick.c_str());
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
    // should we send this ??

    // std::cout << "Client registered" << std::endl;
    // std::string message001;
    // message001 += this->_nickname + ":Welcome to the <networkname> Network, <nick>[!<user>@<host>]";
    // send(this->_fd, message001.c_str(), message001.length(), 0);
    this->_registered = true;
}

std::string Client::get_client() {
	if (_nickname.length())
		return _username;
	return _nickname;
}
