#include "../inc/database.h"

#include <syslog.h>

cJSON *DataBase_user_chat_get(DataBase *database, size_t id) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT chat_name FROM chats WHERE chat_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, id) != SQLITE_OK) {
        syslog(LOG_WARNING, "%s", sqlite3_errmsg(database->db));
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_ROW: break;
        case SQLITE_DONE:
            cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_CHAT_INVALID));
            goto out_cleanup;
        default:
            goto out_cleanup;
    }

    cJSON_AddStringToObject(dbr, "chat_name", (char *) sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT user_id FROM chat_members WHERE chat_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, id) != SQLITE_OK) {
        goto out_cleanup;
    }

    cJSON *members = cJSON_AddArrayToObject(dbr, "members");
another_row:
    switch (sqlite3_step(stmt)) {
        case SQLITE_DONE: break;
        case SQLITE_ROW:
            cJSON_AddItemToArray(members, cJSON_CreateNumber(sqlite3_column_int64(stmt, 0)));
            goto another_row;
        default:
            goto out_cleanup;
    }

    sqlite3_finalize(stmt);

    stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT last_message_id FROM chat_last_messages WHERE chat_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, id) != SQLITE_OK) {
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_ROW:
            cJSON_AddNumberToObject(dbr, "last_message_id", sqlite3_column_int64(stmt, 0));
            break;
        case SQLITE_DONE:
            cJSON_AddNumberToObject(dbr, "last_message_id", -1);
            break;
        default:
            goto out_cleanup;
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

