#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	Channel* channel = server.getChannel(arguments.at(1));
	if (channel == NULL)
		std::cout << "This channel doesnt exist" << std::endl;
	channel->broadcastAll(sender, arguments[2]);
	std::cout << "Message sent to channel: " << arguments.at(2) << std::endl;
}
