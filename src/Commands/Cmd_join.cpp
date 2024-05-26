#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

void Cmd_join::fill_chan_to_join(std::map<std::string, std::string> &chan_to_join, std::string channels, std::string passwords) {
	int chan_start = 0;
	int len = 0;
	int pass_start = 0;
	int pass_len = 0;
	std::string chan_name;
	std::string pass_name;
	while (channels[chan_start]) {
		len = 0;
		if (channels[chan_start] == '#' || channels[chan_start] == '&')
			chan_start++;
		while (channels[chan_start + len] && channels[chan_start + len] != ',')
			len++;
		if (len > 0)
			chan_name = channels.substr(chan_start, len);
		if (passwords == "" && len > 0)
			chan_to_join[chan_name] = "";
		else if (len > 0) {
			pass_len = 0;
			while (passwords[pass_start + pass_len] && passwords[pass_start + pass_len] != ',')
				pass_len++;
			if (pass_len == 0)
				pass_name = "";
			else
				pass_name = passwords.substr(pass_start, pass_len);
			chan_to_join[chan_name] = pass_name;
			pass_start += pass_len;
			if (passwords[pass_start] && passwords[pass_start] == ',')
				pass_start++;
		}
		chan_start += len;
		if (channels[chan_start] && channels[chan_start] == ',')
			chan_start++;
	}
}

void Cmd_join::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	bool is_operator;
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	if (arguments.size() < 2) {
		// err_needmoreparams 461
		send(sender.get_fd(), "need more param\n", 16, 0);
	}
	std::map<std::string, std::string> chan_to_join;
	if (arguments.size() < 3)
		fill_chan_to_join(chan_to_join, arguments[1], "");
	else
		fill_chan_to_join(chan_to_join, arguments[1], arguments[2]);
	std::map<std::string, std::string>::iterator it;
	for (it = chan_to_join.begin(); it != chan_to_join.end(); it++) {
		cout << it->first << " " << it->second << endl;
		is_operator = false;
		Channel *channel = server.getChannel(it->first);
		if (channel == NULL)
		{
			std::cout << "Creating new channel" << std::endl;
			channel = new Channel(it->first);
			server.addChannel(it->first, channel);
			is_operator = true;
			if (it->second != "")
				channel->set_password(it->second);
			channel->addClient(&sender, is_operator);
			cout << "Channel created: " << it->first << endl;
			//broadcast all the new channel created	
			continue ;
		}
		if (channel->get_password() != "" && channel->get_password() != it->second) {
			//err_badchannelkey 475
			send(sender.get_fd(), "bad password\n", 13, 0);
			continue ;
		}
		if (channel->is_on_invite()) {	
			//err_inviteonlychan 473
			send(sender.get_fd(), "No one is allowed here\n", 23, 0);
			continue ;
		}
		bool success = channel->addClient(&sender, is_operator);
		if (success) {
			channel->broadcastCmd(sender.get_nickname(), "has joined the channel.");
		}
	}
}
