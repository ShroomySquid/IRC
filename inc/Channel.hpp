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
    public:
        Channel(std::string name);
        ~Channel();
        Channel& operator=(const Channel& src);
        Channel(const Channel& src);
        bool addClient(Client* client);
        void removeClient(Client* client);
        void broadcastAll(Client &sender, std::string message);
        Client * getMember_by_name(std::string name);
};

#endif
