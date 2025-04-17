#include "../inc/real_talk_ext.h"

RealTalk *RealTalk_user_message_edit(Payload *payload, DataBase *database, Client *clients, Client *client) {
    cJSON *req = cJSON_ParseWithLength(payload->data, payload->length);
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", RQ_UNKNOWN);
    if (req == NULL || res == NULL) {
        goto out_respond_cleanup;
    }

    if (payload == NULL) {
        goto out_invalid_json;
    }

    cJSON *message_id = cJSON_GetObjectItemCaseSensitive(req, "message_id");
    cJSON *text = cJSON_GetObjectItemCaseSensitive(req, "text");
    if (!cJSON_IsNumber(message_id) ||
        !cJSON_IsString(text)) {
        goto out_invalid_json;
    }

    cJSONUtils_MergePatch(res, DataBase_user_message_edit(database, message_id->valueint, text->valuestring));
    cJSON *recepients = cJSON_DetachItemFromObjectCaseSensitive(res, "recepients");

    char *tdata = cJSON_PrintUnformatted(res);
    RealTalk *temp = RealTalk_new_own(USER_MESSAGE_GET, strlen(tdata), tdata);

    cJSON *recepient = NULL;
    cJSON_ArrayForEach(recepient, recepients) {
        int user_id = recepient->valueint;
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
    return data == NULL ? NULL : RealTalk_new_own(USER_MESSAGE_GET, strlen(data), data);

out_invalid_json:
    cJSON_ReplaceItemInObject(res, "status", cJSON_CreateNumber(RQ_JSON_INVALID));
    goto out_respond_cleanup;
}

