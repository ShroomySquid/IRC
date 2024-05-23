#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_topic::Cmd_topic(){}
Cmd_topic::~Cmd_topic(){}

void Cmd_topic::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string current_topic;
	Channel *current_channel;
	int current_length;
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	current_channel = server.getChannel(arguments[1]);
	if (arguments.size() < 2 || !current_channel) {
		// err_nosuchchannel 403
		send(sender.get_fd(), "No such channel\n", 17, 0);
		return ;
	}
	if (arguments.size() < 3) {
		current_topic = current_channel->get_topic();
		current_length = current_topic.length();
		// rpl_notopic 331
		if (!current_length) {
			send(sender.get_fd(), "No current topic for channel\n", 30, 0);
			return ;
		}
		// rpl_topic 332
		else {
			send(sender.get_fd(), "Topic: ", 7, 0);
			send(sender.get_fd(), current_topic.c_str(), current_length, 0);
			send(sender.get_fd(), "\n", 1, 0);
			return ;
		}
	}
	else {
		if (current_channel->is_topic_protected() && !current_channel->is_operator(&sender)) {
			//err_chanoprivsneeded 482
			send(sender.get_fd(), "Nope\n", 5, 0);
			return ;
		}
		if (!current_channel->is_topic_protected() && !current_channel->is_operator(&sender) && !current_channel->is_member(&sender)) {
			//err_chanoprivsneeded 482
			send(sender.get_fd(), "Nope\n", 5, 0);
			return ;
		}
		current_channel->set_topic(arguments[2]);
		current_channel->broadcastCmd("Topic", arguments[2]);
	}
}
