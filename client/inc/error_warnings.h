#ifndef ERROR_WARNINGS_H
#define ERROR_WARNINGS_H

#include "../inc/libs.h"
#include "../inc/main_data.h"
#include "../../utils/inc/request_status.h"

gboolean update_notification_label_register(gpointer user_data);
gboolean update_notification_label_login(gpointer user_data);
gboolean change_username_error(gpointer user_data);
gboolean change_password_error(gpointer user_data);

#endif
