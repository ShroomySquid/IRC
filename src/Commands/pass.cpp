#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_pass::Cmd_pass(){}
Cmd_pass::~Cmd_pass(){}

void Cmd_pass::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	//cout << "entered pass function" << endl;
	if (sender.is_authentified()) {
		send(sender.get_fd(), "You may not reregister\n", 24, 0);
		return ;
	}
	if (arguments.size() > 1 && !server.get_password().compare(arguments[1])) {
		sender.authentify();
		cout << "Client " << sender.get_fd() << " is authentified." << endl;
	}
	else
		send(sender.get_fd(), "Password incorrect\n", 20, 0);
}
