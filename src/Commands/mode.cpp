#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"
#include <stdlib.h>

Cmd_mode::Cmd_mode(){}
Cmd_mode::~Cmd_mode(){}

// Mode functions
void set_invite(Client *sender, Channel* channel, bool b)
{
	if (b)
	{
		channel->set_invite(true);
		channel->broadcastAll(sender,2, "MODE (invite)", "Invite is now requiered to join channel");
	}
	else
	{
		channel->set_invite(false);
		channel->broadcastAll(sender,2, "MODE (invite)", "Invite is not requiered to join channel");
	}
}

void set_topic(Client* sender, Channel* channel, bool b)
{
	if (b)
	{
		channel->set_topic_protected(true);
		channel->broadcastAll(sender, 2, "MODE (topic)", "Topic can only be change by operators");
	}
	else
	{
		channel->set_topic_protected(false);
		channel->broadcastAll(sender, 2, "MODE (topic)", "Topic can be change by all members");
	}
}

void set_password(Client* sender, Channel* channel, bool b, std::string pass)
{
	if (b)
	{
		channel->set_password(pass);
		channel->broadcastAll(sender, 2, "MODE (password)", "This channel is now password protected.");
		return ;
	}
	else
	{
		channel->set_password("");
		channel->broadcastAll(sender, 2, "MODE (password)", "This channel does not have a password anymore.");
	}
}

void set_op(Channel* channel, bool b, std::string user, Server &server, Client& sender)
{
	Client* client = server.find_client(user);
	if (b)
	{
		if (!channel->is_member(client)) {	
			sendErrorMsg(sender.get_fd(), ERR_USERNOTINCHANNEL, user.c_str(), ERR_USERNOTINCHANNEL_MSG, NULL);
			return ;
		}
		channel->promote(client);
		channel->broadcastAll(&sender, 3, "MODE (operator)", user.c_str(), "has been promoted to operator");
	}
	else
	{
		if (!channel->is_operator(client)) {	
			sendErrorMsg(sender.get_fd(), ERR_USERNOTINCHANNEL, user.c_str(), ERR_USERNOTINCHANNEL_MSG, NULL);
			return ;
		}
		channel->demote(client);
		channel->broadcastAll(&sender, 3, "MODE (operator)", user.c_str(), "has been demoted to simple member");
	}
	channel->update_members_in_channel();
}

void set_limit(Channel* channel, bool b, Client& sender, std::string str_limit, std::vector<std::string> arguments)
{
	if (b)
	{
		int limit = atoi(str_limit.c_str());
		if (limit < 1)
		{
			sendErrorMsg(sender.get_fd(), ERR_UNKNOWNERROR, arguments[1].c_str(), "MODE (limit)", ":Invalid parameter", NULL);
			return ;
		}
		channel->set_limit(limit);
		channel->broadcastAll(&sender, 3, "MODE (limit)", "Channel now have a member limit of:", str_limit.c_str());
	}
	else
	{
		channel->set_limit(0);
		channel->broadcastAll(&sender, 2, "MODE (limit)", "Channel now have no member limit");
	}
}

bool checkup(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, ERR_NOTREGISTERED_MSG, NULL);
		return false;
	}
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return false;
	}
	if (arguments[1][0] == '#' || arguments[1][0] == '&')
		arguments[1].erase(0, 1);	
	else {
		sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
		return false;
	}
	Channel * channel = server.getChannel(arguments[1]);
	if (!channel) {
		sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, arguments[1].c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
		return false;
	}
	if (arguments.size() < 3) {
		//sendReplyMsg(sender.get_fd(), RPL_CHANNELMODEIS, arguments[1].c_str(), RPL_CHANNELMODEIS_MSG, NULL);
		return false;
	}
	if (!channel->is_operator(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_CHANOPRIVSNEEDED, arguments[1].c_str(), ERR_CHANOPRIVSNEEDED_MSG, NULL);
		return false;
	}
	return true;
}


void Cmd_mode::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (checkup(server, sender, arguments) == false)
		return;
	arguments[1].erase(0, 1);
	Channel *channel = server.getChannel(arguments[1]);
	std::string mode = arguments[2];
	bool plus = true;
	size_t args_i = 2; // argument iterator for flags
	for (size_t i = 0; i < mode.length(); i++)
	{
		char c = mode[i];
		if (c == '+')
		{
			plus = true;
			continue;
		}
		if (c == '-')
		{
			plus = false;
			continue;
		}

		if ((c == 'k' && plus) || c == 'o' || (c == 'l' && plus))
		{
			args_i ++;
			if (args_i >= arguments.size())
			{
				sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, arguments[1].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
				return;
			}
		}

		if (c == 'i')
			set_invite(&sender, channel, plus);
		else if (c == 't')
			set_topic(&sender, channel, plus);
		else if (c == 'k')
			set_password(&sender, channel, plus, arguments[args_i]);
		else if (c == 'o')
			set_op(channel, plus, arguments[args_i], server, sender);
		else if (c == 'l')
			set_limit(channel, plus, sender, arguments[args_i], arguments);
		else
			sendErrorMsg(sender.get_fd(), ERR_UNKNOWNERROR, arguments[1].c_str(), "MODE", ":Unknowed mode", NULL);
	}
}
