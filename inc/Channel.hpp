#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "Client.hpp"
# include <vector> 
# include <algorithm>
class Client;
class Channel
{
    private:
        Channel();
        std::string name;
        std::vector<Client*> members;
        std::vector<Client*> operators;
    public:
        Channel(std::string name);
        ~Channel();
        Channel& operator=(const Channel& src);
        Channel(const Channel& src);
        void removeClient(Client* client);
	bool promote(Client* client);
	bool is_member(Client* client);
	bool is_operator(Client* client);
        bool addClient(Client* client, bool ope);
        void broadcastAll(Client &sender, std::string message);
        Client * getMember_by_name(std::string name);
};

#endif
