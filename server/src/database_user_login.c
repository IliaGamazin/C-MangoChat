#include "../inc/database.h"

cJSON *DataBase_user_login(DataBase *database, const char *username, const char *password) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "SELECT user_id FROM users WHERE username = ? AND password = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC) != SQLITE_OK) {
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_ROW: break;
        case SQLITE_DONE:
            cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_USER_INVALID));
            goto out_cleanup;
        default:
            goto out_cleanup;
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));
    cJSON_AddNumberToObject(dbr, "id", sqlite3_column_int(stmt, 0));

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

