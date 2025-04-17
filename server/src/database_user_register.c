#include "../inc/database.h"

cJSON *DataBase_user_register(DataBase *database, const char *username, const char *password) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "INSERT INTO users (username, password) VALUES (?, ?) RETURNING user_id",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC) != SQLITE_OK) {
        goto out_cleanup;
    }

    switch (sqlite3_step(stmt)) {
        case SQLITE_ROW: break;
        case SQLITE_CONSTRAINT:
            cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_USER_EXISTS));
            goto out_cleanup;
        default:
            goto out_cleanup;
    }

    char *dap;
    asprintf(&dap, "%s/users/default/avatar", database->vaultpath);
    char *udp;
    asprintf(&udp, "%s/users/%lld", database->vaultpath, sqlite3_column_int64(stmt, 0));
    char *uap;
    asprintf(&uap, "%s/avatar", udp);

    mkdir(udp, S_IRWXU | S_IRWXG | S_IRWXO);
    symlink(dap, uap);

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));

    free(uap);
    free(udp);
    free(dap);
out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

