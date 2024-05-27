#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_nick::Cmd_nick(){}
Cmd_nick::~Cmd_nick(){}

void Cmd_nick::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	//cout << "enter nick function" << endl;
	//(void)server;
	if (!sender.is_authentified()) {	
		send(sender.get_fd(), "Client is not authentified\r\n", 28, 0);
		return ;
	}
	if (arguments.size() < 2 || !arguments[1][0]) {	
		send(sender.get_fd(), "No nickname given\n", 19, 0);
		return ;
	}
	if (check_invalid_symbols(arguments[1])) {	
		send(sender.get_fd(), "Erroneus nickname\n", 19, 0);
		return ;
	}
	// if (check_invalid_symbols(arguments[1])) {	
	// 	send(sender.get_fd(), "Erroneus nickname\n", 19, 0);
	// 	return ;
	// }
	for (std::map<int, Client*>::iterator it = server.get_clients().begin(); it != server.get_clients().end(); it++) {
		if (!it->second->get_nickname().compare(arguments[1])) {
			send(sender.get_fd(), "Nickname is already in use\n", 27, 0);
			return ;
		}
	}
	sender.set_nickname(arguments[1]);
	if (sender.get_nickname().length() && sender.get_username().length())
		sender.regist();
}
