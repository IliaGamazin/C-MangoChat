#include "../inc/database.h"

cJSON *DataBase_user_chat_members_add(DataBase *database, size_t chat_id, cJSON *chat_members) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);
    cJSON *members = cJSON_AddArrayToObject(dbr, "members");

    cJSON *member = NULL;
    cJSON_ArrayForEach(member, chat_members) {
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(database->db, "INSERT INTO chat_members (chat_id, user_id) SELECT ?, (SELECT user_id FROM users WHERE username = ?) RETURNING user_id",  -1, &stmt, NULL) != SQLITE_OK ||
            sqlite3_bind_int64(stmt, 1, chat_id) != SQLITE_OK ||
            sqlite3_bind_text(stmt, 2, member->valuestring, -1, SQLITE_STATIC) != SQLITE_OK) {
            continue;
        }

        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW: break;
            default:
                continue;
        }

        size_t user_id = sqlite3_column_int64(stmt, 0);

        sqlite3_finalize(stmt);
        stmt = NULL;
        if (sqlite3_prepare_v2(database->db, "SELECT username FROM users WHERE user_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
            sqlite3_bind_int64(stmt, 1, user_id) != SQLITE_OK) {
            continue;
        }

        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW: break;
            default:
                continue;
        }

        cJSON *user = cJSON_CreateObject();
        cJSON_AddNumberToObject(user, "user_id", user_id);
        cJSON_AddStringToObject(user, "username", (char *) sqlite3_column_text(stmt, 0));
        cJSON_AddItemToArray(members, user);
        sqlite3_finalize(stmt);
    }

    cJSON_AddNumberToObject(dbr, "status", RQ_SUCCESS);

    return dbr;
}

