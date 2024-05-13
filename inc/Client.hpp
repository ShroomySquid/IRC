#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
    private:
        int fd;
        Client();
    public:
        Client(int fd);
        ~Client();
        Client& operator=(const Client& src);
        Client(const Client& src);
};

#endif