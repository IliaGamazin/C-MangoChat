#ifndef SETTINGS_H
#define SETTINGS_H

#include "../inc/libs.h"
#include "../inc/login.h"
#include "../inc/request.h"

void on_change_avatar_button_clicked(GtkButton *button, gpointer user_data);
void on_logout_button_clicked(GtkButton *button, gpointer user_data);
void on_avatar_settings_clicked(GtkButton *button, gpointer user_data);
void on_change_username_button_clicked(GtkButton *button, gpointer user_data);
void on_change_password_button_clicked(GtkButton *button, gpointer user_data);
void on_save_username_clicked(GtkButton *button, gpointer user_data);
void on_save_password_clicked(GtkButton *button, gpointer user_data);
gboolean on_avatar_received(gpointer user_data);

#endif
