#include "../../inc/Command.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Cmd_cap::Cmd_cap(){}
Cmd_cap::~Cmd_cap(){}

void Cmd_cap::execute(Server &server, Client& sender, std::vector<std::string> arguments)
{
    (void)server;
    (void)arguments;
    std::string cap_msg = "CAP LS 302\r\n";
    send(sender.get_fd(),cap_msg.c_str(), cap_msg.size(), 0);
}
