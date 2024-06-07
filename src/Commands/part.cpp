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
	int i = 2;
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, "PART", ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
	std::vector<std::string> chan_to_quit;
	fill_chan_to_quit(chan_to_quit, arguments[1]);
	std::string args = "has left the channel, reason gived:";
	if (arguments.size() > 3) {
		int args_nbr = arguments.size();
		while (i < args_nbr) {
			args += " ";
			args += arguments[i];
			i++;
		}
	}
	std::vector<std::string>::iterator it;
	for (it = chan_to_quit.begin(); it != chan_to_quit.end(); it++) {
		Channel *channel = server.getChannel(*it);
		if (channel == NULL)
		{
			sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
			continue ;
		}
		if (!channel->is_operator(&sender) && !channel->is_member(&sender)) {
			sendErrorMsg(sender.get_fd(), ERR_NOTONCHANNEL, arguments[1].c_str(), ERR_NOTONCHANNEL_MSG, NULL);
			continue ;
		}

		std::string partmsg = ":"+sender.get_client() + " PART " + channel->get_name() + "\r\n";
		std::vector<Client*> members = channel->get_members();

		for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); it++)
		{
			Client *c = (*it);
			send(c->get_fd(), partmsg.c_str() , partmsg.length(),0);
		}

		channel->removeClient(&sender);
		sendServerMsg("%s has left the channel: %s", sender.get_client().c_str(), arguments[1].c_str(), NULL);
		channel->broadcastAll(&sender, 2, sender.get_client().c_str(), args.c_str());
	}
}
