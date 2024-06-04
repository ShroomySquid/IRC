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
	ss << "\r\n";
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
	ss << "\r\n";
	std::string response = ss.str();
	send(fd, response.c_str(), response.size(), 0);
	va_end(args);
}

void sendMsg(std::string sender_name, int fd, ...) {
    va_list args;
    va_start(args, fd);

	std::stringstream ss;
	ss << ":" << sender_name;
	ss << " " << "PRIVMSG ";
	while (1) {
		const char *arg = va_arg(args, const char *);
		if (arg == NULL) {
			break;
		}
		ss << " " << arg;
	}
	ss << "\r\n";
	std::string response = ss.str();
	send(fd, response.c_str(), response.size(), 0);
	va_end(args);
}

std::string current_timestamp() {
    std::time_t now = std::time(0);
    std::tm now_tm;
    localtime_r(&now, &now_tm);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &now_tm);

    std::ostringstream oss;
    oss << "[" << buffer << "]";
    return oss.str();
}

void sendServerMsg(const char* format, ...) {
    va_list args;
    va_start(args, format);

    std::ostringstream oss;

    oss << "\033[2m" << current_timestamp() << "\033[0m ";

    while (*format != '\0') {
        if (*format == '%' && *(format + 1) != '\0') {
            format++;
            switch (*format) {
                case 'd': {
                    int i = va_arg(args, int);
                    oss << "\033[32m" << i << "\033[0m";
                    break;
                }
                case 's': {
                    const char* s = va_arg(args, const char*);
                    oss << "\033[32m" << s << "\033[0m";
                    break;
                }
                case '%': {
                    oss << '%';
                    break;
                }
                default: {
                    oss << '%' << *format;
                    break;
                }
            }
        } else {
            oss << *format;
        }
        format++;
    }

    va_end(args);
    oss << std::endl;
    std::cout << oss.str();
}

// void sendServerMsg(const char* format, ...) {
//     va_list args;
//     va_start(args, format);

//     std::ostringstream oss;
//     oss << current_timestamp() << " ";

//     while (*format != '\0') {
//         if (*format == '%' && *(format + 1) != '\0') {
//             format++;
//             switch (*format) {
//                 case 'd': {
//                     int i = va_arg(args, int);
//                     oss << i;
//                     break;
//                 }
//                 case 's': {
//                     const char* s = va_arg(args, const char*);
//                     oss << s;
//                     break;
//                 }
//                 default: {
//                     oss << '%' << *format;
//                     break;
//                 }
//             }
//         } else {
//             oss << *format;
//         }
//         format++;
//     }

//     va_end(args);
//     oss << "\n";
//     std::cout << oss.str();
// }
