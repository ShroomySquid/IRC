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
		not_registered_yet(sender.get_fd());
		return ;
	}
	if (arguments.size() < 2) {
		// err_needmoreparams 461
		send(sender.get_fd(), "need more param\n", 16, 0);
	}
	std::vector<std::string> chan_to_quit;
	fill_chan_to_quit(chan_to_quit, arguments[1]);
	std::vector<std::string>::iterator it;
	for (it = chan_to_quit.begin(); it != chan_to_quit.end(); it++) {
		cout << *it << endl;
		Channel *channel = server.getChannel(*it);
		if (channel == NULL)
		{
			// err_nosuchchannel 403
			send(sender.get_fd(), "need more param\n", 16, 0);
			continue ;
		}
		if (!channel->is_operator(&sender) && !channel->is_member(&sender)) {
			//err_notonchannel 442
			send(sender.get_fd(), "bad password\n", 13, 0);
			continue ;
		}
		channel->removeClient(&sender);
		//broadcast to channel that client has leaved + his message 
	}
}
