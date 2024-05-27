#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include <cstdarg>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <vector>

#ifdef DEBUG
#define DEBUG_PRINT(msg) std::cout << "DEBUG: " << msg << std::endl
#else
#define DEBUG_PRINT(msg)
#endif

#define MAX_RESPONSE_SIZE 512
#define PREFIX ":ft_irc"

#define RPL_WELCOME "001 RPL_WELCOME"
#define WELCOME_MSG ":Welcome to the IRC server"
#define RPL_CHANNELMODEIS "324"
#define RPL_CHANNELMODEIS_MSG "+i/-i: Add/remove channel on invitation\n +t-t: Add/remove restriction on topic\n +k/-k: Add/remove channel password\n +o/-o: Promote/demote member as operator\n +l/-l Add/remove limit of users in channel"
#define ERR_UNKNOWNERROR "400"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_NOSUCHCHANNEL_MSG ":No such channel"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_NONICKNAMEGIVEN_MSG ":No nickname given"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_ERRONEUSNICKNAME_MSG ":Erroneus nickname"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NICKNAMEINUSE_MSG ":Nickname is already in use"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_USERNOTINCHANNEL_MSG ":You're not on that channel"
#define ERR_NOTREGISTERED "451"
#define ERR_NOTREGISTERED_MSG ":You have not registered"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_NEEDMOREPARAMS_MSG ":Not enough parameters"
#define ERR_ALREADYREGISTERED "462"
#define ERR_ALREADYREGISTERED_MSG ":You may not reregister"
#define ERR_PASSWDMISMATCH "464"
#define ERR_PASSWDMISMATCH_MSG ":Password incorrect"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_CHANOPRIVSNEEDED_MSG ":You're not channel operator"

void sendErrorMsg(int fd, ...);
void sendReplyMsg(int fd, ...);
std::string current_timestamp();
void sendServerMsg(const char* format, ...);

#endif
