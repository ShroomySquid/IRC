#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_topic::Cmd_topic(){}
Cmd_topic::~Cmd_topic(){}

void Cmd_topic::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string topic;
	Channel *channel;
	int topic_length	
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, client.get_client(), arguments[0], ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
;
	channel = server.getChannel(arguments[1]);
	if (!channel) {
		sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, client.get_client(), arguments[1], ERR_NOSUCHCHANNEL_MSG, NULL);
		return ;
	}
	if (arguments.size() < 3) {
		topic = channel->get_topic();
		topic_length = topic.length();
		if (!topic_length) {
			sendReplyMsg(sender.get_fd(), RPL_NOTOPIC, client.get_client(), arguments[1], RPL_NOTOPIC_MSG, NULL);
			return ;
		}
		else {
			sendReplyMsg(sender.get_fd(), RPL_TOPIC, client.get_client(), arguments[1], channel.get_topic(), NULL);
			return ;
		}
	}
	if (channel->is_topic_protected() && !channel->is_operator(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_CHANOPRIVSNEEDED, client.get_client(), arguments[1], ERR_CHANOPRIVSNEEDED_MSG, NULL);
		return ;
	}
	if (!channel->is_topic_protected() && !channel->is_operator(&sender) && !channel->is_member(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_NOTONCHANNEL, client.get_client(), arguments[1], ERR_NOTONCHANNEL_MSG, NULL);
		return ;
	}
	channel->set_topic(arguments[2]);
	channel->broadcastCmd("Topic", arguments[2]);
}
