#include "../inc/session_cache.h"

User *user_encache(int id, char *name, const char *encoded_avatar) {
    User *res = malloc(sizeof(User));
    res->user_id = id;
    res->name = strdup(name);
    if (encoded_avatar) {
        unsigned long stream_size;
        unsigned char *decoded_stream = g_base64_decode(encoded_avatar, &stream_size);
        res->avatar = create_rounded_pixbuf_from_memory (decoded_stream, stream_size, 42, 42);
        g_free(decoded_stream);
    }
    else {
        res->avatar = create_rounded_pixbuf("img/placeholder_avatar.svg", 42, 42);
    }
    printf("User %s added!\n", res->name);
    return res;
}

void user_free(User *user) {
    if (!user) {
        return;
    }
    if (user->name) {
        free(user->name);
    }
    user->user_id = -1;
}

void user_destroy(void **user) {
    if (!user || !*user) {
        return;
    }

    user_free(*user);
    free(*user);
}

void user_add(int id, char *name, const char *encoded_avatar) {
    User *user = user_encache(id, name, encoded_avatar);
    g_hash_table_insert(main_data->users_cache, GINT_TO_POINTER(id), user);
}

