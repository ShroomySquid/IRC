#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_part::Cmd_part(){}
Cmd_part::~Cmd_part(){}

void Cmd_part::fill_chan_to_quit(std::vector<std::string> &chan_to_quit, std::string channels) {
	int chan_start = 0;
	int len = 0;
	std::string chan_name;
	while (channels[chan_start]) {
		while (channels[chan_start] && channels[chan_start] == ',')
			chan_start++;
		len = 0;
		if (channels[chan_start] == '#' || channels[chan_start] == '&')
			chan_start++;
		while (channels[chan_start + len] && channels[chan_start + len] != ',')
			len++;
		if (len > 0) {
			chan_name = channels.substr(chan_start, len);
			chan_to_quit.push_back(chan_name);
		}
		chan_start += len;
	}
}

void Cmd_part::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, "*", "PART", ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
	std::vector<std::string> chan_to_quit;
	fill_chan_to_quit(chan_to_quit, arguments[1]);
	std::vector<std::string>::iterator it;
	for (it = chan_to_quit.begin(); it != chan_to_quit.end(); it++) {
		Channel *channel = server.getChannel(*it);
		if (channel == NULL)
		{
			sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, sender.get_client().c_str(), arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
			continue ;
		}
		if (!channel->is_operator(&sender) && !channel->is_member(&sender)) {
			sendErrorMsg(sender.get_fd(), ERR_NOTONCHANNEL, sender.get_client().c_str(), arguments[1].c_str(), ERR_NOTONCHANNEL_MSG, NULL);
			continue ;
		}
		channel->removeClient(&sender);
		sendServerMsg("%s has leaved the channel: %s", sender.get_client().c_str(), arguments[1].c_str(), NULL);
		channel->broadcastAll(&sender, 2, sender.get_client().c_str(), "has leaved the channel");
	}
}
