#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_user::Cmd_user(){}
Cmd_user::~Cmd_user(){}

void Cmd_user::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	(void)server;
	if (!sender.is_authentified()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (sender.get_username().length()) {
		sendErrorMsg(sender.get_fd(), ERR_ALREADYREGISTERED, ERR_ALREADYREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 2 || !arguments[1][0]) {
		if (sender.get_nickname().length() == 0)
			sendErrorMsg(sender.get_fd(), ERR_NONICKNAMEGIVEN, ERR_NONICKNAMEGIVEN_MSG, NULL);
		else
			sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_nickname().c_str(), "USER", ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
	sender.set_username(arguments[1]);
	if (sender.get_nickname().length() && sender.get_username().length())
	{
		sender.regist();
		sendServerMsg("%s has joined the server", sender.get_nickname().c_str());
	}
}
