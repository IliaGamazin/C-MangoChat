#ifndef REQUEST_STATUS_H
#define REQUEST_STATUS_H

// RequestStatus

typedef enum e_RequestStatus {
    RQ_SUCCESS,
    RQ_JSON_INVALID,
    RQ_USER_EXISTS,
    RQ_USER_INVALID,
    RQ_CHAT_INVALID,
    RQ_MESSAGE_INVALID,
    RQ_UNKNOWN,
} RequestStatus;

const char *RequestStatus_str(RequestStatus status);

#endif

