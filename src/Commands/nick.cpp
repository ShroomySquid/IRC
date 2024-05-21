#include "../../inc/Command.hpp"

Cmd_nick::Cmd_nick(){}
Cmd_nick::~Cmd_nick(){}

void Cmd_nick::execute(Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_authentified()) {	
		send(client.get_fd(), "Client is not authentified\n", 28, 0);
		return ;
	}
	if (sender.get_nickname().length()) {	
		send(client.get_fd(), "Nickname already registered\n", 29, 0);
		return ;
	}
	if (!buffer || !buffer[0]) {	
		send(sender.get_fd(), "No nickname given\n", 19, 0);
		return ;
	}
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->get_nickname().compare(buffer)) {
			send(client.get_fd(), "Nickname is already in use\n", 27, 0);
			return ;
		}
	}
	if (check_invalid_symbols(buffer)) {	
		send(sender.get_fd(), "Erroneus nickname\n", 19, 0);
		return ;
	}
	sender.set_nickname(arguments[0]);
	if (sender.get_nickname().length() && sender.get_username().length())
		client.regist();
}
