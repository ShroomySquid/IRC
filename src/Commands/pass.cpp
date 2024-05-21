#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_pass::Cmd_pass(){}
Cmd_pass::~Cmd_pass(){}

void Cmd_pass::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (sender.is_authentified()) {
		send(sender.get_fd(), "You may not reregister\n", 24, 0);
		return ;
	}
	if (!server.get_password().compare(arguments[0])) {
		sender.authentify();
		cout << "Client " << sender.get_fd() << " is authentified." << endl;
	}
	else
		send(sender.get_fd(), "Password incorrect\n", 20, 0);
}
