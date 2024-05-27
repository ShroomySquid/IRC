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
    Channel* channel = server.getChannel(channelname);

    if (channel == NULL)
    {
        //sendErrorMsg(sender.get_fd(), "403", "ERR_NOSUCHCHANNEL", ": No such channel\r\n");
        std::string message = "403 ERR_NOSUCHCHANNEL : No such channel\r\n";
        send(sender.get_fd(), message.c_str(), message.length(), 0);
        return;
    }

    if (channel->getMember_by_name(sender.get_username()) == NULL)
    {
        std::string message = "442 ERR_NOTONCHANNEL : User not on channel\r\n";
        send(sender.get_fd(), message.c_str(), message.length(), 0);
        return;
    }
    if (channel->getMember_by_name(user))
    {
        std::string message = "443 ERR_USERONCHANNEL : User already on channel\r\n";
        send(sender.get_fd(), message.c_str(), message.length(), 0);
        return;
    }

    std::string message1 = sender.get_username() + " wants to invite you in channel " + channelname + "\r\n";
    Client *invited = server.find_client(user);
    if(invited == NULL)
    {
        std::cout << "bug, impossible to find client" << std::endl;
        return;
    }
    send(invited->get_fd(), message1.c_str(), message1.length(), 0);
    std::string message2 = "341 RPL_INVITING : Invite sent to " + user + " to join channel " + channelname + "\r\n";
    send(sender.get_fd(), message2.c_str(), message2.length(), 0);
    channel->addInvited(invited);

}