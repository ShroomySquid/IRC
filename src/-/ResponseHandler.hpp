#include <string>
#include <iostream>

enum IRCErrorCode {
    RPL_WELCOME = 001,
    RPL_YOURHOST = 002,
    RPL_CREATED = 003,
    RPL_MYINFO = 004,
    RPL_BOUNCE = 005,

    RPL_USERHOST = 302,
    RPL_ISON = 303,
    RPL_AWAY = 301,
    RPL_UNAWAY = 305,
    RPL_NOWAWAY = 306,

    RPL_WHOISUSER = 311,
    RPL_WHOISSERVER = 312,
    RPL_WHOISOPERATOR = 313,
    RPL_WHOISIDLE = 317,
    RPL_ENDOFWHOIS = 318,
    RPL_WHOISCHANNELS = 319,

    RPL_LISTSTART = 321,
    RPL_LIST = 322,
    RPL_LISTEND = 323,

    RPL_CHANNELMODEIS = 324,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_INVITING = 341,
    RPL_SUMMONING = 342,

    RPL_VERSION = 351,
    RPL_WHOREPLY = 352,
    RPL_ENDOFWHO = 315,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,

    RPL_LINKS = 364,
    RPL_ENDOFLINKS = 365,
    RPL_BANLIST = 367,
    RPL_ENDOFBANLIST = 368,
    RPL_INFO = 371,
    RPL_ENDOFINFO = 374,
    RPL_MOTDSTART = 375,
    RPL_MOTD = 372,
    RPL_ENDOFMOTD = 376,

    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_TOOMANYCHANNELS = 405,
    ERR_WASNOSUCHNICK = 406,

    ERR_TOOMANYTARGETS = 407,
    ERR_NOORIGIN = 409,
    ERR_NORECIPIENT = 411,
    ERR_NOTEXTTOSEND = 412,
    ERR_NOTOPLEVEL = 413,
    ERR_WILDTOPLEVEL = 414,

    ERR_UNKNOWNCOMMAND = 421,
    ERR_NOMOTD = 422,
    ERR_NOADMININFO = 423,
    ERR_FILEERROR = 424,

    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NICKCOLLISION = 436,

    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL = 443,
    ERR_NOLOGIN = 444,
    ERR_SUMMONDISABLED = 445,
    ERR_USERSDISABLED = 446,

    ERR_NOTREGISTERED = 451,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    ERR_NOPERMFORHOST = 463,
    ERR_PASSWDMISMATCH = 464,
    ERR_YOUREBANNEDCREEP = 465,
    ERR_YOUWILLBEBANNED = 466,
    ERR_KEYSET = 467,

    ERR_CHANNELISFULL = 471,
    ERR_UNKNOWNMODE = 472,
    ERR_INVITEONLYCHAN = 473,
    ERR_BANNEDFROMCHAN = 474,
    ERR_BADCHANNELKEY = 475,
    ERR_BADCHANMASK = 476,
    ERR_NOCHANMODES = 477,
    ERR_BANLISTFULL = 478,

    ERR_NOPRIVILEGES = 481,
    ERR_CHANOPRIVSNEEDED = 482,
    ERR_CANTKILLSERVER = 483,

    ERR_RESTRICTED = 484,
    ERR_UNIQOPPRIVSNEEDED = 485,

    ERR_NOOPERHOST = 491,
    ERR_UMODEUNKNOWNFLAG = 501,
    ERR_USERSDONTMATCH = 502,
};

class IRCResponse {
public:
    // Constructor
    IRCResponse(IRCErrorCode code, const std::string& target, const std::string& message)
        : code(code), target(target), message(message) {}

    enum ResponseType { ERROR, REPLY };

    // Getters
    IRCErrorCode getCode() const { return code; }
    const std::string& getTarget() const { return target; }
    const std::string& getMessage() const { return message; }

    static IRCResponse createError(IRCErrorCode code, const std::string& target, const std::string& error) {
        return IRCResponse(ERROR, code, target, "", error); // Empty nickname for errors
    }

    static IRCResponse createReply(IRCErrorCode code, const std::string& target, const std::string& nickname, const std::string& message) {
        return IRCResponse(REPLY, code, target, nickname, message);
    }

    std::string getFormattedResponse() const{
        return ":" + this->prefix + " "
            + std::to_string(this->code) + " "
            + this->target + " :"
            + this->message
            + "\r\n";
    }

    // Setters
    void setCode(IRCErrorCode newCode) { code = newCode; }
    void setTarget(const std::string& newTarget) { target = newTarget; }
    void setMessage(const std::string& newMessage) { message = newMessage; }
    void setPrefix(const std::string& newPrefix) { prefix = newPrefix; }

private:
    IRCErrorCode code;
    std::string target;
    std::string message;
    std::string prefix = "ft_irc";
};
