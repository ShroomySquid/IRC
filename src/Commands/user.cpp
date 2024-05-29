#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_user::Cmd_user(){}
Cmd_user::~Cmd_user(){}

void Cmd_user::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_authentified()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_nickname().c_str(), "USER", ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (sender.get_username().length()) {
		sendErrorMsg(sender.get_fd(), ERR_ALREADYREGISTERED, ERR_ALREADYREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 2 || !arguments[1][0]) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_nickname().c_str(), "USER", ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
	if (check_invalid_symbols(arguments[1])) {
		sendErrorMsg(sender.get_fd(), ERR_ERRONEUSUSERNAME, sender.get_nickname().c_str(), ERR_ERRONEUSUSERNAME_MSG, NULL);
		return ;
	}
	for (std::map<int, Client*>::iterator it = server.get_clients().begin(); it != server.get_clients().end(); it++) {
		if (!it->second->get_username().compare(arguments[1])) {
			sendErrorMsg(sender.get_fd(), "Username already taken", NULL);
			return ;
		}
	}
	sender.set_username(arguments[1]);
	if (sender.get_nickname().length() && sender.get_username().length())
	{
		sender.regist();
		sendServerMsg("%s has joined the server", sender.get_nickname().c_str());
	}
}
