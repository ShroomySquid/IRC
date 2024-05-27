#ifndef SERVER_HPP
#define SERVER_HPP

#include "IRC.hpp"
#include "ResponseHandler.hpp"

#ifdef DEBUG
#define DEBUG_PRINT(msg) std::cout << "DEBUG: " << msg << std::endl
#else
#define DEBUG_PRINT(msg)
#endif

class Server
{
    private:
        char recv_buffer[1024];
		char buffer[1024];
		int buffer_len;
        int socketD;
        struct sockaddr_in *address;
        std::string password;
        std::map<std::string, Command*> commands;
        std::map<int, Client*> clients;
        std::map <std::string, Channel*> channels;
        struct sockaddr_in clientAddress;
        bool need_to_remove_client;
        unsigned int clientAddressSize;
        // -------- Private Methods -----
        bool is_IRC_message(const std::string& message);
        void remove_client(std::map<int, Client*> &clients);
        void process_message(Server &server, Client &sender, std::map<std::string, Command*>& commands, char* input);
        void free_channel();
        void mark_and_remove_disconnected_clients();

        // ---------------- George added and modified ----------------
        // pollfd pfd; // replaced with vector
        // int infd; // replaced with vector
        // void login_attempt(std::map<int, Client*> &clients, int infd); // replaced with RegisterClient
        std::vector<pollfd> pfds;
        void ListenClients();
        void AcceptClients();
        void CreatePollfds(std::vector<pollfd>& pfds);
        void PollAndProcessClients();
        void ProcessClientMessage(const pollfd& pfd);
        void MarkAndRemoveDisconnectedClients();
        void initializeCommands();
        void initializeBindings(int socketD, struct sockaddr_in *address);
		bool append_buffer(void);
        void registerClient(std::map<int, Client*> &clients, Client* received_client);
		void Split_message(Client* client, char *buffer);

        // ------------------------------
    public:
        Server(int socketD, struct sockaddr_in *address, std::string password);
        ~Server();
        void Run();
        // ----- Channels methods
        Channel* getChannel(std::string name);
        void addChannel(std::string name,Channel *c);
        // -----------
		std::string get_password() const;
		std::map<int, Client*>& get_clients();
		Client* find_client(std::string client_name);
};


#endif
