#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

void Cmd_join::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	std::string channelName = arguments[1];
	Channel *c = server.getChannel(channelName);
	if (c == NULL)
	{
		std::cout << "Creating new channel" << std::endl;
		c = new Channel(channelName);
		server.addChannel(channelName, c);
	}
	bool sucess = c->addClient(&sender);
	if (sucess)
		std::cout << sender.get_username() << "have been added to channel: "<< channelName << std::endl;
	else
		std::cout << sender.get_username() << "Client already inside channel: "<< channelName << std::endl;
}
