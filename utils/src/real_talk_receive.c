#include "../inc/real_talk.h"

RealTalk *RealTalk_receive(int cd) {
    RealTalkType type;
    size_t length;

    // Read type
    int bytes = recv(cd, &type, sizeof(type), MSG_WAITALL);
    if (!bytes || bytes == -1) {
        return NULL;
    }

    // Read length
    bytes = recv(cd, &length, sizeof(length), MSG_WAITALL);
    if (!bytes || bytes == -1) {
        return NULL;
    }

    // Construct realtalk
    RealTalk *realtalk = RealTalk_new(type, length, NULL);
    if (realtalk == NULL) {
        return NULL;
    }

    if (!length) {
        return realtalk;
    }

    // Read data
    bytes = recv(cd, realtalk->payload->data, realtalk->payload->length, MSG_WAITALL);
    if (!bytes || bytes == -1) {
        RealTalk_destroy(&realtalk);
        return NULL;
    }

    return realtalk;
}

