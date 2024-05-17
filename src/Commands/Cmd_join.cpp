#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

void Cmd_join::execute(Client& sender, std::vector<std::string> arguments)
{
	std::string channelName = arguments[1];
	Channel *c = Channel::getChannel(channelName);
	if (c == NULL)
	{
		std::cout << "Creating new channel" << std::endl;
		c = new Channel(channelName);
		Channel::addChannel(channelName, c);
	}
	bool sucess = c->addClient(&sender);
	if (sucess)
		std::cout << sender.getName() << "have been added to channel: "<< channelName << std::endl;
	else
		std::cout << sender.getName() << "Client already inside channel: "<< channelName << std::endl;
}