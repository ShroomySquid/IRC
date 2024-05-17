#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "Client.hpp"
# include <vector> 

class Client;
class Channel
{
    private:
        Channel();
        std::string name;
        std::vector<Client*> members;
        // Map of all channels
        static std::map <std::string, Channel*> channels;
        // ---------------------
    public:
        // Get a specific channel by name
        static Channel* getChannel(std::string name);
        static void addChannel(std::string name,Channel *c);
        static void free_channel();
        // -----------
        Channel(std::string name);
        ~Channel();
        Channel& operator=(const Channel& src);
        Channel(const Channel& src);
        bool addClient(Client* client);
        void broadcastAll(Client &sender, std::string message);
};

#endif
