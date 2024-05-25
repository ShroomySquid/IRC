#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

std::map<std::string, str::string> fill_chan_to_join(std::string channels, std::string password) {
	int chan_start = 0;
	int len = 0;
	int pass_start = 0;
	int pass_len = 0;
	while (channels[i]) {
		
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
		chan_to_join = fill_chan_to_join(arguments[1], NULL);
	else
		chan_to_join = fill_chan_to_join(arguments[1], arguments[2]);
	std::string channelName = arguments[1];
	Channel *channel = server.getChannel(channelName);
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
