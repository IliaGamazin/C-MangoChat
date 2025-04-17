#include "../inc/database.h"

cJSON *DataBase_user_set_username(DataBase *database, size_t id, const char *username) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "UPDATE users SET username = ? WHERE user_id = ?",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_int64(stmt, 2, id) != SQLITE_OK) {
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_DONE: break;
        case SQLITE_CONSTRAINT:
            cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_USER_EXISTS));
            goto out_cleanup;
        default:
            goto out_cleanup;
    }

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));

out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

