#include "../../inc/Command.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/ResponseHandler.hpp"

Cmd_ping::Cmd_ping(){}
Cmd_ping::~Cmd_ping(){}

void Cmd_ping::execute(Server &server, Client& sender, std::vector<std::string> arguments) {
    (void)arguments;
    (void)server;
    sendServerMsg("Received PING from fd: %d", sender.get_fd());
    sendReplyMsg(sender.get_fd(), "PONG", NULL);
}
