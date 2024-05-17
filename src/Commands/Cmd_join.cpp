#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

void Cmd_join::execute(Client& sender, std::vector<std::string> arguments)
{
	std::string channelName = arguments[0];
	Channel *c = Channel::getChannel(channelName);
	if (c == NULL)
	{
		std::cout << "Channel doesnt exist !" << std::endl;
		return;
	}

	bool sucess = c->addClient(sender.getName());
	if (sucess)
		std::cout << sender.getName() << "have been added to channel: "<< channelName << std::endl;
	else
		std::cout << sender.getName() << "Client already inside channel: "<< channelName << std::endl;
}