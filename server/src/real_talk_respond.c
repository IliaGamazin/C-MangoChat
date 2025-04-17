#include "../inc/real_talk_ext.h"

void RealTalk_respond(RealTalk *request, Client *clients, Client *client, DataBase *database) {
    if (request == NULL || database == NULL) {
        return;
    }

    RealTalk *response = NULL;
    switch (request->type) {
        case USER_REGISTER:
            response = RealTalk_user_register(request->payload, database);
            break;
        case USER_LOGIN:
            response = RealTalk_user_login(request->payload, database, client);
            break;
        case USER_SET_AVATAR:
            response = RealTalk_user_set_avatar(request->payload, database);
            break;
        case USER_GET_AVATAR:
            response = RealTalk_user_get_avatar(request->payload, database);
            break;
        case USER_SET_USERNAME:
            response = RealTalk_user_set_username(request->payload, database);
            break;
        case USER_SET_PASSWORD:
            response = RealTalk_user_set_password(request->payload, database);
            break;
        case USER_CHAT_CREATE:
            response = RealTalk_user_chat_create(request->payload, database, clients, client);
            break;
        case USER_CHAT_GET:
            response = RealTalk_user_chat_get(request->payload, database);
            break;
        case USER_CHATS_GET:
            response = RealTalk_user_chats_get(request->payload, database);
            break;
        case USER_MESSAGE_CREATE:
            response = RealTalk_user_message_create(request->payload, database, clients, client);
            break;
        case USER_MESSAGE_GET:
            response = RealTalk_user_message_get(request->payload, database);
            break;
        case USER_MESSAGES_GET:
            response = RealTalk_user_messages_get(request->payload, database);
            break;
        case USER_MESSAGE_EDIT:
            response = RealTalk_user_message_edit(request->payload, database, clients, client);
            break;
        default:
            break;
    }

    RealTalk_send(client->cd, response);
    RealTalk_destroy(&response);
}

