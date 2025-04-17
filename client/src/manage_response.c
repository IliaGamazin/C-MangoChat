#include "../inc/manage_response.h"

gboolean destroy_login_window(gpointer data) {
    (void)data;
    GtkWidget *login_window = g_object_get_data(G_OBJECT(main_data->buffer), "login-window");
    if (login_window) {
        gtk_widget_destroy(login_window);
    }

    return G_SOURCE_REMOVE;  
}

gboolean destroy_main_window(gpointer data) {
    (void)data;
    GtkWidget *main_window = g_object_get_data(G_OBJECT(main_data->buffer), "main_window");
    if (main_window) {
        gtk_widget_destroy(main_window);
    }

    return G_SOURCE_REMOVE;  
}

int response_get_status(cJSON *json) {
    int status = RQ_UNKNOWN;
    cJSON *status_item = cJSON_GetObjectItem(json, "status");
    if (status_item != NULL && cJSON_IsNumber(status_item)) {
        status = status_item->valueint;
    }
    return status;
}

void manage_signup(cJSON *json) {
    int status = response_get_status(json);
    g_idle_add(update_notification_label_register, GINT_TO_POINTER(status));

    if (status == RQ_SUCCESS) {
        printf("Signed up successfully!\n");
        return;
    }

    fprintf(stderr, "Could not sign up!\n");
}

void manage_login(cJSON *json) {
    int status = response_get_status(json);

    if (status == RQ_SUCCESS) {
        cJSON *login = cJSON_GetObjectItem(json, "id");
        int id = login->valueint;
        main_data->id = id;
        printf("Logged in successfully!\n");

        cJSON *allchats_req = form_send_id_request();
        send_request(allchats_req, USER_CHATS_GET);

        RealTalk *rt2 = RealTalk_receive(main_data->server->sd);
        if (rt2) {
            printf("\n[LISTENER THREAD] Realtalk received, ID %d\n", rt2->type);
            printf("Payload: %.*s\n\n", (int) rt2->payload->length, rt2->payload->data);
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        // I FUCKING HATE THIS FUNCTION BUT THIS PART IS FOR CHATS

        cJSON *allchats_res = cJSON_ParseWithLength(rt2->payload->data, rt2->payload->length);
        if (!allchats_res) {
            fprintf(stderr, "[MANAGE ADDING ALL CHATS]Invalid Json!\n");
        }

        int status2 = response_get_status(allchats_res);
        if (status2 != RQ_SUCCESS) {
            printf("Error getting all the chats!\n");
        }

        cJSON *chat_arr = cJSON_GetObjectItem(allchats_res, "chats");
        int chats_size = cJSON_GetArraySize(chat_arr);

        printf("Num of chats: %d\n", chats_size);

        cJSON *chat = NULL;
        cJSON_ArrayForEach(chat, chat_arr) {
            cJSON *id_item = cJSON_GetObjectItem(chat, "chat_id");
            cJSON *member_arr = cJSON_GetObjectItem(chat, "members");
            cJSON *avatar_item = cJSON_GetObjectItem(chat, "avatar");
            cJSON *name_item = cJSON_GetObjectItem(chat, "chat_name");
            cJSON *last_message_item = cJSON_GetObjectItem(chat, "last_message_id");
            int chat_id = id_item->valueint;
            int num_members = cJSON_GetArraySize(member_arr);
            const char *encoded_avatar = NULL;
            char *chat_name = "default";
            
            if (num_members > 2) {
                if (avatar_item) {
                    encoded_avatar = avatar_item->valuestring;
                }
                chat_name = name_item->valuestring;
            }
            else if (num_members < 2) {
                encoded_avatar = NULL;
                chat_name = "Notes";
            }

            (void)encoded_avatar;
            (void)chat_name;
            (void)chat_id;
            cJSON *member = NULL;
            cJSON_ArrayForEach(member, member_arr) {
                cJSON *member_id = cJSON_GetObjectItem(member, "user_id");
                cJSON *member_name = cJSON_GetObjectItem(member, "username");
                cJSON *member_avatar = cJSON_GetObjectItem(member, "avatar");
                if (g_hash_table_lookup(main_data->users_cache, GINT_TO_POINTER(member_id->valueint)) == NULL) {
                    user_add(member_id->valueint, member_name->valuestring, member_avatar->valuestring);
                }
                else {
                    printf("User %s already in map!\n", member_name->valuestring);
                }
                if (num_members == 2 && member_id->valueint != main_data->id) {
                    encoded_avatar = member_avatar->valuestring;
                    chat_name = member_name->valuestring;
                }
            }
            chat_add(id_item->valueint, last_message_item->valueint, num_members, chat_name, encoded_avatar);
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        // I FUCKING HATE THIS FUNCTION BUT THIS PART IS FOR MESSAGES

        cJSON *allmessages_req = form_send_id_request();
        send_request(allmessages_req, USER_MESSAGES_GET);

        RealTalk *rt3 = RealTalk_receive(main_data->server->sd);
        if (rt3) {
            printf("\n[LISTENER THREAD] Realtalk received, ID %d\n", rt3->type);
            printf("Payload: %.*s\n\n", (int) rt3->payload->length, rt3->payload->data);
        }

        cJSON *allmessages_res = cJSON_ParseWithLength(rt3->payload->data, rt3->payload->length);
        if (!allmessages_res) {
            fprintf(stderr, "[MANAGE ADDING ALL MESSAGES]Invalid Json!\n");
        }
        int status3 = response_get_status(allmessages_res);
        if (status3 != RQ_SUCCESS) {
            printf("Error getting all the messages!\n");
        }

        cJSON *message_arr = cJSON_GetObjectItem(allmessages_res, "messages");
        printf("Num of mesages: %d\n", cJSON_GetArraySize(message_arr));
        cJSON *message = NULL;
        cJSON_ArrayForEach(message, message_arr) {
            printf("debug mes\n");
            cJSON *id = cJSON_GetObjectItem(message, "message_id");
            main_data->message_order = g_list_append(main_data->message_order, GINT_TO_POINTER(id->valueint));
            cJSON *chat_id = cJSON_GetObjectItem(message, "to_chat_id");
            cJSON *sender_id = cJSON_GetObjectItem(message, "from_user_id");
            cJSON *text = cJSON_GetObjectItem(message, "message_text");
            cJSON *time = cJSON_GetObjectItem(message, "sent_date");

            int timestamp = atoi(time->valuestring); 
            message_add(id->valueint, chat_id->valueint, sender_id->valueint, text->valuestring, (time_t)timestamp);
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        
        g_idle_add(create_main_window, NULL);
        g_idle_add(destroy_login_window, NULL);
        return;
    }

    g_idle_add(update_notification_label_login, GINT_TO_POINTER(status));

    fprintf(stderr, "Could not log in!\n");
}


void manage_get_message(cJSON *json) {
    int status = response_get_status(json);
    if (status == RQ_SUCCESS) {
        printf("Set successfuly!\n");

        cJSON *id = cJSON_GetObjectItem(json, "message_id");
        cJSON *text = cJSON_GetObjectItem(json, "message_text");

        Message *m = g_hash_table_lookup(main_data->messages_cache, GINT_TO_POINTER(id->valueint));

        if(m) {
            m->text = strdup(text->valuestring);
            g_idle_add(update_single_message, GINT_TO_POINTER(id->valueint));
        }

        return;
    }
    fprintf(stderr, "Could not !\n");
}

void manage_set_username(cJSON *json) {
    int status = response_get_status(json);
    if (status == RQ_SUCCESS) {
        printf("Changed username successfuly!\n");
        if (main_data->username) {
            free(main_data->username);
        }
        g_idle_add(create_login_window, NULL);
        g_idle_add(destroy_main_window, NULL);
        return;
    }

    g_idle_add(change_username_error, NULL);

    fprintf(stderr, "Could not change username!\n");
}

void manage_set_password(cJSON *json) {
    int status = response_get_status(json);
    if (status == RQ_SUCCESS) {
        printf("Changed password successfuly!\n");
        if (main_data->username) {
            free(main_data->username);
        }
        g_idle_add(create_login_window, NULL);
        g_idle_add(destroy_main_window, NULL);
        return;
    }

    g_idle_add(change_password_error, NULL);

    fprintf(stderr, "Could not change password!\n");
}

void manage_set_avatar(cJSON *json) {
    int status = response_get_status(json);
    if (status == RQ_SUCCESS) {
        
        printf("Set avatar successfuly!\n");
        return;
    }
    fprintf(stderr, "Could not change avatar!\n");
}


void manage_get_avatar(cJSON *json) {
    int status = response_get_status(json);
    if (status == RQ_SUCCESS) {
        printf("Got avatar successfuly!\n");
        cJSON *avatar_item = cJSON_GetObjectItem(json, "avatar");
        unsigned long stream_size;
        unsigned char *decoded_stream = g_base64_decode(avatar_item->valuestring, &stream_size);

        main_data->avatar = create_rounded_pixbuf_from_memory (decoded_stream, stream_size, 42, 42);
        g_idle_add(on_avatar_received, NULL);
        g_free(decoded_stream);
        return;
    }
    fprintf(stderr, "Could not get avatar!\n");
}

void manage_message_create(cJSON *json) {
    int status = response_get_status(json);
    if (status != RQ_SUCCESS) {
        return;
    }
    printf("Received message succesfully!\n");
    cJSON *id = cJSON_GetObjectItem(json, "message_id");
    cJSON *chat_id = cJSON_GetObjectItem(json, "to_chat_id");
    cJSON *sender_id = cJSON_GetObjectItem(json, "from_user_id");
    cJSON *text = cJSON_GetObjectItem(json, "message_text");
    cJSON *time = cJSON_GetObjectItem(json, "sent_date");

    int timestamp = atoi(time->valuestring); 
    Chat* chat = g_hash_table_lookup(main_data->chats_cache, GINT_TO_POINTER(chat_id->valueint));
    chat->last_message_id = id->valueint;
    message_add_and_draw(id->valueint, chat_id->valueint, sender_id->valueint, text->valuestring, (time_t)timestamp);
    g_idle_add(update_single_chat, GINT_TO_POINTER(chat_id->valueint));
    g_print("%d\n", chat->last_message_id);
}

void manage_chat_create(cJSON *json) {
    int status = response_get_status(json);
    if (status != RQ_SUCCESS) {
        return;
    }
    printf("Received chat succesfully!\n");

    cJSON *id_item = cJSON_GetObjectItem(json, "id");
    cJSON *avatar_item = cJSON_GetObjectItem(json, "avatar");
    cJSON *name_item = cJSON_GetObjectItem(json, "chat_name");
    cJSON *member_arr = cJSON_GetObjectItem(json, "members");
    cJSON *member = NULL;

    int id = id_item->valueint;
    int num_members = cJSON_GetArraySize(member_arr);
    const char *encoded_avatar = NULL;
    char *chat_name = "default";

    if (num_members > 2) {
        if (avatar_item) {
            encoded_avatar = avatar_item->valuestring;
        }
        chat_name = name_item->valuestring;
    }
    else if (num_members < 2) {
        encoded_avatar = NULL;
        chat_name = "Notes";
    }

    cJSON_ArrayForEach(member, member_arr) {
        cJSON *member_id = cJSON_GetObjectItem(member, "user_id");
        cJSON *member_name = cJSON_GetObjectItem(member, "username");
        cJSON *member_avatar = cJSON_GetObjectItem(member, "avatar");
        if (g_hash_table_lookup(main_data->users_cache, GINT_TO_POINTER(member_id->valueint)) == NULL) {
            user_add(member_id->valueint, member_name->valuestring, member_avatar->valuestring);
        }
        else {
            printf("User %s already in map!\n", member_name->valuestring);
        }
        if (num_members == 2 && member_id->valueint != main_data->id) {
            encoded_avatar = member_avatar->valuestring;
            chat_name = member_name->valuestring;
        }
    }
    
    chat_add_and_draw(id, -1, num_members, chat_name, encoded_avatar);
    return;
}

int manage_response(enum e_RealTalkType type, Payload *payload) {
    int rc = 0;
    if (!payload || !payload->data) {
        return -1;
    }

    cJSON *json = cJSON_ParseWithLength(payload->data, payload->length);
    if (!json) {
        fprintf(stderr, "[MANAGE RESPONSE]Invalid Json!\n");
    }

    switch (type) {
        case USER_REGISTER:
            manage_signup(json);
            break;
        case USER_LOGIN:
            manage_login(json);
            break;
        case USER_SET_USERNAME:
            manage_set_username(json);
            break;
        case USER_SET_PASSWORD:
            manage_set_password(json);
            break;
        case USER_SET_AVATAR:
            manage_set_avatar(json);
            break;
        case USER_GET_AVATAR:
            manage_get_avatar(json);
            break;
        case USER_CHAT_CREATE:
            manage_chat_create(json);
            break;
        case USER_MESSAGE_CREATE:
            manage_message_create(json);
            break;
        case USER_MESSAGE_GET:
            manage_get_message(json);
            break;
        default:
            break;
    }

    cJSON_Delete(json);
    return rc;
}

