#include "../inc/database.h"

cJSON *DataBase_user_chat_create(DataBase *database, const char *chat_name) {
    cJSON *dbr = cJSON_CreateObject();
    cJSON_AddNumberToObject(dbr, "status", RQ_UNKNOWN);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(database->db, "INSERT INTO chats (chat_name) VALUES (?) RETURNING chat_id, chat_name",  -1, &stmt, NULL) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 1, chat_name, -1, SQLITE_STATIC) != SQLITE_OK) {
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

    size_t chat_id = sqlite3_column_int64(stmt, 0);

    char *dap;
    asprintf(&dap, "%s/chats/default/avatar", database->vaultpath);
    char *cdp;
    asprintf(&cdp, "%s/chats/%zu", database->vaultpath, chat_id);
    char *cap;
    asprintf(&cap, "%s/avatar", cdp);

    mkdir(cdp, S_IRWXU | S_IRWXG | S_IRWXO);
    symlink(dap, cap);

    cJSON_ReplaceItemInObject(dbr, "status", cJSON_CreateNumber(RQ_SUCCESS));
    cJSON_AddNumberToObject(dbr, "id", chat_id);
    cJSON_AddStringToObject(dbr, "chat_name", (char *) sqlite3_column_text(stmt, 1));

    free(cap);
    free(cdp);
    free(dap);
out_cleanup:
    sqlite3_finalize(stmt);
    return dbr;
}

