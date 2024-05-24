#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include <string>

struct ErrorMessage {
    std::string num;
    std::string msg;
    std::string error;
};

// Function Prototype
std::string createErrorMessage(const ErrorMessage& errorMsg);

// Error Code Macros
#define ERR_NOSUCHNICK ErrorMessage("433", "*", "Nickname is already in use.")
#define ERR_NOSUCHCHANNEL ErrorMessage("403", "*", "No such channel")

void SendErrorMessage(const ErrorMessage& errorMsg);

#endif
