#include "../inc/real_talk_ext.h"

RealTalk *RealTalk_user_chat_create(Payload *payload, DataBase *database, Client *clients, Client *client) {
    (void) clients;
    cJSON *req = cJSON_ParseWithLength(payload->data, payload->length);
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", RQ_UNKNOWN);
    if (req == NULL || res == NULL) {
        goto out_respond_cleanup;
    }

    if (payload == NULL) {
        goto out_invalid_json;
    }

    cJSON *chat_name = cJSON_GetObjectItemCaseSensitive(req, "chat_name");
    cJSON *avatar = cJSON_GetObjectItemCaseSensitive(req, "chat_avatar");
    cJSON *chat_members = cJSON_GetObjectItemCaseSensitive(req, "members");
    if (!(cJSON_IsString(chat_name) || cJSON_IsNull(chat_name)) ||
        !(cJSON_IsString(avatar) || cJSON_IsNull(avatar)) ||
        !cJSON_IsArray(chat_members)) {
        goto out_invalid_json;
    }

    cJSONUtils_MergePatch(res, DataBase_user_chat_create(database, chat_name->valuestring));
    if (cJSON_GetObjectItemCaseSensitive(res, "status")->valueint != RQ_SUCCESS) {
        goto out_respond_cleanup;
    }

    size_t chat_id = cJSON_GetObjectItemCaseSensitive(res, "id")->valueint;

    cJSON *avatar_req_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(avatar_req_json, "chat_id", chat_id);
    cJSON_AddStringToObject(avatar_req_json, "avatar", avatar->valuestring);
    char *avatar_req_data = cJSON_PrintUnformatted(avatar_req_json);
    cJSON_Delete(avatar_req_json);

    if (!cJSON_IsNull(avatar)) {
        cJSON_AddStringToObject(res, "avatar", avatar->valuestring);
        RealTalk_user_chat_set_avatar(Payload_new_own(strlen(avatar_req_data), avatar_req_data), database);
    }
    cJSONUtils_MergePatch(res, DataBase_user_chat_members_add(database, chat_id, chat_members));
    cJSON *member = NULL;
    cJSON *members = cJSON_GetObjectItemCaseSensitive(res, "members");
    cJSON_ArrayForEach(member, members) {
        size_t user_id = cJSON_GetObjectItemCaseSensitive(member, "user_id")->valueint;
        char *avatarpath = NULL;
        asprintf(&avatarpath, "%s/users/%ld/avatar", database->vaultpath, user_id);

        Payload *avatar_payload = Payload_new_from_file(avatarpath);
        Payload_base64_encode(avatar_payload);

        cJSON_AddStringToObject(member, "avatar", avatar_payload->data);
        Payload_destroy(&avatar_payload);
        free(avatarpath);
    }

    char *tdata = cJSON_PrintUnformatted(res);
    RealTalk *temp = RealTalk_new_own(USER_CHAT_CREATE, strlen(tdata), tdata);

    member = NULL;
    members = cJSON_GetObjectItemCaseSensitive(res, "members");
    cJSON_ArrayForEach(member, members) {
        int user_id = cJSON_GetObjectItemCaseSensitive(member, "user_id")->valueint;
        if (user_id == client->id) {
            continue;
        }

        Client *cuser = Client_find_by(clients, Client_by_id, &user_id);
        if (cuser != NULL) {
            RealTalk_send(cuser->cd, temp);
        }
    }

    RealTalk_destroy(&temp);

out_respond_cleanup:;
    char *data = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);
    cJSON_Delete(req);
    return data == NULL ? NULL : RealTalk_new_own(USER_CHAT_CREATE, strlen(data), data);

out_invalid_json:
    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_JSON_INVALID));
    goto out_respond_cleanup;
}

