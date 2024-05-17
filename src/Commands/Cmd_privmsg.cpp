#include "../../inc/Command.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::execute(Client& sender, std::vector<std::string> arguments)
{
	(void) sender;
	(void) arguments;
	std::cout << "Message prive !" << std::endl;
}