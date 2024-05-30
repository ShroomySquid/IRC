#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return false;
	}
	if (arguments.size() < 3) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return false;
	}
	Channel* channel = server.getChannel(arguments.at(1));
	if (channel == NULL)
	{
		std::cout << "This channel doesnt exist" << std::endl;
		return;
	}
	channel->broadcastAll(2, sender.get_client().c_str(), arguments[2].c_str());
}
