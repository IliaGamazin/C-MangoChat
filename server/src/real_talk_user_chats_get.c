#include "../inc/real_talk_ext.h"

RealTalk *RealTalk_user_chats_get(Payload *payload, DataBase *database) {
    cJSON *req = cJSON_ParseWithLength(payload->data, payload->length);
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", RQ_UNKNOWN);
    if (req == NULL || res == NULL) {
        goto out_respond_cleanup;
    }

    if (payload == NULL) {
        goto out_invalid_json;
    }

    cJSON *id = cJSON_GetObjectItemCaseSensitive(req, "id");
    if (!cJSON_IsNumber(id)) {
        goto out_invalid_json;
    }

    cJSONUtils_MergePatch(res, DataBase_user_chats_get(database, id->valueint));
    if (cJSON_GetObjectItemCaseSensitive(res, "status")->valueint != RQ_SUCCESS) {
        goto out_respond_cleanup;
    }

    cJSON *chats = cJSON_GetObjectItemCaseSensitive(res, "chats");
    cJSON *chat = NULL;
    cJSON_ArrayForEach(chat, chats) {
        char *ca = NULL;
        asprintf(&ca, "%s/chats/%d/avatar", database->vaultpath, cJSON_GetObjectItemCaseSensitive(chat, "chat_id")->valueint);
        Payload *cap = Payload_new_from_file(ca);
        Payload_base64_encode(cap);
        cJSON_AddStringToObject(chat, "avatar", cap->data);
        Payload_destroy(&cap);
        free(ca);

        cJSON *members = cJSON_GetObjectItemCaseSensitive(chat, "members");
        cJSON *member = NULL;
        cJSON_ArrayForEach(member, members) {
            ca = NULL;
            asprintf(&ca, "%s/users/%d/avatar", database->vaultpath, cJSON_GetObjectItemCaseSensitive(member, "user_id")->valueint);
            cap = Payload_new_from_file(ca);
            Payload_base64_encode(cap);
            cJSON_AddStringToObject(member, "avatar", cap->data);
            Payload_destroy(&cap);
            free(ca);
        }
    }

out_respond_cleanup:;
    char *data = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);
    cJSON_Delete(req);
    return data == NULL ? NULL : RealTalk_new_own(USER_CHATS_GET, strlen(data), data);

out_invalid_json:
    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_JSON_INVALID));
    goto out_respond_cleanup;
}

