#ifndef REQUEST_H
#define REQUEST_H

#include "../inc/libs.h"
#include "../inc/main_data.h"
#include "../inc/validator.h"
#include "../inc/file_manager.h"
#include "../../utils/inc/real_talk.h"

cJSON *form_login_request(const char *username, const char *password);
cJSON *form_signup_request(const char *username, const char *password);
cJSON *form_change_avatar_request(const char *filename);
cJSON *form_change_username_request(const char *username);
cJSON *form_change_password_request(const char *password);
cJSON *form_create_chat_request (const char *chat_name, char *avatar_filename, GList *member_list);
cJSON *form_send_id_request(void);
cJSON *form_send_message_request(char *text, bool is_file);
cJSON *form_edit_message_request(int message_id, char *new_text);

void send_request(cJSON *json, int type);

#endif
