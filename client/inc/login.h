#ifndef LOGIN_H
#define LOGIN_H

#include "../inc/libs.h"
#include "../inc/messenger.h"
#include "../inc/headerbar.h"
#include "../inc/request.h"

#define LOGIN_WINDOW_WIDTH 575
#define LOGIN_WINDOW_HEIGHT 525

gboolean create_login_window(gpointer user_data);
void toggle_password_visibility_icon(GtkEntry *entry, GtkEntryIconPosition icon_pos);

#endif
