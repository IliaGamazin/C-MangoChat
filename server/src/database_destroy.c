#include "../inc/database.h"

void DataBase_destroy(DataBase **database) {
    if (database == NULL) {
        return;
    }

    DataBase_free(*database);
    free(*database);
    *database = NULL;
}

