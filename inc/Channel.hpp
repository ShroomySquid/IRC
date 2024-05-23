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
		std::string topic;
		bool topic_protection;
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
		void broadcastCmd(std::string cmd, std::string arg);
        
		Client * getMember_by_name(std::string name);
		
		std::string get_topic(void);
		void set_topic(std::string new_topic);
		bool is_topic_protected(void);
		void set_topic_protected(bool is_protected);
};

#endif
