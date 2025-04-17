#ifndef MESSAGES_H
#define MESSAGES_H

#include "../inc/libs.h"
#include "../inc/main_data.h"
#include "../inc/request.h"

void on_chat_entry_activate(GtkEntry *entry, gpointer user_data);
void foreach_draw_message(void);

#endif
