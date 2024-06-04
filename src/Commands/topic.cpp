#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_topic::Cmd_topic(){}
Cmd_topic::~Cmd_topic(){}

void Cmd_topic::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	std::string topic;
	Channel *channel;
	int topic_length;	
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
	arguments[1].erase(0 ,1);
	channel = server.getChannel((arguments[1]));
	if (!channel) {
		sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
		return ;
	}
	if (arguments.size() < 3) {
		topic = channel->get_topic();
		topic_length = topic.length();
		if (!topic_length) {
			sendReplyMsg(sender.get_fd(), RPL_NOTOPIC, arguments[1].c_str(), RPL_NOTOPIC_MSG, NULL);
			return ;
		}
		else {
			sendReplyMsg(sender.get_fd(), RPL_TOPIC, sender.get_client().c_str(), arguments[1].c_str(), channel->get_topic().c_str(), NULL);
			return ;
		}
	}
	if (channel->is_topic_protected() && !channel->is_operator(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_CHANOPRIVSNEEDED, arguments[1].c_str(), ERR_CHANOPRIVSNEEDED_MSG, NULL);
		return ;
	}
	if (!channel->is_topic_protected() && !channel->is_operator(&sender) && !channel->is_member(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_NOTONCHANNEL, arguments[1].c_str(), ERR_NOTONCHANNEL_MSG, NULL);
		return ;
	}
	channel->set_topic(arguments[2]);
	channel->broadcastAll(&sender, 3, "TOPIC", "is set to:", arguments[2].c_str());
}
