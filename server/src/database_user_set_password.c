#include "../inc/database.h"

cJSON *DataBase_user_set_password(DataBase *database, size_t id, const char *password) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "UPDATE users SET password = ? WHERE user_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 1, password, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 2, id) != SQLITE_OK) {
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_DONE: break;
        default:
            goto out_cleanup;
    }

    if (!sqlite3_changes(database->db)) {
        cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_USER_INVALID));
        goto out_cleanup;
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

