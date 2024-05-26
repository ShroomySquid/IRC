#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include <cstdarg>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <ctime>

#define MAX_RESPONSE_SIZE 512
#define PREFIX ":ft_irc"
#define ERROR_CODE "461"
#define ERROR_NOTENOUGHPARAM ":Not enough parameters"
#define RPL_WELCOME "001 RPL_WELCOME"
#define WELCOME_MSG ":Welcome to the IRC server"
#define ERROR_MAYNOTREREGISTER ":You may not reregister"
#define ERR_PASSWDMISMATCH ":Password incorrect"

void sendErrorMsg(int fd, ...);
void sendReplyMsg(int fd, ...);
std::string current_timestamp();
void sendServerMsg(const char* format, ...);

#endif
