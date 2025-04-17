#include "../inc/real_talk_ext.h"

RealTalk *RealTalk_user_set_username(Payload *payload, DataBase *database) {
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
    cJSON *username = cJSON_GetObjectItemCaseSensitive(req, "username");
    if (!cJSON_IsNumber(id) || !cJSON_IsString(username)) {
        goto out_invalid_json;
    }

    cJSONUtils_MergePatch(res, DataBase_user_set_username(database, id->valueint, username->valuestring));

out_respond_cleanup:;
    char *data = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);
    cJSON_Delete(req);
    return data == NULL ? NULL : RealTalk_new_own(USER_SET_USERNAME, strlen(data), data);

out_invalid_json:
    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_JSON_INVALID));
    goto out_respond_cleanup;
}

