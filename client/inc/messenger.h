#ifndef MESSENGER_H
#define MESSENGER_H

#include "../inc/libs.h"
#include "../inc/headerbar.h"
#include "../inc/main_data.h"
#include "../inc/login.h"
#include "../inc/settings.h"
#include "../inc/request.h"
#include "../inc/contacts_groups.h"
#include "../inc/avatar_handler.h"
#include "../inc/session_cache.h"
#include "../inc/messages.h"
#include "../inc/searchbar.h"


#define MAIN_WINDOW_WIDTH 1280
#define MAIN_WINDOW_HEIGHT 720

void import_css_theme(const char* theme);
GdkPixbuf *create_rounded_pixbuf(gchar *selected_file, int dest_width, int dest_height);
gboolean create_main_window(gpointer user_data);
gboolean update_connection_status(gpointer user_data);

#endif
