#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"

Cmd_privmsg::Cmd_privmsg(){}
Cmd_privmsg::~Cmd_privmsg(){}

void Cmd_privmsg::parse_targets(std::vector<std::string> &target_vec, std::string targets) {
	int target_start = 0;
	int len = 0;
	std::string target_name;
	while (targets[target_start]) {
		while (targets[target_start] && targets[target_start] == ',')
			target_start++;
		len = 0;
		while (targets[target_start + len] && targets[target_start + len] != ',')
			len++;
		if (len > 0) {
			target_name = targets.substr(target_start, len);
			target_vec.push_back(target_name);
		}
		target_start += len;
	}
}

void Cmd_privmsg::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	int args_nbr = arguments.size();
	int i = 4;
	if (!sender.is_registered()) {
		sendErrorMsg(sender.get_fd(), ERR_NOTREGISTERED, sender.get_client().c_str(), ERR_NOTREGISTERED_MSG, NULL);
		return ;
	}
	if (args_nbr < 3) {
		sendErrorMsg(sender.get_fd(), ERR_NEEDMOREPARAMS, sender.get_client().c_str(), arguments[0].c_str(), ERR_NEEDMOREPARAMS_MSG, NULL);
		return ;
	}
	std::vector<std::string> targets;
	parse_targets(targets, arguments[1]);
	std::vector<std::string>::iterator it;
	std::string args = arguments[2];
	while (i < args_nbr) {
		args += " ";
		args += arguments[i];
		i++;
	}
	for (it = targets.begin(); it != targets.end(); it++) {
		if ((*it)[0] == '#' || (*it)[0] == '&')
		{
			it->erase(0, 1);
			Channel* channel = server.getChannel(*it);
			if (channel == NULL)
				sendErrorMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, sender.get_client().c_str(), (*it).c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
			else if (channel->is_member(&sender)) {
				sendServerMsg("PRIVMSG sent from %s to %s", sender.get_client().c_str(), (*it).c_str(), NULL);
				channel->broadcastAlmostAll(&sender, 2, sender.get_client().c_str(), args.c_str());
			}
			else
				sendErrorMsg(sender.get_fd(), ERR_CANNOTSENDTOCHAN, sender.get_client().c_str(), (*it).c_str(), ERR_CANNOTSENDTOCHAN_MSG, NULL);
			continue ;
		}
		Client* client = server.find_client(*it);
		if (client == NULL) {
			sendErrorMsg(sender.get_fd(), ERR_NOSUCHNICK, sender.get_client().c_str(), (*it).c_str(), ERR_NOSUCHNICK_MSG, NULL);
			continue ;
		}
		if (client->get_fd() != sender.get_fd()) {
			sendServerMsg("PRIVMSG sent from %s to %s", sender.get_client().c_str(), (*it).c_str(), NULL);
			sendMsg(sender.get_client(), client->get_fd(), sender.get_client().c_str(), args.c_str(), NULL);
		}
	}
}
