#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./deploy [name]\n");
        return -1;
    }

    const char *filename = argv[1];

    // Open
    sqlite3 *database = NULL;
    int status = sqlite3_open(filename, &database);
    if (status != SQLITE_OK) {
        fprintf(stderr, "sqlite3_open: %s\n", sqlite3_errmsg(database));
        goto out_cleanup;
    }

    // SQL
    const char *sql = "CREATE TABLE IF NOT EXISTS users ("
                          "user_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                          "username TEXT NOT NULL UNIQUE,"
                          "password TEXT NOT NULL"
                      ");"

                      "CREATE TABLE IF NOT EXISTS chats ("
                          "chat_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                          "chat_name TEXT"
                      ");"

                      "CREATE TABLE IF NOT EXISTS chat_members ("
                          "chat_member_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                          "chat_id INTEGER NOT NULL,"
                          "user_id INTEGER NOT NULL,"
                          "FOREIGN KEY(chat_id) REFERENCES chats(chat_id) ON DELETE SET NULL,"
                          "FOREIGN KEY(user_id) REFERENCES users(user_id) ON DELETE SET NULL"
                      ");"

                      "CREATE TABLE IF NOT EXISTS messages ("
                          "message_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                          "message_text TEXT NOT NULL,"
                          "sent_date INTEGER NOT NULL,"
                          "from_user_id INTEGER NOT NULL,"
                          "to_chat_id INTEGER,"
                          "FOREIGN KEY(from_user_id) REFERENCES users(user_id) ON DELETE SET NULL,"
                          "FOREIGN KEY(to_chat_id) REFERENCES chats(chat_id) ON DELETE CASCADE"
                      ");"

                      "CREATE TABLE IF NOT EXISTS chat_last_messages ("
                          "chat_id INTEGER PRIMARY KEY,"
                          "last_message_id INTEGER,"
                          "FOREIGN KEY(chat_id) REFERENCES chats(chat_id) ON DELETE CASCADE,"
                          "FOREIGN KEY(last_message_id) REFERENCES messages(message_id) ON DELETE SET NULL"
                      ");";

    // Initialize
    char *errmsg = NULL;
    status = sqlite3_exec(database, sql, NULL, NULL, &errmsg);
    if (status != SQLITE_OK) {
        fprintf(stderr, "sqlite3_exec: %s\n", errmsg);
        sqlite3_free(errmsg);
    }

out_cleanup:
    sqlite3_close(database);
    return status;
}

