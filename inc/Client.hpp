#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "IRC.hpp"
class Client
{
    private:
        const int _fd;
		const std::string _username;
		const std::string _nickname;
        bool _disconnected;
        Client();
    public:
        Client(int fd);
        Client(int fd, std::string username, std::string nickname);
        ~Client();
        Client& operator=(const Client& src);
        Client(const Client& src);

        int get_fd();
		std::string get_username();
		std::string get_nickname();
        bool is_disconnected();
        void disconnect();
};

#endif
