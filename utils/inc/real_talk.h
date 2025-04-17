#ifndef REAL_TALK_H
#define REAL_TALK_H

#include <sys/socket.h>

#include "./real_talk_type.h"
#include "./payload.h"

// RealTalk

typedef struct s_RealTalk {
    RealTalkType type;
    Payload *payload;
} RealTalk;

RealTalk *RealTalk_new(RealTalkType type, size_t length, const char *data);
RealTalk *RealTalk_new_own(RealTalkType type, size_t length, char *data);
RealTalk *RealTalk_receive(int cd);
void RealTalk_send(int cd, RealTalk *realtalk);
void RealTalk_free(RealTalk *realtalk);
void RealTalk_destroy(RealTalk **realtalk);

#endif

