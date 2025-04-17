#include "../inc/database.h"

void DataBase_free(DataBase *database) {
    if (database == NULL) {
        return;
    }

    sqlite3_close(database->db);
    database->db = NULL;
}

