#include "../inc/Bot.hpp"

Bot::Bot(int fd, const std::string& ip, int port, const std::string& nickname)
    : Client(fd, ip, port), nickname(nickname) {}

void Bot::handleMessage(const std::string& message) {
    if (message.find("PING") == 0) {
        send("PONG " + message.substr(5));
    }
}

void Bot::send(const std::string& message) {
    std::string msg_with_newline = message + "\r\n";
    if (::send(get_fd(), msg_with_newline.c_str(), msg_with_newline.size(), 0) == -1) {
        perror("send");
    }
}
