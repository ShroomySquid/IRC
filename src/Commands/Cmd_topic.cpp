#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_topic::Cmd_topic(){}
Cmd_topic::~Cmd_topic(){}

void Cmd_topic::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string current_topic;
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
		topic = channel->get_topic();
		topic_length = topic.length();
		// rpl_notopic 331
		if (!topic_length) {
			send(sender.get_fd(), "No current topic for channel\n", 30, 0);
			return ;
		}
		// rpl_topic 332
		else {
			send(sender.get_fd(), "Topic: ", 7, 0);
			send(sender.get_fd(), topic.c_str(), topic_length, 0);
			send(sender.get_fd(), "\n", 1, 0);
			return ;
		}
	}
	if (channel->is_topic_protected() && !channel->is_operator(&sender)) {
		//err_chanoprivsneeded 482
		send(sender.get_fd(), "Nope\n", 5, 0);
		return ;
	}
	if (!channel->is_topic_protected() && !channel->is_operator(&sender) && !channel->is_member(&sender)) {
		//err_notonchannel 442
		send(sender.get_fd(), "Nope\n", 5, 0);
		return ;
	}
	channel->set_topic(arguments[2]);
	channel->broadcastCmd("Topic", arguments[2]);
}
