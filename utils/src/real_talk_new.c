#include "../inc/real_talk.h"

RealTalk *RealTalk_new(RealTalkType type, size_t length, const char *data) {
    RealTalk *realtalk = malloc(sizeof(RealTalk));
    if (realtalk == NULL) {
        return NULL;
    }

    realtalk->type = type;
    realtalk->payload = Payload_new(length, data);
    if (realtalk->payload == NULL) {
        RealTalk_destroy(&realtalk);
        return NULL;
    }

    return realtalk;
}

