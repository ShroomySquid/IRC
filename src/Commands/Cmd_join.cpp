#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

void Cmd_join::fill_chan_to_join(std::map<std::string, std::string> chan_to_join, std::string channels, std::string passwords) {
	int chan_start = 0;
	int len = 0;
	int pass_start = 0;
	int pass_len = 0;
	std::string chan_name;
	std::string pass;
	while (channels[chan_start]) {
		len = 0;
		if (channels[chan_start] == '@' || channels[chan_start] == '&')
			chan_start++;
		while (channels[chan_start + len] && channels[chan_start + len] != ',')
			len++;
		chan_name = channels.substr(chan_start, len);
		if (passwords == NULL)
			chan_to_join[chan_name] = "";
		else {
			pass_len = 0;
			while (passwords[pass_start + pass_len] && passwords[pass_start + pass_len] != ',')
				pass_len++;
			if (pass_len == 0)
				pass_name = "";
			else
				pass_name = passwords.substr(pass_start, pass_len);
			chan_to_join[chan_name] = pass;
			pass_start += pass_len;
			if (passwords[pass_start] && (passwords[pass_start] == ',')
				pass_start++;
		}
		chan_start += len;
		if (channels[chan_start] && (channels[chan_start] == ',')
			chan_start++;
	}
}

void Cmd_join::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	bool is_operator = false;
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	if (arguments.size() < 2) {
		// err_needmoreparams 461
		send(sender.get_fd(), "need more param\n", 16, 0);
	}
	std::map<std::string, str::string> chan_to_join;
	if (arguments.size() < 3)
		fill_chan_to_join(chan_to_join, arguments[1], NULL);
	else
		fill_chan_to_join(chan_to_join, arguments[1], arguments[2]);
	map<std::string, std::string>::iterator it;
	for (it = chan_to_join.begin(); it != chan_to_join.end(); it++) {
		Channel *channel = server.getChannel(it->second);
		if (c == NULL)
		{
			std::cout << "Creating new channel" << std::endl;
			channel = new Channel(channelName);
			server.addChannel(channelName, c);
			is_operator = true;
		}
		bool success = channel->addClient(&sender, is_operator);
		if (success)
			std::cout << sender.get_username() << " Client have been added to channel: "<< channelName << std::endl;
		else
			std::cout << sender.get_username() << " Client already inside channel: "<< channelName << std::endl;
	}
}
