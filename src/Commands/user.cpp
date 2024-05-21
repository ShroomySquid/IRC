#include "../../inc/Command.hpp"

Cmd_user::Cmd_user(){}
Cmd_user::~Cmd_user(){}

void Cmd_user::execute(Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_authentified()) {	
		send(client.get_fd(), "Client is not authentified\n", 28, 0);
		return ;
	}
	if (sender.get_username().length()) {	
		send(client.get_fd(), "Username already registered\n", 29, 0);
		return ;
	}
	if (!buffer || !buffer[0]) {	
		send(sender.get_fd(), "No username given\n", 19, 0);
		return ;
	}
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->get_nickname().compare(buffer)) {
			send(client.get_fd(), "Nickname is already in use\n", 27, 0);
			return ;
		}
	}
	if (check_invalid_symbols(arguments[0])) {	
		send(sender.get_fd(), "Erroneus nickname\n", 19, 0);
		return ;
	}
	sender.set_username(arguments[0]);
	if (sender.get_nickname().length() && sender.get_username().length())
		client.regist();
}
