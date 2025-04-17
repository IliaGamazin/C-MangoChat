#include "../inc/database.h"

cJSON *DataBase_user_message_get(DataBase *database, size_t user_id, size_t message_id) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT * FROM messages WHERE message_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, message_id) != SQLITE_OK) {
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_ROW: break;
        case SQLITE_DONE:
            cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_MESSAGE_INVALID));
            goto out_cleanup;
        default:
            goto out_cleanup;
    }

    cJSON *message = cJSON_CreateObject();
    cJSON_AddNumberToObject(message, "message_id", sqlite3_column_int64(stmt, 0));
    cJSON_AddStringToObject(message, "message_text", (char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(message, "sent_date", (char *) sqlite3_column_text(stmt, 2));
    cJSON_AddNumberToObject(message, "from_user_id", sqlite3_column_int64(stmt, 3));
    cJSON_AddNumberToObject(message, "to_chat_id", sqlite3_column_int64(stmt, 4));
    sqlite3_finalize(stmt);

    stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT EXISTS (SELECT * FROM chat_members WHERE chat_id = ? AND user_id = ?)",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, cJSON_GetObjectItemCaseSensitive(message, "to_chat_id")->valueint) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 2, user_id) != SQLITE_OK) {
        cJSON_Delete(message);
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_ROW: break;
        default:
            cJSON_Delete(message);
            goto out_cleanup;
    }

    if (!sqlite3_column_int(stmt, 0)) {
        cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_USER_INVALID));
        cJSON_Delete(message);
        goto out_cleanup;
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));
    cJSONUtils_MergePatch(dbr, message);

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

