#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "../inc/libs.h"
#include "../inc/main_data.h"

bool password_validator(const char *password);
bool username_validator(const char *username);
bool strings_match(const char *str1, const char *str2);
void set_valid_entry_color(GtkWidget *entry, gboolean is_valid);
GList *retrieve_chat_members(GtkWidget *member_list_box);
bool has_dup_member(GList *members, const gchar *member_name);

#endif
