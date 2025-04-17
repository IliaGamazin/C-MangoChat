#include "../inc/database.h"

#include <syslog.h>

cJSON *DataBase_user_chats_get(DataBase *database, size_t id) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT * FROM chats WHERE chat_id IN (SELECT chat_id FROM chat_members WHERE user_id = ?)",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 1, id) != SQLITE_OK) {
        goto out_cleanup;
    }

    cJSON *chats = cJSON_AddArrayToObject(dbr, "chats");

another_row:
    switch (sqlite3_step(stmt)) {
        case SQLITE_DONE: break;
        case SQLITE_ROW:;
            cJSON *chat = cJSON_CreateObject();
            cJSON_AddNumberToObject(chat, "chat_id", sqlite3_column_int64(stmt, 0));
            cJSON_AddStringToObject(chat, "chat_name", (char *) sqlite3_column_text(stmt, 1));
            cJSON_AddItemToArray(chats, chat);
            goto another_row;
        default:
            goto out_cleanup;
    }

    sqlite3_finalize(stmt);

    cJSON *chat = NULL;
    cJSON_ArrayForEach(chat, chats) {
        stmt = NULL;
        if (sqlite3_prepare_v2(database->db, "SELECT user_id, username FROM users WHERE user_id IN (SELECT user_id FROM chat_members WHERE chat_id = ?)",  -1, &stmt, NULL) != SQLITE_OK ||
            sqlite3_bind_int64(stmt, 1, cJSON_GetObjectItemCaseSensitive(chat, "chat_id")->valueint) != SQLITE_OK) {
            goto out_cleanup;
        }

        cJSON *members = cJSON_AddArrayToObject(chat, "members");
another_row1:
        switch (sqlite3_step(stmt)) {
            case SQLITE_DONE: break;
            case SQLITE_ROW:;
                cJSON *member = cJSON_CreateObject();
                cJSON_AddNumberToObject(member, "user_id", sqlite3_column_int64(stmt, 0));
                cJSON_AddStringToObject(member, "username", (char *) sqlite3_column_text(stmt, 1));
                cJSON_AddItemToArray(members, member);
                goto another_row1;
            default:
                goto out_cleanup;
        }
        sqlite3_finalize(stmt);

        stmt = NULL;
        if (sqlite3_prepare_v2(database->db, "SELECT last_message_id FROM chat_last_messages WHERE chat_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
            sqlite3_bind_int64(stmt, 1, cJSON_GetObjectItemCaseSensitive(chat, "chat_id")->valueint) != SQLITE_OK) {
            goto out_cleanup;
        }

        switch (sqlite3_step(stmt)) {
            case SQLITE_DONE:
                cJSON_AddNumberToObject(chat, "last_message_id", -1);
                break;
            case SQLITE_ROW:
                cJSON_AddNumberToObject(chat, "last_message_id", sqlite3_column_int64(stmt, 0));
                break;
            default:
                goto out_cleanup;
        }

        sqlite3_finalize(stmt);
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));

    return dbr;

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

