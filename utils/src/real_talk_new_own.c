#include "../inc/real_talk.h"

RealTalk *RealTalk_new_own(RealTalkType type, size_t length, char *data) {
    RealTalk *realtalk = malloc(sizeof(RealTalk));
    if (realtalk == NULL) {
        return NULL;
    }

    realtalk->type = type;
    realtalk->payload = Payload_new_own(length, data);
    if (realtalk->payload == NULL) {
        RealTalk_destroy(&realtalk);
        return NULL;
    }

    return realtalk;
}

