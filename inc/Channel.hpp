#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <string>
#include <vector>

class Channel
{
    private:
        Channel();
        std::string name;
        std::vector<std::string> clients;

    public:
        Channel(std::string name);
        ~Channel();
        Channel& operator=(const Channel& src);
        Channel(const Channel& src);
        void addClient(std::string name);
};

#endif