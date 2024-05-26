#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_pass::Cmd_pass(){}
Cmd_pass::~Cmd_pass(){}

void Cmd_pass::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!arguments[1].size()) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, "*", "PASS", ERR_NEEDMOREPARAMS_MSG, NULL);
		return;
	}
	else if (sender.is_authentified()) {
		sendErrorMsg(sender.get_fd(), ERR_ALREADYREGISTERED, "*", ERR_ALREADYREGISTERED_MSG, NULL);
		return;
	}
	if (!server.get_password().compare(arguments[1])) {
		sender.authentify();
		sendServerMsg("User is authenticated");
	}
	else {
		sendErrorMsg(sender.get_fd(), ERR_PASSWDMISMATCH, "*", ERR_PASSWDMISMATCH_MSG, NULL);
	}
}
