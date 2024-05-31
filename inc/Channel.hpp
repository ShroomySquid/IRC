#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "Client.hpp"
# include <vector> 
# include <algorithm>
# include <sstream>
class Client;
class Channel
{
    private:
        Channel();
        std::string name;
        std::vector<Client*> members;
        std::vector<Client*> operators;
		std::vector<Client*> invited;
		std::string topic;
		bool topic_protection;
		bool on_invite;
		int limit;
		int clients_in_channel;
		std::string password;
    public:
        Channel(std::string name);
        ~Channel();
        Channel(const Channel& src);
		std::vector<Client*> get_members();
        void removeClient(Client* client);
		bool demote(Client* c);
		bool promote(Client* c);
		bool is_member(Client* c);
		bool is_operator(Client* c);
        bool addClient(Client* c, bool ope);
        bool addInvited(Client* c);
		bool is_Invited(Client* c);
		void broadcastAll(int count, ...);
		void broadcastAlmostAll(Client* sender, int count, ...);
        
		Client * getMember_by_name(std::string name);
		
		std::string get_topic(void);
		void set_topic(std::string new_topic);
		bool is_topic_protected(void);
		void set_topic_protected(bool protect_val);
		bool is_on_invite(void);
		void set_invite(bool invite_val);
		std::string get_password(void);
		void set_password(std::string new_password);
		int get_clients_nbr(void);
		int	get_limit(void);
		std::string	get_name(void);
		void set_limit(int new_limit);
};

#endif
