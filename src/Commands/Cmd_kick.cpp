#include "../../inc/Command.hpp"

Cmd_kick::Cmd_kick(){}
Cmd_kick::~Cmd_kick(){}

void Cmd_kick::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	(void) server;
	(void) sender;
	(void) arguments;
	std::cout << "Vous avez kick quelqun !" << std::endl;
}
