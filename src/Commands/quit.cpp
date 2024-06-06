#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_quit::Cmd_quit(){}
Cmd_quit::~Cmd_quit(){}

void Cmd_quit::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{

	// --------- broadcast a message to all users
	std::string quitmessage;
	if (arguments.size() > 1)
		quitmessage = ":" + sender.get_client() + " QUIT " + "Quit: " + arguments[1] + "\r\n";
	else
		quitmessage = ":" + sender.get_client() + " QUIT " + "Quit: " + "\r\n";

	std::map<int, Client*> clients =  server.get_clients();
	std::map<int, Client*>::iterator it2;
	for (it2 = clients.begin(); it2 != clients.end(); it2++)
	{
		Client* c = (*it2).second;
		send(c->get_fd(),quitmessage.c_str(), quitmessage.length(),0);
	}
	// --------------------------

	//disconnect sender from all channels
    std::map <std::string, Channel*> channels = server.get_all_channels();
    std::map <std::string, Channel*>::iterator it;
	for (it = channels.begin(); it != channels.end(); it++)
	{
		Channel *c = (*it).second;
		c->removeClient(&sender);
	}
	sendServerMsg("%s has quit", sender.get_client().c_str());
	sender.disconnect();
}
