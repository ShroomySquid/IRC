#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_cap::Cmd_cap(){}
Cmd_cap::~Cmd_cap(){}

void Cmd_cap::execute(Server &server, Client& sender, std::vector<std::string> arguments) {
    (void)server;
    (void)arguments;
    sendReplyMsg(sender.get_fd(), "CAP LS 302\r\n", NULL);
}
