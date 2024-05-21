#include "../../inc/Command.hpp"

Cmd_pass::Cmd_pass(){}
Cmd_pass::~Cmd_pass(){}

void Cmd_pass::execute(Client& sender, std::vector<std::string> arguments)
	if (sender.is_authentified()) {
		send(sender.get_fd(), "You may not reregister\n", 24, 0);
		return ;
	}
	if (!password.compare(buffer)) {
		client.authentify();
		cout << "Client " << client.get_fd() << " is authentified." << endl;
	}
	else
		send(client.get_fd(), "Password incorrect\n", 20, 0);
}
