#include "../inc/real_talk.h"

void RealTalk_send(int cd, RealTalk *realtalk) {
    if (realtalk == NULL) {
        return;
    }

    if (send(cd, &realtalk->type, sizeof(realtalk->type), 0) == -1
        || send(cd, &realtalk->payload->length, sizeof(realtalk->payload->length), 0) == -1
        || send(cd, realtalk->payload->data, realtalk->payload->length, 0) == -1) {
        return;
    }
}

