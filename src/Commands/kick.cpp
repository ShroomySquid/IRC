#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_kick::Cmd_kick(){}
Cmd_kick::~Cmd_kick(){}

void Cmd_kick::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 3) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return;
	}
	std::string channelName = arguments[1];
	if (channelName[0] == '#' || channelName[0] == '&') {
		channelName.erase(0, 1);
	}
	std::string user = arguments[2];
	Channel* channel = server.getChannel(channelName);
	if (!channel) {
		sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
		return;
	}
	if (!channel->is_operator(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_CHANOPRIVSNEEDED, arguments[1].c_str(), ERR_CHANOPRIVSNEEDED_MSG, NULL);
		return ;
	}
	Client *kicked = channel->getMember_by_name(user);
	if (kicked)
	{
		// send kick message to client
    	std::string kick_message = (":"+sender.get_nickname() + " KICK " + "#" + channelName + " " + user + "\r\n");

		std::vector<Client*> members = channel->get_members();

		for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); it++)
		{
			Client *c = (*it);
			send(c->get_fd(), kick_message.c_str() , kick_message.length(),0);
		}
		sendServerMsg("%s has kicked %s from %s", sender.get_client().c_str(), user.c_str(), channel->get_name().c_str());
		channel->removeClient(kicked);
	}
	else {
		sendErrorMsg(sender.get_fd(), ERR_USERNOTINCHANNEL, sender.get_client().c_str(), arguments[1].c_str(), ERR_USERNOTINCHANNEL_MSG, NULL);
	}
}
