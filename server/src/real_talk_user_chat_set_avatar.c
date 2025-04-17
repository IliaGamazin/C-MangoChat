#include "../inc/real_talk_ext.h"

RealTalk *RealTalk_user_chat_set_avatar(Payload *payload, DataBase *database) {
    cJSON *req = cJSON_ParseWithLength(payload->data, payload->length);
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", RQ_UNKNOWN);
    if (req == NULL || res == NULL) {
        goto out_respond_cleanup;
    }

    if (payload == NULL) {
        goto out_json_invalid;
    }

    cJSON *chat_id = cJSON_GetObjectItemCaseSensitive(req, "chat_id");
    cJSON *avatar = cJSON_GetObjectItemCaseSensitive(req, "avatar");
    if (!cJSON_IsNumber(chat_id) || !cJSON_IsString(avatar)) {
        goto out_json_invalid;
    }

    char *avatarpath = NULL;
    if (asprintf(&avatarpath, "%s/chats/%d/avatar", database->vaultpath, chat_id->valueint) == -1) {
        goto out_avatarpath_cleanup;
    }

    if (unlink(avatarpath) == -1) {
        goto out_chat_invalid;
    }

    Payload *avatar_payload = Payload_new(strlen(avatar->valuestring), avatar->valuestring);
    if (avatar_payload == NULL ||
        Payload_base64_decode(avatar_payload) ||
        Payload_save_to_file(avatar_payload, avatarpath)) {
        goto out_avatar_payload_cleanup;
    }

    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_SUCCESS));

out_avatar_payload_cleanup:
    Payload_destroy(&avatar_payload);
out_avatarpath_cleanup:
    free(avatarpath);
out_respond_cleanup:;
    char *data = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);
    cJSON_Delete(req);
    return data == NULL ? NULL : RealTalk_new_own(USER_SET_AVATAR, strlen(data), data);

out_json_invalid:
    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_JSON_INVALID));
    goto out_respond_cleanup;

out_chat_invalid:
    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_CHAT_INVALID));
    goto out_respond_cleanup;
}

