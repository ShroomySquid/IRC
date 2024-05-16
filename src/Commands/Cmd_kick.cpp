#include "../../inc/Command.hpp"

Cmd_kick::Cmd_kick(){}
Cmd_kick::~Cmd_kick(){}

void Cmd_kick::execute(std::vector<std::string> arguments)
{
	(void) arguments;
	std::cout << "Vous avez kick quelqun !" << std::endl;
}