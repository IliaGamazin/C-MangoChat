#include "../inc/database.h"

DataBase *DataBase_new(void) {
    DataBase *database = malloc(sizeof(DataBase));
    if (database == NULL) {
        return NULL;
    }

    database->db = NULL;
    database->vaultpath = NULL;
    
    return database;
}

