#include "../inc/database.h"

cJSON *DataBase_user_message_edit(DataBase *database, size_t message_id, const char *text) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "UPDATE messages SET message_text = ? WHERE message_id = ? RETURNING *",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 1, text, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 2, message_id) != SQLITE_OK) {
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

    cJSON_AddNumberToObject(dbr, "message_id", sqlite3_column_int64(stmt, 0));
    cJSON_AddStringToObject(dbr, "message_text", (char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(dbr, "sent_date", (char *) sqlite3_column_text(stmt, 2));
    cJSON_AddNumberToObject(dbr, "from_user_id", sqlite3_column_int64(stmt, 3));
    cJSON_AddNumberToObject(dbr, "to_chat_id", sqlite3_column_int64(stmt, 4));
    sqlite3_finalize(stmt);

    cJSON *recepients = cJSON_AddArrayToObject(dbr, "recepients");

    stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT user_id FROM chat_members WHERE chat_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, cJSON_GetObjectItemCaseSensitive(dbr, "to_chat_id")->valueint) != SQLITE_OK) {
        goto out_cleanup;
    }

another_row:
    switch (sqlite3_step(stmt)) {
        case SQLITE_DONE: break;
        case SQLITE_ROW:
            cJSON_AddItemToArray(recepients, cJSON_CreateNumber(sqlite3_column_int64(stmt, 0)));
            goto another_row;
        default:
            goto out_cleanup;
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

