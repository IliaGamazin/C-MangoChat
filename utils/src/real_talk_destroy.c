#include "../inc/real_talk.h"

void RealTalk_destroy(RealTalk **realtalk) {
    if (realtalk == NULL) {
        return;
    }

    RealTalk_free(*realtalk);
    free(*realtalk);
    *realtalk = NULL;
}

