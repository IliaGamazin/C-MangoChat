#include "../inc/real_talk_ext.h"

#include <syslog.h>

RealTalk *RealTalk_user_chat_get(Payload *payload, DataBase *database) {
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

    cJSONUtils_MergePatch(res, DataBase_user_chat_get(database, id->valueint));
    if (cJSON_GetObjectItemCaseSensitive(res, "status")->valueint != RQ_SUCCESS) {
        syslog(LOG_WARNING, "GOTCHA, BITCH");
        goto out_respond_cleanup;
    }

    char *avatarpath = NULL;
    if (asprintf(&avatarpath, "%s/chats/%d/avatar", database->vaultpath, id->valueint) == -1) {
        goto out_avatarpath_cleanup;
    }

    Payload *avatar_payload = Payload_new_from_file(avatarpath);
    if (avatar_payload == NULL || Payload_base64_encode(avatar_payload)) {
        goto out_avatar_payload_cleanup;
    }

    cJSON_AddStringToObject(res, "avatar", avatar_payload->data);

out_avatar_payload_cleanup:
    Payload_destroy(&avatar_payload);
out_avatarpath_cleanup:
    free(avatarpath);
out_respond_cleanup:;
    char *data = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);
    cJSON_Delete(req);
    return data == NULL ? NULL : RealTalk_new_own(USER_CHAT_GET, strlen(data), data);

out_invalid_json:
    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_JSON_INVALID));
    goto out_respond_cleanup;
}

