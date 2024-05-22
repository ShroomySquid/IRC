#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_kick::Cmd_kick(){}
Cmd_kick::~Cmd_kick(){}

void Cmd_kick::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	if (arguments.size() <= 2)
		return;

	std::string channelName = arguments[1];
	std::string user = arguments[2];

	Channel* channel = server.getChannel(channelName);
	if (channel == NULL)
		return;
	Client *client = channel->getMember_by_name(user);
	if (client)
	{
		channel->removeClient(client);
		std::cout << user << "kicked from" << channelName << std::endl;
	}
}
