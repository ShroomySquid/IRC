#ifndef SERVER_HPP
#define SERVER_HPP

#include "IRC.hpp"

class Server
{
    private:
        char buffer[1024];
        int socketD;
        struct sockaddr_in *address;
        std::string password;
        std::map<std::string, Command*> commands;
        std::map<int, Client*> clients;
        pollfd pfd;
        int infd;
        struct sockaddr_in clientAddress;
        bool need_to_remove_client;
        unsigned int clientAddressSize;
        // -------- Private Methods -----
        void login_attempt(std::map<int, Client*> &clients, int infd);
        bool is_IRC_message(const std::string& message);
        void remove_client(std::map<int, Client*> &clients);
        void process_message(Server &server, Client &sender, std::map<std::string, Command*>& commands, std::string input);
        // ------------------------------
    public:
        Server(int socketD, struct sockaddr_in *address, std::string password);
        ~Server();
        void Run();

};


#endif