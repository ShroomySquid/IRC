#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>

class Client
{
    private:
        int fd;
        bool disconnected;
        Client();
        std::string name;
    public:
        Client(std::string name, int fd);
        ~Client();
        Client& operator=(const Client& src);
        Client(const Client& src);
        int get_fd();
        bool is_disconnected();
        void disconnect();
        std::string getName();
};

#endif