#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_quit::Cmd_quit(){}
Cmd_quit::~Cmd_quit(){}

void Cmd_quit::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	//if (arguments.size() <= 1)
	//	return;

    // quit all channels with this user
    std::map <std::string, Channel*> channels = server.get_all_channels();
    std::map <std::string, Channel*>::iterator it;
	for (it = channels.begin(); it != channels.end(); it++)
	{
		Channel *c = (*it).second;
		c->removeClient(&sender);
	}
	sender.disconnect();
	// broadcast a message to all users
	std::string quitmessage = std::string();
	quitmessage += sender.get_client();
	quitmessage += " Quit: ";
	if (arguments.size() > 1)
		quitmessage += arguments[1];
	std::map<int, Client*> clients =  server.get_clients();
	std::map<int, Client*>::iterator it2;
	for (it2 = clients.begin(); it2 != clients.end(); it2++)
	{
		Client* c = (*it2).second;
		sendReplyMsg(c->get_fd(), quitmessage.c_str(), NULL);
		// send(c->get_fd(), quitmessage.c_str(), quitmessage.size(), 0);
	}

}
