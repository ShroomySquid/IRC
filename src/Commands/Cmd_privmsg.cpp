#include "../../inc/Command.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::execute(std::vector<std::string> arguments)
{
	(void) arguments;
	std::cout << "Message prive !" << std::endl;
}