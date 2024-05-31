#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_join::Cmd_join(){}
Cmd_join::~Cmd_join(){}

// send response message to the client back
// channel message + topic + list of users
void send_join_response_msg(Client& sender, Channel* channel)
{
	std::vector<Client*> members = channel->get_members();
	std::vector<Client*>::iterator it = members.begin();
	std::string clients_nicknames = "";
	for (it = members.begin(); it != members.end(); it++)
	{
		clients_nicknames += " ";
		if (channel->is_operator(*(it)))
			clients_nicknames += "@";
		clients_nicknames += (*it)->get_nickname();
	}
	// for each operator
	// for (it = members.begin(); it != members.end(); it++)
	// {
	// 	clients_nicknames += " ";
	// 	if (channel->is_operator(*(it)))
	// 		clients_nicknames += "@";
	// 	clients_nicknames += (*it)->get_nickname();
	// }

	std::string joinmessage = ":" + sender.get_client() + " JOIN " + channel->get_name() + "\r\n";

	for (it = members.begin(); it != members.end(); it++)
	{
		Client* c = (*it);
		send(c->get_fd(), joinmessage.c_str(), joinmessage.length(),0);
		if (channel->get_topic() != "")
			sendReplyMsg(c->get_fd(), RPL_TOPIC, c->get_client().c_str(), channel->get_name().c_str(), channel->get_topic(), NULL);

		sendReplyMsg(c->get_fd(), RPL_NAMREPLY, c->get_client().c_str(), "=", channel->get_name().c_str(), ":",clients_nicknames.c_str(), NULL);
		sendReplyMsg(c->get_fd(), RPL_ENDOFNAMES,c->get_client().c_str(),channel->get_name().c_str(),":End of /NAMES list",  NULL);
	}
}

void Cmd_join::fill_chan_to_join(std::map<std::string, std::string> &chan_to_join, std::string channels, std::string passwords) {
	int chan_start = 0;
	int len = 0;
	int pass_start = 0;
	int pass_len = 0;
	std::string chan_name;
	std::string pass_name;
	while (channels[chan_start]) {
		len = 0;
		if (channels[chan_start] == '#' || channels[chan_start] == '&')
			chan_start++;
		while (channels[chan_start + len] && channels[chan_start + len] != ',')
			len++;
		if (len > 0)
			chan_name = channels.substr(chan_start, len);
		if (passwords == "" && len > 0)
			chan_to_join[chan_name] = "";
		else if (len > 0) {
			pass_len = 0;
			while (passwords[pass_start + pass_len] && passwords[pass_start + pass_len] != ',')
				pass_len++;
			if (pass_len == 0)
				pass_name = "";
			else
				pass_name = passwords.substr(pass_start, pass_len);
			chan_to_join[chan_name] = pass_name;
			pass_start += pass_len;
			if (passwords[pass_start] && passwords[pass_start] == ',')
				pass_start++;
		}
		chan_start += len;
		if (channels[chan_start] && channels[chan_start] == ',')
			chan_start++;
	}
}

void Cmd_join::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	bool is_operator;
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
	std::map<std::string, std::string> chan_to_join;
	if (arguments.size() < 3)
		fill_chan_to_join(chan_to_join, arguments[1], "");
	else
		fill_chan_to_join(chan_to_join, arguments[1], arguments[2]);
	std::map<std::string, std::string>::iterator it;
	for (it = chan_to_join.begin(); it != chan_to_join.end(); it++) {
		cout << it->first << " " << it->second << endl;
		is_operator = false;
		Channel *channel = server.getChannel(it->first);
		if (channel == NULL)
		{
			//std::cout << "Creating new channel" << std::endl;
			channel = new Channel(it->first);
			server.addChannel(it->first, channel);
			is_operator = true;
			if (it->second != "")
				channel->set_password(it->second);
			channel->addClient(&sender, is_operator);
			cout << "Channel created: " << it->first << endl;
			send_join_response_msg(sender, channel);
			//broadcast all the new channel created
			continue ;
		}
		if (channel->get_password() != "" && channel->get_password() != it->second) {
			sendErrorMsg(sender.get_fd(), ERR_BADCHANNELKEY, sender.get_client().c_str(), arguments[1].c_str(), ERR_BADCHANNELKEY_MSG, NULL);
			continue ;
		}
		if (channel->is_on_invite() && !channel->is_Invited(&sender)) {
			sendErrorMsg(sender.get_fd(), ERR_INVITEONLYCHAN, sender.get_client().c_str(), arguments[1].c_str(), ERR_INVITEONLYCHAN_MSG, NULL);
			continue ;
		}
		if (channel->get_limit() > 0 && channel->get_limit() <= channel->get_clients_nbr()) {
			sendErrorMsg(sender.get_fd(), ERR_CHANNELISFULL, sender.get_client().c_str(), arguments[1].c_str(), ERR_CHANNELISFULL_MSG, NULL);
			continue ;
		}
		bool success = channel->addClient(&sender, is_operator);
		if (success) {
			send_join_response_msg(sender, channel);
			channel->broadcastAll(2, sender.get_nickname().c_str(), "has joined the channel");
		}
	}
}
