#include "../inc/request_status.h"

const char *RequestStatus_str(RequestStatus status) {
    switch (status) {
        case RQ_SUCCESS:
            return "Success, yipeeee!\n";
        case RQ_JSON_INVALID:
            return "Invalid JSON\n";
        case RQ_USER_EXISTS:
            return "User already exists\n";
        case RQ_USER_INVALID:
            return "User does not exist\n";
        default:
            return "Unknown error\n";
    }
}

