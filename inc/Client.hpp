#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "IRC.hpp"

class Client
{
    private:
        const int _fd;
		std::string _username;
		std::string _nickname;
        bool _disconnected;
		bool _authentified;
		bool _registered;
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
		void set_username(std::string user);
		void set_nickname(std::string nick);
        bool is_disconnected();
		bool is_authentified();
		bool is_registered();
		void regist();
		void authentify();
        void disconnect();
};

#endif
