#ifndef REAL_TALK_EXT_H
#define REAL_TALK_EXT_H

#include "../../utils/inc/real_talk.h"
#include "../../utils/inc/request_status.h"

#include "./database.h"
#include "./client.h"

#include <cjson/cJSON_Utils.h>
#include <cjson/cJSON.h>

// RealTalkExt

void RealTalk_respond(RealTalk *request, Client *clients, Client *client, DataBase *database);
RealTalk *RealTalk_user_register(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_login(Payload *payload, DataBase *database, Client *client);
RealTalk *RealTalk_user_set_avatar(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_get_avatar(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_set_username(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_set_password(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_chat_create(Payload *payload, DataBase *database, Client *clients, Client *client);
RealTalk *RealTalk_user_chat_set_avatar(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_chat_get(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_chats_get(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_message_create(Payload *payload, DataBase *database, Client *clients, Client *client);
RealTalk *RealTalk_user_message_get(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_messages_get(Payload *payload, DataBase *database);
RealTalk *RealTalk_user_message_edit(Payload *payload, DataBase *database, Client *clients, Client *client);

#endif

