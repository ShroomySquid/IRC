#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"


Cmd_nick::Cmd_nick(){}
Cmd_nick::~Cmd_nick(){}

void Cmd_nick::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_authentified()) {
		sendErrorMsg(sender.get_fd(), "You must be authentified to use this command", NULL);
		return ;
	}
	if (sender.get_nickname().length()) {
		sendErrorMsg(sender.get_fd(), ERR_ALREADYREGISTERED, ERR_ALREADYREGISTERED_MSG, NULL);
		// send(sender.get_fd(), "Nickname already registered\n", 29, 0);
		return ;
	}
	if (arguments.size() < 2 || !arguments[1][0]) {
		sendErrorMsg(sender.get_fd(), ERR_NONICKNAMEGIVEN, ERR_NONICKNAMEGIVEN_MSG, NULL);
		return ;
	}
	// if (check_invalid_symbols(arguments[1])) {
	// 	send(sender.get_fd(), "Erroneus nickname\n", 19, 0);
	// 	return ;
	// }
	for (std::map<int, Client*>::iterator it = server.get_clients().begin(); it != server.get_clients().end(); it++) {
		if (!it->second->get_nickname().compare(arguments[1])) {
			sendErrorMsg(sender.get_fd(), ERR_NICKNAMEINUSE, ERR_NICKNAMEINUSE_MSG, NULL);
			return ;
		}
	}
	sender.set_nickname(arguments[1]);
	if (sender.get_nickname().length() && sender.get_username().length())
	{
		sender.regist();
		sendServerMsg("%s has joined the server", sender.get_nickname().c_str());
		sendReplyMsg(sender.get_fd(), "001 Welcome to the Internet Relay Network", NULL);
	}
}
