#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_invite::Cmd_invite(){}
Cmd_invite::~Cmd_invite(){}

void Cmd_invite::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
	if (!sender.is_registered()) {
		not_registered_yet(sender.get_fd());
		return ;
	}
	if (arguments.size() <= 2)
		return;

    std::string user = arguments[1];
    std::string channelname = arguments[2];
    if (channelname[0] == '#' || channelname[0] == '&')
    {
        channelname = channelname.substr(1, channelname.length());
    }
    Channel* channel = server.getChannel(channelname);

    if (channel == NULL)
    {
        //sendErrorMsg(sender.get_fd(), "403", "ERR_NOSUCHCHANNEL", ": No such channel\r\n");
        sendReplyMsg(sender.get_fd(), ERR_NOSUCHCHANNEL, sender.get_nickname().c_str(), channelname.c_str(), ERR_NOSUCHCHANNEL_MSG, NULL);
        return;
    }

    if (channel->getMember_by_name(sender.get_client()) == NULL)
    {
        sendReplyMsg(sender.get_fd(), ERR_NOTONCHANNEL, sender.get_nickname().c_str(), channel->get_name().c_str(), ERR_NOTONCHANNEL_MSG, NULL);
        return;
    }
    if (channel->getMember_by_name(user))
    {
        //   "<client> <nick> <channel> :is already on channel"
        sendReplyMsg(sender.get_fd(), ERR_USERONCHANNEL, sender.get_nickname().c_str(), channel->get_name().c_str() ,ERR_USERONCHANNEL_MSG, NULL);
        return;
    }

    Client *invited = server.find_client(user);
    if(invited == NULL)
    {
        std::cout << "impossible to find client" << std::endl;
        return;
    }

    std::string message1 = (":"+sender.get_nickname() + " PRIVMSG " + sender.get_client() + " wants to invite you in channel: " + channelname + "\r\n");
    send(invited->get_fd(), message1.c_str() , message1.length(),0);
    sendReplyMsg(sender.get_fd(), RPL_INVITING, sender.get_client().c_str(), invited->get_client().c_str(), ("#"+channelname).c_str(), NULL);
    channel->addInvited(invited);

}
