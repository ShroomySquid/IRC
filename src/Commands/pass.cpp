#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_pass::Cmd_pass(){}
Cmd_pass::~Cmd_pass(){}

void Cmd_pass::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, "*", "PASS", ERR_NEEDMOREPARAMS_MSG, NULL);
		return;
	}
	else if (sender.is_authentified()) {
		sendErrorMsg(sender.get_fd(), ERR_ALREADYREGISTERED, "*", ERR_ALREADYREGISTERED_MSG, NULL);
		return;
	}
	if (arguments.size() > 1 && !server.get_password().compare(arguments[1])) {
		sender.authentify();
		sendServerMsg("User is authenticated from: %s:%d", sender.get_ip().c_str(), sender.get_port());
	}
	else {
		sendErrorMsg(sender.get_fd(), ERR_PASSWDMISMATCH, "*", ERR_PASSWDMISMATCH_MSG, NULL);
	}
}
