#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <string>
#include <unistd.h>

Cmd_pass::Cmd_pass(){}
Cmd_pass::~Cmd_pass(){}

#define MAX_RESPONSE_SIZE 512
#define PREFIX ":ft_irc"
#define ERROR_CODE "461"
#define ERROR_NOTENOUGHPARAM "Not enough parameters"
#define RPL_WELCOME "001"
#define WELCOME_MSG "Welcome to the IRC server"
#define ERROR_MAYNOTREREGISTER "You may not reregister"

void sendResponse(int fd, ...) {
    va_list args;
    va_start(args, fd);

	std::stringstream ss;
	ss << PREFIX << " ";
	while (1) {
		const char *arg = va_arg(args, const char *);
		if (arg == NULL) {
			break;
		}
		ss << arg;
	}
	ss << "\n";
	std::string response = ss.str();
	send(fd, response.c_str(), response.size(), 0);
	va_end(args);
}

void sendResponse2(int fd, ...) {
	va_list args;
	va_start(args, fd);

	char response[MAX_RESPONSE_SIZE];
	int i = 0;
	while (1) {
		const char *arg = va_arg(args, const char *);
		if (arg == NULL) {
			break;
		}
		while (*arg) {
			response[i++] = *arg++;
		}
	}
	response[i++] = '\n';
	send(fd, response, i, 0);
	va_end(args);
}

void Cmd_pass::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string nickname = sender.get_nickname();
	std::string password = server.get_password();
	if (!arguments[1].size()) {
		sendResponse(sender.get_fd(), ERROR_CODE, " ", sender.get_nickname().c_str(), " ", ERROR_NOTENOUGHPARAM, "\n", NULL);
		sendResponse2(sender.get_fd(), PREFIX, " ", ERROR_CODE, " ", sender.get_nickname().c_str(), " ", ERROR_NOTENOUGHPARAM, "\n", NULL);
		return;
	}
}


// void Cmd_pass::execute(Server &server, Client& sender, std::vector<std::string> arguments)
// {
// 	//cout << "entered pass function" << endl;
// 	if (sender.is_authentified()) {
// 		send(sender.get_fd(), "You may not reregister\n", 24, 0);
// 		return ;
// 	}
// 	if (!server.get_password().compare(arguments[1])) {
// 		sender.authentify();
// 		cout << "Client " << sender.get_fd() << " is authentified." << endl;
// 	}
// 	else
// 		send(sender.get_fd(), "Password incorrect\n", 20, 0);
// }
