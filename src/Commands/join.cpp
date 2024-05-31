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

	//for (it = members.begin(); it != members.end(); it++)
	//{
		//Client* c = (*it);
		send(sender.get_fd(), joinmessage.c_str(), joinmessage.length(),0);
		if (channel->get_topic() != "")
			sendReplyMsg(sender.get_fd(), RPL_TOPIC, sender.get_client().c_str(), channel->get_name().c_str(), channel->get_topic().c_str(), NULL);

		sendReplyMsg(sender.get_fd(), RPL_NAMREPLY, sender.get_client().c_str(), "=", channel->get_name().c_str(), ":",clients_nicknames.c_str(), NULL);
		sendReplyMsg(sender.get_fd(), RPL_ENDOFNAMES,sender.get_client().c_str(),channel->get_name().c_str(),":End of /NAMES list",  NULL);
	//}
}

void Cmd_join::fill_chan_to_join(std::vector<std::string> &chan_to_join, std::string channels) {
	int chan_start = 0;
	int len = 0;
	std::string chan_name;
	while (channels[chan_start]) {
		while (channels[chan_start] && channels[chan_start] == ',')
			chan_start++;
		len = 0;
		if (channels[chan_start] == '#' || channels[chan_start] == '&')
			chan_start++;
		while (channels[chan_start + len] && channels[chan_start + len] != ',')
			len++;
		if (len > 0) {
			chan_name = channels.substr(chan_start, len);
			chan_to_join.push_back(chan_name);
		}
		chan_start += len;
	}
}

int Cmd_join::init(Client& sender, std::vector<std::string> arguments) {
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return (1);
	}
	if (arguments.size() < 2) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return (1);
	}
	return (0);
}

int Cmd_join::check_errors(int *pass_start, Channel* channel, std::string passwords, Client& sender) {
	if (channel->get_password() != "") {
		std::string pass_name = return_pass(pass_start, passwords);
		if (channel->get_password() != pass_name) {
			sendErrorMsg(sender.get_fd(), ERR_BADCHANNELKEY, sender.get_client().c_str(), channel->get_name().c_str(), ERR_BADCHANNELKEY_MSG, NULL);
			return (1) ;
		}
	}
	if (channel->is_on_invite() && !channel->is_Invited(&sender)) {
		sendErrorMsg(sender.get_fd(), ERR_INVITEONLYCHAN, sender.get_client().c_str(), channel->get_name().c_str(), ERR_INVITEONLYCHAN_MSG, NULL);
		return (1);
	}
	if (channel->get_limit() > 0 && channel->get_limit() <= channel->get_clients_nbr()) {
		sendErrorMsg(sender.get_fd(), ERR_CHANNELISFULL, sender.get_client().c_str(), channel->get_name().c_str(), ERR_CHANNELISFULL_MSG, NULL);
		return (1);
	}
	return (0);
}

std::string Cmd_join::return_pass(int *pass_start, std::string passwords) {	
	int pass_len = 0;
	std::string pass_name;
	while (passwords[*pass_start] && passwords[*pass_start] == ',')
		*pass_start += 1;
	while (passwords[*pass_start + pass_len] && passwords[*pass_start + pass_len] != ',')
		pass_len++;
	if (pass_len == 0)
		pass_name = "";
	else
		pass_name = passwords.substr(*pass_start, pass_len);	
	*pass_start += pass_len;
	return (pass_name);
}

void Cmd_join::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	bool is_operator;
	int pass_start = 0;
	std::string new_password;
	std::string passwords;
	if (init(sender, arguments))
		return ;
	std::vector<std::string> chan_to_join;
	if (arguments.size() < 3)
		passwords = "";
	else
		passwords = arguments[2];
	fill_chan_to_join(chan_to_join, arguments[1]);
	std::vector<std::string>::iterator it;
	for (it = chan_to_join.begin(); it != chan_to_join.end(); it++) {
		is_operator = false;
		Channel *channel = server.getChannel(*it);
		if (channel == NULL)
		{
			channel = new Channel(*it);
			server.addChannel(*it, channel);
			is_operator = true;
			channel->set_password(return_pass(&pass_start, passwords));
			channel->addClient(&sender, is_operator);
			cout << "Channel created: " << *it << endl;
			send_join_response_msg(sender, channel);
			//broadcast all the new channel created
			continue ;
		}
		if (check_errors(&pass_start, channel, passwords, sender))
			continue ;
		bool success = channel->addClient(&sender, is_operator);
		if (success) {
			send_join_response_msg(sender, channel);
			channel->broadcastAll(&sender, 2, sender.get_nickname().c_str(), " has joined the channel");
		}
	}
}
