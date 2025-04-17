#ifndef CONTACTS_GROUPS_H
#define CONTACTS_GROUPS_H

#include "../inc/libs.h"
#include "../inc/request.h"
#include "../inc/avatar_handler.h"

void on_chats_button_clicked(GtkToggleButton *button, gpointer user_data);
void on_groups_button_clicked(GtkToggleButton *button, gpointer user_data);

void on_add_contact_button_clicked(GtkButton *button, gpointer user_data);
void on_add_new_contact_button_clicked(GtkButton *button, gpointer user_data);

void on_add_new_member_button_clicked(GtkButton *button, gpointer user_data);
void on_upload_group_avatar_clicked(GtkButton *button, gpointer user_data);
void on_group_settings_clicked(GtkButton *button, gpointer user_data);
void on_add_new_group_button_clicked(GtkButton *button, gpointer user_data);

#endif
