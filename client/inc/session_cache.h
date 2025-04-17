#ifndef SESSION_CACHE
#define SESSION_CACHE

#include "../inc/libs.h"
#include "../inc/main_data.h"
#include "../inc/request.h"
#include "../inc/avatar_handler.h"
typedef struct {
    int user_id;
    char *name;
    GdkPixbuf *avatar;
} User;

typedef struct {
    int message_id;
    int sender_id;
    int chat_id;
    char *text;
    int file_id;
    time_t sent_time;
    GtkWidget *message_box;
} Message;

typedef struct {
    int chat_id;
    int last_message_id;
    int members_num;
    int *user_ids;
    char *name;
    GdkPixbuf *avatar;
    GtkWidget *chat_box;
    GtkWidget *chat_message_container;
} Chat;

User *user_encache(int id, char *name, const char *encoded_avatar);
void user_free(User *user);
void user_destroy(void **user);
void user_add(int id, char *name, const char *encoded_avatar);

Message *message_encache(int id, int chat_id, int sender_id, char *text, time_t sent_time);
void message_free(Message *message);
void message_destroy(Message **message);
void draw_message(gpointer key, gpointer value, gpointer user_data);
gboolean update_single_message(gpointer user_data);

gboolean draw_single_message(gpointer user_data);
void destroy_single_message(int message_id);
void update_message_ui(gpointer key, gpointer value, gpointer user_data);
void message_add_and_draw(int id, int chat_id, int sender_id, char *text, time_t send_time);
void message_add(int id, int chat_id, int sender_id, char *text, time_t send_time);

Chat *chat_encache(int id, int last_message_id, int members_num, char *name, const char *encoded_avatar);
void chat_free(Chat *chat);
void chat_destroy(Chat **chat);
void draw_chat(gpointer key, gpointer value, gpointer user_data);
void update_chat_ui(gpointer key, gpointer value, gpointer user_data);
gboolean draw_single_chat(gpointer user_data);
void chat_add_and_draw(int id, int last_message_id, int members_num, char *name, const char *encoded_avatar);
void chat_add(int id, int last_message_id, int members_num, char *name, const char *encoded_avatar);
gboolean update_single_chat(gpointer user_data);

#endif
