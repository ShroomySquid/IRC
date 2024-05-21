#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	Channel* channel = server.getChannel(arguments.at(1));
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