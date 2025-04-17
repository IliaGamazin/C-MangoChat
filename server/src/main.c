#include "../inc/main.h"

static int intr = 0;
static Client *clients = NULL;

static void daemon_end(int sig) {
    (void) sig;
    intr = 1;
    Client *client = clients;
    while (client != NULL) {
        pthread_cancel(client->tid);
        client = client->next;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./server [service]\n");
        return -1;
    }

    const char *service = argv[1];

    char *epath = realpath(*argv, NULL);
    char *edir = strndup(epath, strrchr(epath, '/') - epath);

    char *dbpath;
    asprintf(&dbpath, "%s/%s", edir, "mangodb.db");

    char *vaultpath;
    asprintf(&vaultpath, "%s/%s", edir, "vault");

    // Start daemon
    if (daemon(0, 1) == -1) {
        fprintf(stderr, "daemon: %s\n", strerror(errno));
        return -1;
    }

    printf("Server started with PID %d\n", getpid());
    openlog("mangodaemon", LOG_PID, LOG_LOCAL0);
    syslog(LOG_INFO, "daemon started");

    // Set signals
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = daemon_end;
    act.sa_flags = 0;

    if (sigaction(SIGTERM, &act, NULL) == -1) {
        syslog(LOG_ERR, "sigaction: %s", strerror(errno));
        goto out_cleanup;
    }

    // Init server
    Server server;
    if (Server_init(&server, NULL, service)) {
        syslog(LOG_ERR, "Server_init: failed to initialize server");
        goto out_server_cleanup;
    }

    // Bind
    if (Server_bind(&server)) {
        syslog(LOG_ERR, "Server_bind: %s", strerror(errno));
        goto out_server_cleanup;
    }

    // Listen
    if (Server_listen(&server, 10)) {
        syslog(LOG_ERR, "Server_listen: %s", strerror(errno));
        goto out_server_cleanup;
    }

    // Log
    syslog(LOG_INFO, "server node is listening on port %s", service);

    // Main loop
    while (!intr) {
        int cd = Server_accept(&server, NULL, NULL);
        if (cd == -1) {
            syslog(LOG_ERR, "Server_accept: %s", strerror(errno));
            continue;
        }

        DataBase *database = DataBase_new();
        DataBase_init(database, dbpath, vaultpath);

        Client *client = Client_push_back(&clients, cd);

        Handler *handler = Handler_new(database, clients, client);
        if (handler == NULL) {
            syslog(LOG_ERR, "Handler_new: failed to create handler");
            Client_destroy(&client);
            DataBase_destroy(&database);
            continue;
        }

        int pthread_errno = pthread_create(&handler->client->tid, NULL, Handler_routine, handler);
        if (pthread_errno == -1) {
            Handler_destroy(&handler);
            syslog(LOG_ERR, "pthread_create: %s", strerror(pthread_errno));
        }
    }

out_server_cleanup:
    Server_free(&server);
out_cleanup:
    syslog(LOG_INFO, "daemon ended");
    closelog();
    free(dbpath);
    free(edir);
    free(epath);
    pthread_exit(NULL);
}
