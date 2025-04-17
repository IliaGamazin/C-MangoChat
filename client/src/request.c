#include "../inc/request.h"

cJSON *form_login_request(const char *username, const char *password) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[LOGIN REQUEST] Error creating JSON object\n");
        return NULL;
    }

    cJSON_AddStringToObject(json, "username", username);
    cJSON_AddStringToObject(json, "password", password);

    return json;
}

cJSON *form_signup_request(const char *username, const char *password) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[SIGNUP REQUEST] Error creating JSON object\n");
        return NULL;
    }

    cJSON_AddStringToObject(json, "username", username);
    cJSON_AddStringToObject(json, "password", password);

    return json;
}

cJSON *form_change_avatar_request(const char *filename) {
    if (!filename) {
        fprintf(stderr, "[CHANGE AVATAR REQUEST] Invalid filename\n");
        return NULL;
    }

    long file_size;
    unsigned char *file_data = read_file_to_buffer(filename, &file_size);
    if (!file_data) {
        fprintf(stderr, "[CHANGE AVATAR REQUEST] Error reading avatar file\n");
        return NULL;
    }

    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[CHANGE AVATAR REQUEST] Error creating JSON object\n");
        free(file_data);
        return NULL;
    }

    gchar *encoded_file = g_base64_encode(file_data, file_size);

    cJSON_AddNumberToObject(json, "id", main_data->id);
    cJSON_AddStringToObject(json, "avatar", encoded_file);

    g_free(encoded_file);
    free(file_data);
    return json;
}

cJSON *form_change_password_request(const char *password) {
    if (!password) {
	    fprintf(stderr, "[CHANGE PASSWORD REQUEST] No password provided \n");
    }

    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[CHANGE PASSWORD REQUEST] Error creating JSON object\n");
        return NULL;
    }

    cJSON_AddNumberToObject(json, "id", main_data->id);
    cJSON_AddStringToObject(json, "password", password);
    return json;
}

cJSON *form_change_username_request(const char *username) {
    if (!username) {
	    fprintf(stderr, "[CHANGE USERNAME REQUEST] No username provided \n");
    }

    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[CHANGE REQUEST] Error creating JSON object\n");
        return NULL;
    }

    cJSON_AddNumberToObject(json, "id", main_data->id);
    cJSON_AddStringToObject(json, "username", username);

    return json;
}

cJSON *form_create_chat_request (const char *chat_name, char *avatar_filename, GList *member_list) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[CREATE CHAT REQUEST] Error creating JSON object\n");
        return NULL;
    }

	cJSON_AddItemToObject(json, "chat_name", chat_name ? cJSON_CreateString(chat_name) : cJSON_CreateNull());

    long file_size;
    gchar *chat_avatar = NULL;
    if (avatar_filename) {
        unsigned char *file_data = read_file_to_buffer(avatar_filename, &file_size);
        chat_avatar = g_base64_encode(file_data, file_size);
    }

    cJSON_AddItemToObject(json, "chat_avatar", chat_avatar ? cJSON_CreateString(chat_avatar) : cJSON_CreateNull());

    cJSON *members_array = cJSON_CreateArray();
    if (!members_array) {
        fprintf(stderr, "[CREATE CHAT REQUEST] Error creating JSON array\n");
        cJSON_Delete(json);
        return NULL;
    }

    for (GList *iter = member_list; iter != NULL; iter = iter->next) {
        if (iter->data) {
            cJSON *member_name = cJSON_CreateString((const char *)iter->data);
            if (member_name) {
                cJSON_AddItemToArray(members_array, member_name);
                continue;
            }
            fprintf(stderr, "[CREATE CHAT REQUEST] Error adding member to JSON array\n");
        }
    }

    cJSON_AddItemToObject(json, "members", members_array);

    return json;
}

cJSON *form_send_message_request(char *text, bool is_file) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[SEND MESSAGE REQUEST] Error creating JSON object\n");
        return NULL;
    }

    if (!text || !strlen(text)) {
        fprintf(stderr, "[SEND MESSAGE REQUEST] Invalid message\n");
        cJSON_Delete(json);
        return NULL;
    }

    if (main_data->current_chat_id == -1) {
        fprintf(stderr, "[SEND MESSAGE REQUEST] Chat is not chosen\n");
        cJSON_Delete(json);
        return NULL;
    }

    if (main_data->id == -1) {
        fprintf(stderr, "[SEND MESSAGE REQUEST] Woah huh\n");
        cJSON_Delete(json);
        return NULL;
    }

    cJSON_AddNumberToObject(json, "chat_id", main_data->current_chat_id);
    cJSON_AddNumberToObject(json, "sender_id", main_data->id);
    cJSON_AddNumberToObject(json, "sent_time", time(NULL));
    cJSON_AddStringToObject(json, "text", text);
    if (is_file) {
        long file_size;
        unsigned char *file_data = read_file_to_buffer(text, &file_size);
        gchar *attachment = g_base64_encode(file_data, file_size);
        cJSON_AddStringToObject(json, "attachment", attachment);
    }

    return json;
}

cJSON *form_edit_message_request(int message_id, char *new_text) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[EDIT MESSAGE REQUEST] Error creating JSON object\n");
        return NULL;
    }
    cJSON_AddNumberToObject(json, "message_id", message_id);
    if (!new_text || !strlen(new_text)) {
        printf("Editing to '', really? : %d\n", message_id);
        cJSON_AddStringToObject(json, "text", "Wow! The message is no more :(");
    }
    else {
        cJSON_AddStringToObject(json, "text", new_text);
    }   

    return json;
}

cJSON *form_send_id_request(void) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        fprintf(stderr, "[SEND ID] Error creating JSON object\n");
        return NULL;
    }

    if (main_data->id == -1) {
        fprintf(stderr, "[SEND ID] Woah huh\n");
        cJSON_Delete(json);
        return NULL;
    }
    cJSON_AddNumberToObject(json, "id", main_data->id);

    return json;
}

void send_request(cJSON *json, int type) {
    if (!json) {
        fprintf(stderr, "[SEND REQUEST] json is NULL\n");
        return;
    }

    char *json_string = cJSON_PrintUnformatted(json);
    
    if (main_data->is_connected) {
        printf("\n[SEND REQUEST] Request: %d %s\n",type, json_string);
        RealTalk *rt = RealTalk_new(type, strlen(json_string), json_string);
        RealTalk_send(main_data->server->sd, rt);
        RealTalk_destroy(&rt);
    }
    
    cJSON_Delete(json);
    free(json_string);
}


