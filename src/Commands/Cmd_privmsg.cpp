#include "../../inc/Command.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	(void)server;
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
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
