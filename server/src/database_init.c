#include "../inc/database.h"

int DataBase_init(DataBase *database, const char *filename, char *vaultpath) {
    if (database == NULL || filename == NULL) {
        return -1;
    }

    if (sqlite3_open_v2(filename, &database->db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL) != SQLITE_OK) {
        DataBase_free(database);
        return -1;
    }

    database->vaultpath = vaultpath;

    return 0;
}

