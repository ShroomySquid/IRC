#include "../../inc/Command.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

void Cmd_join::execute(std::vector<std::string> arguments)
{
	(void) arguments;
	std::cout << "Vous avez bien rejoin un channel !" << std::endl;
}