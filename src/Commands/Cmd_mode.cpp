#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_mode::Cmd_mode(){}
Cmd_mode::~Cmd_mode(){}

void Cmd_mode::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string current_mode;
	Channel *channel;
	int current_length;
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
	if (!current_channel->is_operator(&sender)) {
		// err_chanoprivsneeded 482
		send(sender.get_fd(), "Ehhhh\n", 6, 0);
		return ;
	}
	if (!arguments[2].compare("+i")) {
		channel.set_invite(true);
		channel->boardcastCmd("MODE (invite)", "Invite is now requiered to join channel");
		return ;
	}
	if (!arguments[2].compare("-i")) {
		channel.set_invite(false);
		channel->boardcastCmd("MODE (invite)", "Invite is not requiered to join channel");
		return ;
	}
	if (!arguments[2].compare("+t")) {
		channel.set_topic_protected(true);
		channel->boardcastCmd("MODE (topic)", "Topic can only be change by operators");
		return ;
	}
	if (!arguments[2].compare("-t")) {
		channel.set_topic_protected(false);
		channel->boardcastCmd("MODE (topic)", "Topic can be change by all members");
		return ;
	}
	if (!arguments[2].compare("+k")) {
		if (arguments.size < 4 || !arguments[3].length()) {	
			// err_???
			send(sender.get_fd(), "No password provided\n", 21, 0);
			return ;
		}
		channel.set_password(arguments[3]);
		channel->boardcastCmd("MODE (password)", "This channel requieres now a password.");
		return ;
	}
	if (!arguments[2].compare("-k")) {
		channel.set_password("");
		channel->boardcastCmd("MODE (password)", "This channel does not requieres a password anymore.");
		return ;
	}
	if (!arguments[2].compare("+o")) {
		if (arguments.size < 4 || !arguments[3].length() || channel.is_member(arguments[3])) {	
			// err_???
			send(sender.get_fd(), "No valid channel member provided\n", 33, 0);
			return ;
		}
		channel.promote(arguments[3]);
		channel->boardcastCmd("MODE (operator)", "<client> has been promoted to operator");
		return ;
	}
	if (!arguments[2].compare("-o")) {
		if (arguments.size < 4 || !arguments[3].length() || channel.is_operator(arguments[3])) {
			// err_???
			send(sender.get_fd(), "No valid channel operator provided\n", 33, 0);
			return ;
		}
		channel.demote(arguments[3]);
		channel->boardcastCmd("MODE (operator)", "<client> has been promoted to operator");
		return ;
	}
	// ERR_???
	send(sender.get_fd(), "Invalid mode passed\n", 6, 0);
}
