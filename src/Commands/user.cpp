#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_user::Cmd_user(){}
Cmd_user::~Cmd_user(){}

void Cmd_user::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	//cout << "enter user function" << endl;
	//(void)server;
	if (!sender.is_authentified()) {	
		send(sender.get_fd(), "Client is not authentified\n", 28, 0);
		return ;
	}
	if (sender.get_username().length()) {	
		send(sender.get_fd(), "Username already registered\n", 29, 0);
		return ;
	}
	if (arguments.size() < 2 || !arguments[1][0]) {	
		send(sender.get_fd(), "No username given\n", 19, 0);
		return ;
	}
	if (check_invalid_symbols(arguments[1])) {	
		send(sender.get_fd(), "Erroneus username\n", 19, 0);
		return ;
	}
	for (std::map<int, Client*>::iterator it = server.get_clients().begin(); it != server.get_clients().end(); it++) {
		if (!it->second->get_username().compare(arguments[1])) {
			send(sender.get_fd(), "Username is already in use\n", 27, 0);
			return ;
		}
	}
	sender.set_username(arguments[1]);
	if (sender.get_nickname().length() && sender.get_username().length())
		sender.regist();
}
