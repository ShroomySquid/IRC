#include <cstdarg>
#include <iostream>
#include <sstream>
#include <sys/socket.h>

#define ERR_NEEDMOREPARAMS ":ft_irc 461 * PASS :Not enough parameters"

// Macro for error codes
#define ERROR_CODE "461"
#define PREFIX ":ft_irc"
#define MESSAGE "Not enough parameters"

// Error code enum
enum ErrorCode {
    ERR_NEEDMORE = 461,
};

// User class
class User {
    std::string _password;
    std::string _nick;
    std::string _user;
public:
    void setNick(const std::string& nick) { _nick = nick; }
    void setUser(const std::string& user) { _user = user; }
    void setPassword(const std::string& password) { _password = password; }
    std::string getPassword() const { return _password; }
    std::string getNick() const { return _nick; }
    std::string getUser() const { return _user; }
};

// Error message response class
class ErrorMessageResponse : public User {
public:
    std::string ErrorMessageResponseCstring(ErrorCode error_code, int safeguard, ...);
    std::string ErrorMessageResponseObject(ErrorCode error_code, const User& User, int safeguard, ...);
    void ErrorMessageResponseTemplate();
    template <typename T, typename... Args>
    void ErrorMessageResponseTemplate(T value, Args... args);
    std::string ErrorMessageResponseConstructor(const std::string& num, const std::string& msg, const std::string& error);
};

// Error message string
std::string ErrorMessageResponse::ErrorMessageResponseCstring(ErrorCode error_code, int safeguard, ...) {
    va_list args;
    va_start(args, safeguard);

    std::ostringstream oss;
    oss << PREFIX << " " << error_code << " :" << MESSAGE << " " << this->getUser();
    // oss << PREFIX << " " << error_code << " " << this->getNick() << " " << MESSAGE;

    char* arg;
    while ((arg = va_arg(args, char*)) != NULL) {
        oss << " " << arg;
    }

    va_end(args);
    return oss.str();
}

// Error message object
std::string ErrorMessageResponseObject(ErrorCode error_code, const User& user, int safeguard, ...) {
    va_list args;
    va_start(args, safeguard);

    std::ostringstream oss;
    oss << PREFIX << " " << error_code << " " << user.getUser() << " :" << MESSAGE;

    char* arg;
    while ((arg = va_arg(args, char*)) != NULL) {
        oss << " " << arg;
    }

    va_end(args);
    return oss.str();
}

// Error message template
void ErrorMessageResponseTemplate() { std::cout << std::endl; }

template <typename T, typename... Args>
void ErrorMessageResponseTemplate(T value, Args... args) {
    std::cout << value << " ";
    ErrorMessageResponseTemplate(args...);
}

void ErrorMessageResponseTemplate(const char* value, ...) {
    va_list args;
    va_start(args, value);

    const char* arg;
    while ((arg = va_arg(args, const char*)) != nullptr) {
        std::cout << arg << " ";
    }

    va_end(args);
    std::cout << std::endl;
}

// Error message constructor
std::string ErrorMessageResponse::ErrorMessageResponseConstructor(const std::string& num, const std::string& msg = "", const std::string& error = "") {
    std::ostringstream oss;

    oss << PREFIX << " " << num;
    if (!msg.empty()) {
        oss << " " << msg;
    }
    if (!error.empty()) {
        oss << " :" << error;
    }

    return oss.str();
}


int main() {
    User user;
    user.setUser("username");
    std::string username = user.getUser();

    ErrorMessageResponse error_message;

    std::string error_message_object = ErrorMessageResponseObject(ERR_NEEDMORE, user, 0, NULL);
    error_message.ErrorMessageResponseCstring(ERR_NEEDMORE, 0, username.c_str(), NULL);
    error_message.ErrorMessageResponseConstructor(ERROR_CODE, username, MESSAGE);

    std::cout << error_message_object << std::endl;
    std::cout << error_message.ErrorMessageResponseCstring(ERR_NEEDMORE, 0, username.c_str(), NULL) << std::endl;
    ErrorMessageResponseTemplate(PREFIX, ERR_NEEDMORE, username, ":", MESSAGE);
    std::cout << error_message.ErrorMessageResponseConstructor(ERROR_CODE, username, MESSAGE) << std::endl;

    return 0;
}



