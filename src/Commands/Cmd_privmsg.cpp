#include "../../inc/Command.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::execute(Client& sender, std::vector<std::string> arguments)
{
	std::cout << arguments[1] << std::endl;
	std::cout << arguments[2] << std::endl;
	Channel* channel = Channel::getChannel(arguments.at(1));
	if (channel)
	{
		channel->broadcastAll(sender, arguments[2]);
		std::cout << "Message sent to channel: " << arguments.at(1) << std::endl;
	}
	else
	{
		std::cout << "This channel doesnt exist" << std::endl;
	}
}