#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_pass::Cmd_pass(){}
Cmd_pass::~Cmd_pass(){}

void Cmd_pass::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string nickname = sender.get_nickname();
	std::string password = server.get_password();
	if (!arguments[1].size()) {
		sendErrorMsg(sender.get_fd(), ERROR_CODE, "*", "PASS", ERROR_NOTENOUGHPARAM, NULL);
		return;
	}
	else if (sender.is_authentified()) {
		sendErrorMsg(sender.get_fd(), ERROR_CODE, "*", ERROR_MAYNOTREREGISTER, NULL);
		return;
	}
	if (!password.compare(arguments[1])) {
		sender.authentify();
		std::cout << "Client " << sender.get_fd() << " is authentified." << std::endl;
	}
	else {
		sendErrorMsg(sender.get_fd(), ERROR_CODE, "*", ERR_PASSWDMISMATCH, NULL);
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
