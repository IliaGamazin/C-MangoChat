#include "../inc/client.h"

MainData *main_data;

static void *connection_handler(void *arg) {
    (void)arg;
    printf("\n[LISTENER THREAD] Thread entered\n");
    while (!main_data->is_closing) {
        Server_sd_init(main_data->server);
        if (Server_connect(main_data->server) == -1) {
            g_idle_add(update_connection_status, NULL);
            printf("[LISTENER THREAD] Failed to connect, reconnecting in %d...\n", main_data->reconnect_delay);
            sleep(main_data->reconnect_delay);
            continue;
        }

        printf("[LISTENER THREAD] Connecion established!\n");
        main_data->is_connected = true;
        g_idle_add(update_connection_status, NULL);
        RealTalk *rt2 = NULL;
        while (!main_data->is_closing && (rt2 = RealTalk_receive(main_data->server->sd))){
            if (rt2) {
                printf("\n[LISTENER THREAD] Realtalk received, ID %d\n", rt2->type);
                printf("Payload: %.*s\n\n", (int) rt2->payload->length, rt2->payload->data);
                manage_response(rt2->type, rt2->payload);
                RealTalk_destroy(&rt2);
            }
        }
        close(main_data->server->sd);
        main_data->server->sd = -1;
        main_data->is_connected = false;

        sleep(main_data->reconnect_delay);
    }

    printf("[LISTENER THREAD] Closing the listener thread\n");
    pthread_exit(NULL);
}

static int command_line(GApplication *app, GApplicationCommandLine *command_line, gpointer user_data) {
    (void)user_data;
    int argc;
    char **argv = g_application_command_line_get_arguments(command_line, &argc);
    if (argc != 3) {
        fprintf(stderr, "usage: ./uchat [node] [service]\n");
        g_strfreev(argv);
        return 1;
    }

    main_data = MainData_new(GTK_APPLICATION(app), argv[1], argv[2]);
    g_strfreev(argv);
    
    g_application_activate(app);
    return 0; 
}

static void activate(GtkApplication* app, gpointer user_data) {
    (void)app;
    if (main_data) {
        pthread_t thread_id;
        if ((pthread_create(&thread_id, NULL, connection_handler, NULL)) != 0) {
            MainData_destroy(main_data);
            return;
        }
        main_data->thread_id = thread_id;
        import_css_theme("css/mango.css");
        create_login_window(user_data);
    }
}

int main(int argc, char **argv) {
    GtkApplication *mango_chat;
    int status;
    int pid = getpid();
    char app_id[24];
    snprintf(app_id, sizeof(app_id), "mango.chat%d", pid);

    mango_chat = gtk_application_new(app_id, 8);
    g_signal_connect(mango_chat, "activate", G_CALLBACK(activate), NULL);
    g_signal_connect(mango_chat, "command-line", G_CALLBACK(command_line), NULL);

    status = g_application_run(G_APPLICATION(mango_chat), argc, argv);

    if (main_data) {
        main_data->is_closing = true;
    }
    
    MainData_destroy(main_data);
    g_object_unref(mango_chat);
    return status;
}
