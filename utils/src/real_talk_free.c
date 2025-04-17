#include "../inc/real_talk.h"

void RealTalk_free(RealTalk *realtalk) {
    if (realtalk == NULL) {
        return;
    }

    Payload_destroy(&realtalk->payload);
}

