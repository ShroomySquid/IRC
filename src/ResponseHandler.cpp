#include "../inc/ResponseHandler.hpp"

void sendErrorMsg(int fd, ...) {
    va_list args;
    va_start(args, fd);

	std::stringstream ss;
	ss << PREFIX;
	while (1) {
		const char *arg = va_arg(args, const char *);
		if (arg == NULL) {
			break;
		}
		ss << " " << arg;
	}
	ss << "\n";
	std::string response = ss.str();
	send(fd, response.c_str(), response.size(), 0);
	va_end(args);
}

void sendReplyMsg(int fd, ...) {
    va_list args;
    va_start(args, fd);

	std::stringstream ss;
	ss << PREFIX;
	while (1) {
		const char *arg = va_arg(args, const char *);
		if (arg == NULL) {
			break;
		}
		ss << " " << arg;
	}
	ss << "\n";
	std::string response = ss.str();
	send(fd, response.c_str(), response.size(), 0);
	va_end(args);
}

std::string current_timestamp() {
	std::time_t now = std::time(0);
    std::tm now_tm;
    localtime_r(&now, &now_tm);
    std::ostringstream oss;
    oss << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "]";
    return oss.str();
}

void sendServerMsg(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char messageBuffer[1024];
	vsnprintf(messageBuffer, sizeof(messageBuffer), format, args);
    va_end(args);

    std::string timestamp = current_timestamp();
    std::string fullMessage = timestamp + " " + messageBuffer + "\n";
    std::cout << fullMessage;
}
