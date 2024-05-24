#include "../inc/ResponseHandler.hpp"

std::string createErrorMessage(const ErrorMessage& errorMsg) {
    // Input Validation
    if (errorMsg.num.empty() || errorMsg.msg.empty() || errorMsg.error.empty()) {
        return "ERR_INVALID_ERROR_PARAMETERS"; // Handle invalid input
    }

    std::ostringstream oss;

    // Constructing the Error Message (IRC Format)
    oss << ":" << "SERVER_PREFIX" << " "
        << errorMsg.num << " "
        << errorMsg.msg << " "
        << errorMsg.error;

    return oss.str();
}
