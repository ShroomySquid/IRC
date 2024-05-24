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
	//int current_length;
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	channel = server.getChannel(arguments[1]);
	if (!channel) {
		// err_nosuchchannel 403
		send(sender.get_fd(), "No such channel\n", 17, 0);
		return ;
	}
	if (arguments.size() < 3) {
		// RPL_CHANNELMODEIS 324
		send(sender.get_fd(), "Ehhhh\n", 6, 0);
		return ;
	}
	if (!channel->is_operator(&sender)) {
		// err_chanoprivsneeded 482
		send(sender.get_fd(), "Ehhhh\n", 6, 0);
		return ;
	}
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
			// err_???
			send(sender.get_fd(), "No password provided\n", 21, 0);
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
		if (arguments.size() < 4 || !arguments[3].length() || channel->is_member(client)) {	
			// err_???
			send(sender.get_fd(), "No valid channel member provided\n", 33, 0);
			return ;
		}
		channel->promote(client);
		channel->broadcastCmd("MODE (operator)", "<client> has been promoted to operator");
		return ;
	}
	if (!mode.compare("-o")) {
		if (arguments.size() < 4 || !arguments[3].length() || channel->is_operator(client)) {
			// err_???
			send(sender.get_fd(), "No valid channel operator provided\n", 33, 0);
			return ;
		}
		channel->demote(client);
		channel->broadcastCmd("MODE (operator)", "<client> has been promoted to operator");
		return ;
	}
	if (!mode.compare("+l")) {
		if (arguments.size() < 4 || !arguments[3].length()) {	
			// err_???
			send(sender.get_fd(), "No members limit provided\n", 26, 0);
			return ;
		}
		int limit = atoi(arguments[3].c_str());
		if (limit < 1) {
			// err_???
			send(sender.get_fd(), "Invalid members limit provided\n", 31, 0);
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
	// ERR_???
	send(sender.get_fd(), "Invalid mode passed\n", 6, 0);
}
