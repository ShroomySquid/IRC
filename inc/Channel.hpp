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
		std::vector<Client*> invited;
		std::string topic;
		bool topic_protection;
		bool on_invite;
		int limit;
		std::string password;
    public:
        Channel(std::string name);
        ~Channel();
        Channel& operator=(const Channel& src);
        Channel(const Channel& src);

        void removeClient(Client* client);
		bool demote(Client* c);
		bool promote(Client* c);
		bool is_member(Client* c);
		bool is_operator(Client* c);
        bool addClient(Client* c, bool ope);
        bool addInvited(Client* c);
		bool is_Invited(Client* c);
		void broadcastAll(Client &sender, std::string message);
		void broadcastCmd(std::string cmd, std::string arg);
        
		Client * getMember_by_name(std::string name);
		
		std::string get_topic(void);
		void set_topic(std::string new_topic);
		bool is_topic_protected(void);
		void set_topic_protected(bool protect_val);
		bool is_on_invite(void);
		void set_invite(bool invite_val);
		std::string get_password(void);
		void set_password(std::string new_password);
		int	get_limit(void);
		void set_limit(int new_limit);
};

#endif
