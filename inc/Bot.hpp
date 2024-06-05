#ifndef BOT_HPP
#define BOT_HPP

#include "IRC.hpp"
#include "Client.hpp"
#include "ResponseHandler.hpp"
#include <string>
#include <unistd.h>
#include <sys/socket.h>

class Bot : public Client {
public:
    Bot(int fd, const std::string& ip, int port, const std::string& nickname);
    void handleMessage(const std::string& message);
    void send(const std::string& message);

private:
    std::string nickname;
};

#endif
