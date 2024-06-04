#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_kick::Cmd_kick(){}
Cmd_kick::~Cmd_kick(){}

void Cmd_kick::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 3) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return;
	}
	std::string channelName = arguments[1];
	if (channelName[0] == '#' || channelName[0] == '&') {
		channelName.erase(0, 1);
	}
	std::string user = arguments[2];
	Channel* channel = server.getChannel(channelName);
	if (!channel) {
		sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, sender.get_client().c_str(), arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
		return;
	}
	if (!channel->is_operator(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_CHANOPRIVSNEEDED, sender.get_client().c_str(), arguments[1].c_str(), ERR_CHANOPRIVSNEEDED_MSG, NULL);
		return ;
	}
	Client *client = channel->getMember_by_name(user);
	if (client)
	{
		channel->removeClient(client);
		channel->broadcastAll(&sender, 3, "KICK", user.c_str(), "has been kick out of the server");
	}
}
