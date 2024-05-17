#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <string>
#include <vector>
#include "Client.hpp"
#include <string>
#include <sys/socket.h>
#include <map>
#include <algorithm>

class Channel
{
    private:
        Channel();
        std::string name;
        std::vector<std::string> clients;
        // Map of all channels
        static std::map <std::string, Channel*> channels;
        // ---------------------
    public:
        // Get a specific channel by name
        static Channel* getChannel(std::string name);
        // -----------
        Channel(std::string name);
        ~Channel();
        Channel& operator=(const Channel& src);
        Channel(const Channel& src);
        bool addClient(std::string name);
        void broadcastAll(Client &sender, std::string message);
};

#endif