#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
    private:
        int fd;
        bool disconnected;
        Client();
    public:
        Client(int fd);
        ~Client();
        Client& operator=(const Client& src);
        Client(const Client& src);
        int get_fd();
        bool is_disconnected();
        void disconnect();
};

#endif