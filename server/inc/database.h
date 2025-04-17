#ifndef DATABASE_H
#define DATABASE_H

#include "../../utils/inc/request_status.h"

#include <sqlite3.h>
#include <cjson/cJSON_Utils.h>
#include <cjson/cJSON.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>

typedef struct s_DataBase {
    sqlite3 *db;
    char *vaultpath;
} DataBase;

DataBase *DataBase_new(void);
int DataBase_init(DataBase *database, const char *filename, char *vaultpath);
void DataBase_free(DataBase *database);
void DataBase_destroy(DataBase **database);
cJSON *DataBase_user_register(DataBase *database, const char *username, const char *password);
cJSON *DataBase_user_login(DataBase *database, const char *username, const char *password);
cJSON *DataBase_user_set_avatar(DataBase *database, size_t id, const char *avatar);
cJSON *DataBase_user_set_username(DataBase *database, size_t id, const char *username);
cJSON *DataBase_user_set_password(DataBase *database, size_t id, const char *password);
cJSON *DataBase_user_chat_create(DataBase *database, const char *chat_name);
cJSON *DataBase_user_chat_members_add(DataBase *database, size_t chat_id, cJSON *chat_members);
cJSON *DataBase_user_chat_get(DataBase *database, size_t id);
cJSON *DataBase_user_chats_get(DataBase *database, size_t id);
cJSON *DataBase_user_message_create(DataBase *database, size_t to_chat_id, size_t from_user_id, const char *message_text);
cJSON *DataBase_user_message_get(DataBase *database, size_t user_id, size_t message_id);
cJSON *DataBase_user_messages_get(DataBase *database, size_t user_id);
cJSON *DataBase_user_message_edit(DataBase *database, size_t message_id, const char *text);

#endif

