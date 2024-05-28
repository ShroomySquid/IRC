#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"
#include <stdlib.h>

Cmd_mode::Cmd_mode(){}
Cmd_mode::~Cmd_mode(){}

void Cmd_mode::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string current_mode;
	Channel *channel;
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() <= 1)
		return;
	channel = server.getChannel(arguments[1]);
	if (!channel) {
		sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, sender.get_client().c_str(), arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
		return ;
	}
	if (arguments.size() < 3) {
		sendReplyMsg(sender.get_fd(), RPL_CHANNELMODEIS, sender.get_client().c_str(), arguments[1].c_str(), RPL_CHANNELMODEIS_MSG, NULL);
		return ;
	}
	if (!channel->is_operator(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_CHANOPRIVSNEEDED, sender.get_client().c_str(), arguments[1].c_str(), ERR_CHANOPRIVSNEEDED_MSG, NULL);
		return ;
	}	
	//std::string mode = arguments[2];








	std::string mode = arguments[2];
	if (!mode.compare("+i")) {
		channel->set_invite(true);
		channel->broadcastCmd("MODE (invite)", "Invite is now requiered to join channel");
		return ;
	}
	if (!mode.compare("-i")) {
		channel->set_invite(false);
		channel->broadcastCmd("MODE (invite)", "Invite is not requiered to join channel");
		return ;
	}
	if (!mode.compare("+t")) {
		channel->set_topic_protected(true);
		channel->broadcastCmd("MODE (topic)", "Topic can only be change by operators");
		return ;
	}
	if (!mode.compare("-t")) {
		channel->set_topic_protected(false);
		channel->broadcastCmd("MODE (topic)", "Topic can be change by all members");
		return ;
	}
	if (!mode.compare("+k")) {
		if (arguments.size() < 4 || !arguments[3].length()) {	
			sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[1].c_str(), "MODE (password)", ERR_NEEDMOREPARAMS_MSG, NULL);
			return ;
		}
		channel->set_password(arguments[3]);
		channel->broadcastCmd("MODE (password)", "This channel requieres now a password.");
		return ;
	}
	if (!mode.compare("-k")) {
		channel->set_password("");
		channel->broadcastCmd("MODE (password)", "This channel does not requieres a password anymore.");
		return ;
	}
	Client* client = server.find_client(arguments[3]);
	if (!mode.compare("+o")) {
		if (arguments.size() < 5 || !arguments[3].length()) {
			sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[1].c_str(), "MODE (operator)", ERR_NEEDMOREPARAMS_MSG, NULL);
			return ;
		}
		if (!channel->is_member(client)) {	
			sendErrorMsg(sender.get_fd(), ERR_USERNOTINCHANNEL, sender.get_client().c_str(), arguments[1].c_str(), arguments[3].c_str(), ERR_USERNOTINCHANNEL_MSG, NULL);
			return ;
		}
		channel->promote(client);
		channel->broadcastCmd("MODE (operator)", "<client> has been promoted to operator");
		return ;
	}
	if (!mode.compare("-o")) {
		if (arguments.size() < 4 || !arguments[3].length()) {
			sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[1].c_str(), "MODE (operator)", ERR_NEEDMOREPARAMS_MSG, NULL);
			return ;
		}
		if (!channel->is_operator(client)) {	
			sendErrorMsg(sender.get_fd(), ERR_USERNOTINCHANNEL, sender.get_client().c_str(), arguments[1].c_str(), arguments[3].c_str(), ERR_USERNOTINCHANNEL_MSG, NULL);
			return ;
		}
		channel->demote(client);
		channel->broadcastCmd("MODE (operator)", "<client> has been demoted to member");
		return ;
	}
	if (!mode.compare("+l")) {
		if (arguments.size() < 4 || !arguments[3].length()) {	
			sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[1].c_str(), "MODE (operator)", ERR_NEEDMOREPARAMS_MSG, NULL);
			return ;
		}
		int limit = atoi(arguments[3].c_str());
		if (limit < 1) {
			sendErrorMsg(sender.get_fd(), ERR_UNKNOWNERROR, sender.get_client().c_str(), arguments[1].c_str(), "MODE (limit)", ":Invalid parameter", NULL);
			return ;
		}
		channel->set_limit(limit);
		channel->broadcastCmd("MODE (limit)", "Channel now have a member limit");
		return ;
	}
	if (!mode.compare("-l")) {
		channel->set_limit(-1);
		channel->broadcastCmd("MODE (limit)", "Channel now have no member limit");
		return ;
	}
	sendErrorMsg(sender.get_fd(), ERR_UNKNOWNERROR, sender.get_client().c_str(), arguments[1].c_str(), "MODE", ":Unknowed mode", NULL);
}
