#include "../../inc/Command.hpp"

Cmd_kick::Cmd_kick(){}
Cmd_kick::~Cmd_kick(){}

void Cmd_kick::execute(Client& sender, std::vector<std::string> arguments)
{
	(void) sender;
	(void) arguments;
	std::cout << "Vous avez kick quelqun !" << std::endl;
}
