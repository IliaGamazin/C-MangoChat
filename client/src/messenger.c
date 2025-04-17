#include "../inc/messenger.h"

void on_emoji_selected(GtkButton *button, gpointer user_data) {
    (void)user_data;
    GtkWidget *chat_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "chat_entry"));

    if (chat_entry) {
        const gchar *emoji = gtk_button_get_label(button); // Get emoji label
        const gchar *current_text = gtk_entry_get_text(GTK_ENTRY(chat_entry)); // Get current text

        gchar *new_text = g_strconcat(current_text, emoji, NULL);
        gtk_entry_set_text(GTK_ENTRY(chat_entry), new_text);

        g_free(new_text);
    }
}

static void on_file_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *parent_window = GTK_WIDGET(user_data);
    GtkWidget *dialog = create_filechooser_window(parent_window, true);
    gchar *selected_file = NULL;

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        selected_file = gtk_file_chooser_get_filename(chooser);
    }

    if (selected_file) {
        printf("selected file: %s\n", selected_file);
        cJSON *json = form_send_message_request(selected_file, true);
        send_request(json, 20);
        g_free(selected_file);
    }

    gtk_widget_destroy(dialog);
}

static void on_emoji_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *emoji_popover = gtk_popover_new(GTK_WIDGET(button));
    gtk_widget_set_name(emoji_popover, "emoji_popover");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(box, "emoji_popover_box");
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(emoji_popover), box);

    const gchar *emojis[] = {
        "💀", "🔥", "🤡", "🎲", "🥭", "🛖", "🥩", "🥶",
        "🗿", "🍀", "💎", "🧠", "🐝", "🐍", "🤔", "🎰",
        "🍻", "🍄", "🚬", "🚀", "🧊", "🎩", "💰", "💼",
        "🐼", "🐍", "📦", "🐼", "🦦", "🍺", "🦊", "🕯️",
        "🌵", "🎁", "🪝", "🍷", "👹", "🥃", "🧉", "🦾"
    };

    int num_emojis = sizeof(emojis) / sizeof(emojis[0]);
    const int columns = 8;

    GtkWidget *chat_entry = GTK_WIDGET(user_data);

    for (int i = 0; i < num_emojis; i++) {
        GtkWidget *emoji_button = gtk_button_new_with_label(emojis[i]);
        gtk_widget_set_name(emoji_button, "emoji_button");
        gtk_widget_set_margin_start(emoji_button, 5);
        gtk_widget_set_margin_end(emoji_button, 5);
        gtk_widget_set_margin_top(emoji_button, 5);
        gtk_widget_set_margin_bottom(emoji_button, 5);

        g_object_set_data(G_OBJECT(emoji_button), "chat_entry", chat_entry);
        g_signal_connect(emoji_button, "clicked", G_CALLBACK(on_emoji_selected), NULL);

        gtk_grid_attach(GTK_GRID(grid), emoji_button, i % columns, i / columns, 1, 1);
    }

    gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 0);

    gtk_widget_show_all(emoji_popover);
    gtk_popover_popup(GTK_POPOVER(emoji_popover));
}

static void update_button_styles(GtkToggleButton *button, GtkToggleButton *other_button) {
    if (!gtk_toggle_button_get_active(button)) {
        return;
    }

    GtkWidget *label = gtk_bin_get_child(GTK_BIN(button));
    GtkWidget *other_label = gtk_bin_get_child(GTK_BIN(other_button));

    gtk_widget_set_name(label, "bold_label");
    gtk_widget_set_name(other_label, "normal_label");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(other_button), FALSE);
}

gboolean update_connection_status(gpointer user_data) {
    (void)user_data;
    GtkWidget *connection_icon = g_object_get_data(G_OBJECT(main_data->buffer), "connection_lost_icon");
    if (!connection_icon) {
        return FALSE;
    }

    if (main_data->is_connected) {
        gtk_widget_set_visible(connection_icon, FALSE);
        return G_SOURCE_REMOVE;
    }

    gtk_widget_set_visible(connection_icon, TRUE);
    return G_SOURCE_REMOVE;
}

GtkWidget *create_list_box(GtkWidget *chat_list_scroll) {

    GtkWidget *chat_list_box = gtk_list_box_new();
    gtk_widget_set_name(chat_list_box, "chat_list_box");
    gtk_widget_set_size_request(chat_list_box, 290, -1);
    gtk_container_add(GTK_CONTAINER(chat_list_scroll), chat_list_box);
    gtk_widget_set_vexpand(chat_list_box, TRUE);
    gtk_widget_show(chat_list_scroll);
    gtk_widget_show(chat_list_box);
    GtkWidget *custom_scrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(chat_list_scroll));
    gtk_widget_set_name(custom_scrollbar, "custom_scrollbar");

    g_object_set_data(G_OBJECT(chat_list_scroll), "chat_list_box", chat_list_box);

    return chat_list_box;
}

gboolean create_main_window(gpointer user_data) {
    /* Main window containers */
    (void) user_data;
    GtkWidget *main_window;
    GtkWidget *main_box;

    /* Sidebar containers     */
    GtkWidget *sidebar_box;
    GtkWidget *sidebar_top_box;

    /* Avatar button          */
    GtkWidget *avatar_settings_button;
    GtkWidget *avatar_image;

    /* Search containers      */
    GtkWidget *search_box;
    GtkWidget *search_entry;

    /* Logo containers        */
    GtkWidget *mango_box;
    GtkWidget *mango_icon;
    GtkWidget *mango_label;

    /* Connection warning     */
    GtkWidget *connection_lost_icon;

    /* Chat containers        */
    GtkWidget *chat_box;
    GtkWidget *chat_message_container_scroll;

    /* Top bar containers     */
    GtkWidget *topbar_box;
    GtkWidget *chat_info_box;
    GtkWidget *chat_avatar_topbar_box;
    GtkWidget *avatar_icon_topbar;
    GtkWidget *chat_name_topbar;
    GtkWidget *search_message_box;
    GtkWidget *search_message_entry;

    /* Chat boxes*/


    /* Switch Chats / Groups   */
    GtkWidget *switch_chats_groups_box;
    GtkWidget *add_new_contact_button;
    GtkWidget *add_new_group_button;
    GtkWidget *chats_button;
    GtkWidget *groups_button;
    GtkWidget *arrow_image;

    /* file buttons           */
    GtkWidget *chat_entry_box;
    GtkWidget *chat_entry;
    GtkWidget *emoji_button;
    GtkWidget *emoji_button_image;
    GtkWidget *file_button;
    GtkWidget *file_button_image;
    GdkGeometry hints;

    /* Chat textbox,  emoji & */

    /* Setup main window       */

    main_window = gtk_application_window_new(main_data->app);
    main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkStyleContext *main_context = gtk_widget_get_style_context(main_window);
    gtk_style_context_add_class(main_context, "main_window");

    hints.min_width = 800;
    hints.min_height = 600;
    gtk_window_set_title(GTK_WINDOW(main_window), "Mango-chat-main");
    gtk_widget_set_name(main_window, "main_window");
    gtk_window_set_default_size(GTK_WINDOW(main_window), MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(main_window), TRUE);
    gtk_window_set_geometry_hints(GTK_WINDOW(main_window), NULL, &hints, GDK_HINT_MIN_SIZE);
    gtk_container_add(GTK_CONTAINER(main_window), main_box);

    headbar_handler(main_window, "img/green_mango.svg", "img/red_mango.svg", "img/orange_mango.svg", 1);

    sidebar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(sidebar_box, 296, -1);
    gtk_widget_set_name(sidebar_box, "sidebar");
    gtk_box_pack_start(GTK_BOX(main_box), sidebar_box, FALSE, FALSE, 0);

    sidebar_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_name(sidebar_top_box, "sidebar_top");

    avatar_image = gtk_image_new_from_file("img/placeholder_avatar.svg");
    gtk_widget_set_halign(avatar_image, GTK_ALIGN_START);
    g_object_set_data(G_OBJECT(main_window), "avatar_image", avatar_image);

    avatar_settings_button = gtk_button_new();
    gtk_widget_set_name(avatar_settings_button, "avatar_setting");
    gtk_button_set_image(GTK_BUTTON(avatar_settings_button), avatar_image);
    g_signal_connect(avatar_settings_button, "clicked", G_CALLBACK(on_avatar_settings_clicked), main_window);

    search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(search_box, "search_box");

    search_entry = gtk_entry_new();
    gtk_widget_set_name(search_entry, "search_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search contact...");
    gtk_widget_set_halign(search_entry, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(search_entry, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(search_entry, 150, 42);
    g_signal_connect(search_entry, "activate", G_CALLBACK(on_search_chats_activate), NULL);

    switch_chats_groups_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_name(switch_chats_groups_box, "switch_chats_groups_box");

    add_new_contact_button = gtk_button_new();
    gtk_widget_set_name(add_new_contact_button, "add_contact_button");
    gtk_button_set_image(GTK_BUTTON(add_new_contact_button), gtk_image_new_from_file("img/add_contact.svg"));
    gtk_widget_set_margin_start(add_new_contact_button, 25);
    gtk_box_pack_start(GTK_BOX(switch_chats_groups_box), add_new_contact_button, FALSE, FALSE, 0);
    g_signal_connect(add_new_contact_button, "clicked", G_CALLBACK(on_add_new_contact_button_clicked), NULL);

    add_new_group_button = gtk_button_new();
    gtk_widget_set_name(add_new_group_button, "add_group_button");
    gtk_button_set_image(GTK_BUTTON(add_new_group_button), gtk_image_new_from_file("img/add_contact.svg"));
    gtk_widget_set_margin_start(add_new_group_button, 25);
    gtk_box_pack_start(GTK_BOX(switch_chats_groups_box), add_new_group_button, FALSE, FALSE, 0);

    chats_button = gtk_toggle_button_new_with_label("Chats");
    gtk_widget_set_name(chats_button, "chats_button");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chats_button), TRUE);
    g_signal_connect(chats_button, "toggled", G_CALLBACK(on_chats_button_clicked), NULL);
    GtkWidget *chats_label = gtk_bin_get_child(GTK_BIN(chats_button));
    gtk_widget_set_name(chats_label, "bold_label");

    g_object_set_data(G_OBJECT(chats_button), "add_new_contact_button", add_new_contact_button);
    g_object_set_data(G_OBJECT(chats_button), "add_new_group_button", add_new_group_button);

    groups_button = gtk_toggle_button_new_with_label("Groups");
    gtk_widget_set_name(groups_button, "groups_button");
    g_signal_connect(groups_button, "toggled", G_CALLBACK(on_groups_button_clicked), NULL);
    g_signal_connect(groups_button, "toggled", G_CALLBACK(update_button_styles), chats_button);
    g_signal_connect(chats_button, "toggled", G_CALLBACK(update_button_styles), groups_button);
    GtkWidget *groups_label = gtk_bin_get_child(GTK_BIN(groups_button));
    gtk_widget_set_name(groups_label, "normal_label");

    g_object_set_data(G_OBJECT(groups_button), "add_new_contact_button", add_new_contact_button);
    g_object_set_data(G_OBJECT(groups_button), "add_new_group_button", add_new_group_button);

    arrow_image = gtk_image_new_from_file("img/arrow_icon.svg");

    gtk_box_pack_start(GTK_BOX(sidebar_box), sidebar_top_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_top_box), avatar_settings_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_top_box), search_box, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(search_box), search_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_box), switch_chats_groups_box, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(switch_chats_groups_box), chats_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(switch_chats_groups_box), arrow_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(switch_chats_groups_box), groups_button, TRUE, TRUE, 0);

    GtkWidget *chat_list_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_list_scroll),
                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(sidebar_box), chat_list_scroll, TRUE, TRUE, 0);

    create_list_box(chat_list_scroll);

    GtkWidget *custom_chat_scrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(chat_list_scroll));
    GtkWidget *custom_group_scrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(chat_list_scroll));
    gtk_widget_set_name(custom_chat_scrollbar, "custom_scrollbar");
    gtk_widget_set_name(custom_group_scrollbar, "custom_scrollbar");

    g_object_set_data(G_OBJECT(main_data->buffer), "chat_list_scroll", chat_list_scroll);

    g_signal_connect(add_new_group_button, "clicked", G_CALLBACK(on_add_new_group_button_clicked), sidebar_box);

    mango_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    gtk_widget_set_name(mango_box, "mango_label");
    gtk_box_pack_end(GTK_BOX(sidebar_box), mango_box, FALSE, FALSE, 0);

    mango_icon = gtk_image_new_from_file("img/mango_mini_transparent.svg");
    gtk_widget_set_name(mango_icon, "icon");
    gtk_box_pack_start(GTK_BOX(mango_box), mango_icon, FALSE, FALSE, 0);
    gtk_widget_set_size_request(mango_icon, 24, 24);

    mango_label = gtk_label_new("MangoChat");
    gtk_widget_set_name(mango_label, "label");
    gtk_box_pack_start(GTK_BOX(mango_box), mango_label, FALSE, FALSE, 0);

    connection_lost_icon = gtk_image_new_from_file("img/connection_lost_icon.svg");
    gtk_widget_set_name(connection_lost_icon, "connection_lost_icon");
    gtk_box_pack_start(GTK_BOX(mango_box), connection_lost_icon, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(main_data->buffer), "connection_lost_icon", connection_lost_icon);

    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), chat_box, TRUE, TRUE, 0);

    topbar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_name(topbar_box, "top_bar");
    gtk_widget_set_size_request(topbar_box, -1, 63);
    gtk_box_pack_start(GTK_BOX(chat_box), topbar_box, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(main_data->buffer), "topbar_box", topbar_box);

    chat_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_name(chat_info_box, "chat_info_box");
    gtk_widget_set_size_request(chat_info_box, 210, 42);
    gtk_box_pack_start(GTK_BOX(topbar_box), chat_info_box, FALSE, FALSE, 5);

    chat_avatar_topbar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(chat_avatar_topbar_box, "chat_avatar_topbar_box");
    gtk_box_pack_start(GTK_BOX(chat_info_box), chat_avatar_topbar_box, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(topbar_box), "chat_avatar_topbar_box", chat_avatar_topbar_box);

    avatar_icon_topbar = gtk_image_new_from_file("img/avatar_placeholder.svg");
    gtk_widget_set_name(avatar_icon_topbar, "icon");
    gtk_box_pack_start(GTK_BOX(chat_avatar_topbar_box), avatar_icon_topbar, FALSE, FALSE, 0);
    gtk_widget_set_size_request(avatar_icon_topbar, 42, 42);
    g_object_set_data(G_OBJECT(topbar_box), "avatar_icon_topbar", avatar_icon_topbar);

    chat_name_topbar = gtk_label_new("Choose a chat...");
    gtk_widget_set_name(chat_name_topbar, "chat_name_topbar");
    gtk_box_pack_start(GTK_BOX(chat_info_box), chat_name_topbar, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(topbar_box), "chat_name_topbar", chat_name_topbar);

    search_message_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(search_message_box, "search_message_box");
    gtk_widget_set_halign(search_message_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(search_message_box, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(search_message_box, 150, 42);
    gtk_box_pack_end(GTK_BOX(topbar_box), search_message_box, FALSE, FALSE, 0);

    search_message_entry = gtk_entry_new();
    gtk_widget_set_name(search_message_entry, "search_message_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_message_entry), "Search message...");
    gtk_box_pack_start(GTK_BOX(search_message_box), search_message_entry, FALSE, FALSE, 0);
    g_signal_connect(search_message_entry, "activate", G_CALLBACK(on_search_messages_activate), NULL);

    chat_message_container_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_message_container_scroll),
                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(chat_box), chat_message_container_scroll, TRUE, TRUE, 0);

    GtkWidget *custom_message_container_scrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(chat_message_container_scroll));
    gtk_widget_set_name(custom_message_container_scrollbar, "custom_scrollbar");

    GtkWidget *chat_message_container_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    g_object_set_data(G_OBJECT(main_data->buffer), "chat_message_container_box", chat_message_container_box);
    gtk_container_add(GTK_CONTAINER(chat_message_container_scroll), chat_message_container_box);

    chat_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(chat_entry_box, "chat_entry_box");
    gtk_widget_set_halign(chat_entry_box, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(chat_entry_box, 492, 48);

    file_button = gtk_button_new();
    gtk_widget_set_name(file_button, "file_button");
    gtk_widget_set_halign(file_button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(file_button, GTK_ALIGN_CENTER);
    g_signal_connect(file_button, "clicked", G_CALLBACK(on_file_button_clicked), main_window);

    file_button_image = gtk_image_new_from_file("img/file.svg");
    gtk_button_set_image(GTK_BUTTON(file_button), file_button_image);

    emoji_button = gtk_button_new();
    gtk_widget_set_name(emoji_button, "emoji_button");
    gtk_widget_set_halign(emoji_button, GTK_ALIGN_CENTER);

    emoji_button_image = gtk_image_new_from_file("img/smile.svg");
    gtk_button_set_image(GTK_BUTTON(emoji_button), emoji_button_image);

    chat_entry = gtk_entry_new();
    gtk_widget_set_name(chat_entry, "chat_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(chat_entry), "Message");
    gtk_widget_set_size_request(chat_entry, 492, 48);

    g_signal_connect(chat_entry, "activate", G_CALLBACK(on_chat_entry_activate), NULL);
    g_signal_connect(emoji_button, "clicked", G_CALLBACK(on_emoji_button_clicked), chat_entry);

    g_hash_table_foreach(main_data->chats_cache, draw_chat, NULL);
    foreach_draw_message();

    gtk_box_pack_end(GTK_BOX(chat_box), chat_entry_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(chat_entry_box), emoji_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(chat_entry_box), chat_entry, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(chat_entry_box), file_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
    gtk_widget_set_visible(connection_lost_icon, FALSE);
    gtk_widget_set_visible(add_new_group_button, FALSE);

    g_object_set_data(G_OBJECT(main_data->buffer), "main_window", main_window);
    g_object_set_data(G_OBJECT(main_data->buffer), "chat_entry_box", chat_entry_box);
    g_object_set_data(G_OBJECT(chat_entry_box), "chat_entry", chat_entry);

    cJSON *json = form_send_id_request();
    send_request(json, USER_GET_AVATAR);

    return G_SOURCE_REMOVE;
}

