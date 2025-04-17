#include "../inc/login.h"

static void switch_to_signup_mode(GtkWidget *widget, gpointer data) {
    (void) widget;
    GtkWidget *login_window = GTK_WIDGET(data);

    GtkWidget *confirm_password_entry = g_object_get_data(G_OBJECT(login_window), "confirm_password_entry");
    GtkWidget *signup_button = g_object_get_data(G_OBJECT(login_window), "signup_button");
    GtkWidget *login_existing_account_button = g_object_get_data(G_OBJECT(login_window), "login_existing_account_button");
    GtkWidget *login_button = g_object_get_data(G_OBJECT(login_window), "login_button");
    GtkWidget *create_account_button = g_object_get_data(G_OBJECT(login_window), "create_account_button");
    GtkWidget *notification_label = g_object_get_data(G_OBJECT(login_window), "notification_label");

    gtk_widget_show(confirm_password_entry);
    gtk_widget_show(signup_button);
    gtk_widget_show(login_existing_account_button);

    gtk_widget_hide(notification_label);
    gtk_widget_hide(login_button);
    gtk_widget_hide(create_account_button);
}

static void switch_to_login_mode(GtkWidget *widget, gpointer data) {
    (void) widget;
    GtkWidget *login_window = GTK_WIDGET(data);

    GtkWidget *confirm_password_entry = g_object_get_data(G_OBJECT(login_window), "confirm_password_entry");
    GtkWidget *signup_button = g_object_get_data(G_OBJECT(login_window), "signup_button");
    GtkWidget *login_existing_account_button = g_object_get_data(G_OBJECT(login_window), "login_existing_account_button");
    GtkWidget *login_button = g_object_get_data(G_OBJECT(login_window), "login_button");
    GtkWidget *create_account_button = g_object_get_data(G_OBJECT(login_window), "create_account_button");
    GtkWidget *notification_label = g_object_get_data(G_OBJECT(login_window), "notification_label");

    gtk_widget_hide(confirm_password_entry);
    gtk_widget_hide(signup_button);
    gtk_widget_hide(login_existing_account_button);
    gtk_widget_hide(notification_label);

    gtk_widget_show(login_button);
    gtk_widget_show(create_account_button);
}

static void on_signup_button_clicked(GtkWidget *widget, gpointer user_data) {
    (void) user_data;
    GtkWidget *username_entry = g_object_get_data(G_OBJECT(widget), "username_entry");
    GtkWidget *password_entry = g_object_get_data(G_OBJECT(widget), "password_entry");
    GtkWidget *confirm_entry = g_object_get_data(G_OBJECT(widget), "confirm_password_entry");

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
    const gchar *confirm = gtk_entry_get_text(GTK_ENTRY(confirm_entry));

    set_valid_entry_color(username_entry, true);
    set_valid_entry_color(password_entry, true);
    set_valid_entry_color(confirm_entry, true);

    bool valid = true;

    if (!username_validator(username)) {
        set_valid_entry_color(username_entry, false);
        valid = false;
    }
    if (!password_validator(password)) {
        set_valid_entry_color(password_entry, false);
        valid = false;
    }
    if (!strings_match(password, confirm)) {
        set_valid_entry_color(password_entry, false);
        set_valid_entry_color(confirm_entry, false);
        valid = false;
    }
    if (!valid) {
        return;
    }

    cJSON *json = form_signup_request(username, password);    
    send_request(json, USER_REGISTER);
}

static void on_login_button_clicked(GtkWidget *widget, gpointer user_data) {
    (void)user_data;
    (void)widget;
    GtkWidget *username_entry = g_object_get_data(G_OBJECT(widget), "username_entry");
    GtkWidget *password_entry = g_object_get_data(G_OBJECT(widget), "password_entry");

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
    
    bool valid = true;

    set_valid_entry_color(username_entry, true);
    set_valid_entry_color(password_entry, true);

    if (!username_validator(username)) {
        set_valid_entry_color(username_entry, false);
        valid = false;
    }
    if (!password_validator(password)) {
        set_valid_entry_color(password_entry, false);
        valid = false;
    }
    if (!valid) {
        return;
    }

    cJSON *json = form_login_request(username, password);    
    main_data->username = strdup(username);
    send_request(json, USER_LOGIN);
}

void toggle_password_visibility_icon(GtkEntry *entry, GtkEntryIconPosition icon_pos) {
    if (icon_pos != GTK_ENTRY_ICON_SECONDARY) {
        return;
    }

    gboolean visible = !gtk_entry_get_visibility(entry);
    gtk_entry_set_visibility(entry, visible);

    const char *icon_name = visible ? "view-conceal-symbolic" : "view-reveal-symbolic";
    gtk_entry_set_icon_from_icon_name(entry, GTK_ENTRY_ICON_SECONDARY, icon_name);
}

gboolean create_login_window(gpointer user_data) {
    (void) user_data;
    GtkWidget *login_window;
    GtkWidget *login_box;
    GtkWidget *logo_box;
    GtkWidget *logo_label;
    GtkWidget *title_label;
    GtkWidget *background_box;
    GtkWidget *form_box;
    GtkWidget *notification_label;
    GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *confirm_password_entry;
    GtkWidget *login_button;
    GtkWidget *signup_button;
    GtkWidget *create_account_button;
    GtkWidget *login_existing_account_button;

    login_window = gtk_application_window_new(main_data->app);
    gtk_window_set_title(GTK_WINDOW(login_window), "MangoChat - Login");
    gtk_widget_set_name(login_window, "login_window");
    gtk_window_set_default_size(GTK_WINDOW(login_window), LOGIN_WINDOW_WIDTH, LOGIN_WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(login_window), FALSE);

    headbar_handler(login_window, "img/green_mango.svg", "img/red_mango.svg", "img/orange_mango.svg", 0);

    login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(login_box), 20);
    gtk_container_add(GTK_CONTAINER(login_window), login_box);

    logo_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(login_box), logo_box, FALSE, FALSE, 0);

    logo_label = gtk_image_new_from_file("img/mango.svg");
    gtk_widget_set_name(logo_label, "logo");
    gtk_box_pack_start(GTK_BOX(logo_box), logo_label, FALSE, FALSE, 0);

    title_label = gtk_label_new("MangoChat");
    gtk_widget_set_name(title_label, "title");
    gtk_box_pack_start(GTK_BOX(logo_box), title_label, FALSE, FALSE, 5);
    
    background_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(background_box, "background_box");
    gtk_box_pack_start(GTK_BOX(login_box), background_box, FALSE, FALSE, 0);
    gtk_widget_set_halign(background_box,
    GTK_ALIGN_CENTER);
    
    form_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(background_box), form_box, FALSE, FALSE, 5);

    notification_label = gtk_label_new("");
    gtk_widget_set_name(notification_label, "notification_label");
    gtk_widget_set_halign(notification_label, GTK_ALIGN_CENTER); 
    gtk_widget_set_valign(notification_label, GTK_ALIGN_START);  
    gtk_box_pack_start(GTK_BOX(form_box), notification_label, FALSE, FALSE, 0);
    gtk_widget_set_margin_bottom(notification_label, 5);

    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Username");
    gtk_widget_set_name(username_entry, "username_entry");
    gtk_widget_set_halign(username_entry, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(form_box), username_entry, FALSE, FALSE, 5);
    
    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_widget_set_name(password_entry, "password_entry");
    gtk_widget_set_halign(password_entry, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(form_box), password_entry, FALSE, FALSE, 10);
    gtk_entry_set_icon_from_icon_name(GTK_ENTRY(password_entry), GTK_ENTRY_ICON_SECONDARY, "view-reveal-symbolic");

    confirm_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password_entry), "Confirm password");
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE);
    gtk_widget_set_name(confirm_password_entry, "confirm_password_entry");
    gtk_widget_set_halign(confirm_password_entry, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(form_box), confirm_password_entry, FALSE, FALSE, 10);
    gtk_entry_set_icon_from_icon_name(GTK_ENTRY(confirm_password_entry), GTK_ENTRY_ICON_SECONDARY, "view-reveal-symbolic");

    signup_button = gtk_button_new_with_label("Sign up");
    gtk_widget_set_name(signup_button, "signup_button");
    gtk_box_pack_start(GTK_BOX(form_box), signup_button, FALSE, FALSE, 10);
    g_object_set_data(G_OBJECT(signup_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(signup_button), "password_entry", password_entry);
    g_object_set_data(G_OBJECT(signup_button), "confirm_password_entry", confirm_password_entry);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(on_signup_button_clicked), login_window);

    login_existing_account_button = gtk_button_new_with_label("Log in existing account");
    gtk_widget_set_name(login_existing_account_button, "login_existing_account_button");
    gtk_widget_set_halign(login_existing_account_button, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(form_box), login_existing_account_button, FALSE, FALSE, 0);

    login_button = gtk_button_new_with_label("Log in");
    gtk_widget_set_name(login_button, "login_button");
    gtk_box_pack_start(GTK_BOX(form_box), login_button, FALSE, FALSE, 10);
    g_object_set_data(G_OBJECT(login_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);

    create_account_button = gtk_button_new_with_label("Create account");
    gtk_widget_set_name(create_account_button, "create_account_button");
    gtk_widget_set_halign(create_account_button, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(form_box), create_account_button, FALSE, FALSE, 0);

    g_object_set_data(G_OBJECT(login_window), "notification_label", notification_label);
    g_object_set_data(G_OBJECT(login_window), "confirm_password_entry", confirm_password_entry);
    g_object_set_data(G_OBJECT(login_window), "signup_button", signup_button);
    g_object_set_data(G_OBJECT(login_window), "login_existing_account_button", login_existing_account_button);
    g_object_set_data(G_OBJECT(login_window), "login_button", login_button);
    g_object_set_data(G_OBJECT(login_window), "create_account_button", create_account_button);

    g_signal_connect(password_entry, "icon-press", G_CALLBACK(toggle_password_visibility_icon), NULL);
    g_signal_connect(confirm_password_entry, "icon-press", G_CALLBACK(toggle_password_visibility_icon), NULL);
    g_signal_connect(create_account_button, "clicked", G_CALLBACK(switch_to_signup_mode), login_window);
    g_signal_connect(login_existing_account_button, "clicked", G_CALLBACK(switch_to_login_mode), login_window);

    g_object_set_data(G_OBJECT(main_data->buffer), "login-window", login_window);

    gtk_widget_grab_focus(create_account_button);

    gtk_widget_show_all(login_window);
    gtk_widget_hide(login_existing_account_button);
    gtk_widget_hide(signup_button);
    gtk_widget_hide(confirm_password_entry);
    gtk_widget_hide(notification_label);

    return G_SOURCE_REMOVE;
}

