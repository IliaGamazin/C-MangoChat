#include "../inc/validator.h"

bool strings_match(const char *str1, const char *str2) {
    if (!str1 || !str2) {
        return false;
    }

    return strcmp(str1, str2) == 0; 
}

GList *retrieve_chat_members(GtkWidget *member_list_box) {
    GList *member_list = NULL;
    GList *rows = gtk_container_get_children(GTK_CONTAINER(member_list_box));
    for (GList *iter = rows; iter != NULL; iter = iter->next) {
        GtkWidget *row = GTK_WIDGET(iter->data);
        if (!GTK_IS_LIST_BOX_ROW(row)) {
            continue;
        }

        GtkWidget *member_box = gtk_bin_get_child(GTK_BIN(row));
        if (!member_box || !GTK_IS_WIDGET(member_box)) {
            continue;
        }

        GtkWidget *member_name_label = g_object_get_data(G_OBJECT(member_box), "member_name_label");
        const gchar *member_name = gtk_label_get_label(GTK_LABEL(member_name_label));
        member_list = g_list_append(member_list, g_strdup(member_name));
    }

    g_list_free(rows);
    return member_list;
}

bool has_dup_member(GList *members, const gchar *member_name) {
    if (strings_match(member_name, main_data->username)) {
        return true;
    }
    for (GList *iter = members; iter != NULL; iter = iter->next) {
        gchar *member = iter->data;
        if (!member) {
            continue;
        }
        if (strings_match(member, member_name)) {
            return true;
        }
    }

    return false;
}

void set_valid_entry_color(GtkWidget *entry, gboolean is_valid) {
    if (!entry) return;

    GtkCssProvider *provider = gtk_css_provider_new();
    const char *css_data = is_valid
        ? "entry { border: 2px solid orange; }"  
        : "entry { border: 2px solid red; }";   

    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(entry);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);
}

bool username_validator(const char *username) {
    if (!username || !*username) { 
        return false;
    }

    while (*username) {
        if (!isalnum((unsigned char)*username)) { 
            return false;
        }
        username++;
    }

    return true; 
}

bool password_validator(const char *password) {
    if (!password || strlen(password) < 8) { 
        return false;
    }

    bool has_letter = false;
    bool has_digit = false;
    const char *p = password;

    while (*p) {
        if (isalpha((unsigned char)*p)) {
            has_letter = true;
        } 
        else if (isdigit((unsigned char)*p)) {
            has_digit = true;
        }
        else { return false; }

        p++; 
    }

    return has_letter && has_digit;
}

