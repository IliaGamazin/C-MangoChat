#include "../inc/database.h"

cJSON *DataBase_user_messages_get(DataBase *database, size_t user_id) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT * FROM messages WHERE to_chat_id IN (SELECT chat_id FROM chat_members WHERE user_id = ?) ORDER BY sent_date ASC",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, user_id) != SQLITE_OK) {
        goto out_cleanup;
    }

    cJSON *messages = cJSON_CreateArray();

another_row:
    switch (sqlite3_step(stmt)) {
        case SQLITE_DONE: break;
        case SQLITE_ROW:;
            cJSON *message = cJSON_CreateObject();
            cJSON_AddNumberToObject(message, "message_id", sqlite3_column_int64(stmt, 0));
            cJSON_AddStringToObject(message, "message_text", (char *) sqlite3_column_text(stmt, 1));
            cJSON_AddStringToObject(message, "sent_date", (char *) sqlite3_column_text(stmt, 2));
            cJSON_AddNumberToObject(message, "from_user_id", sqlite3_column_int64(stmt, 3));
            cJSON_AddNumberToObject(message, "to_chat_id", sqlite3_column_int64(stmt, 4));
            cJSON_AddItemToArray(messages, message);
            goto another_row;
        default:
            goto out_cleanup;
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));
    cJSON_AddItemToObject(dbr, "messages", messages);

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

