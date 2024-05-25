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
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	return buf;
}

void sendServerMsg(int fd, const char* format, ...) {
    va_list args;
    va_start(args, format);

    // Format message (C98 compliant)
    char messageBuffer[1024]; // Adjust buffer size as needed
    vsnprintf(messageBuffer, sizeof(messageBuffer), format, args);
    va_end(args);

    // Add timestamp (C98 string concatenation)
    std::string timestamp = current_timestamp(); // Assuming you have a function to get this
    std::string fullMessage = timestamp + " " + messageBuffer + "\n";

    // Send to server (replace with your actual network logic)
    write(fd, fullMessage.c_str(), strlen(fullMessage.c_str()));
}
