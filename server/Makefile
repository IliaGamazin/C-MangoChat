CC := clang
CFLAGS := -std=c11 -Wall -Wextra -Werror -Wpedantic -D_GNU_SOURCE `pkg-config --cflags-only-I libcjson_utils libcjson sqlite3 openssl`
LDFLAGS := `pkg-config --libs-only-L libcjson_utils libcjson sqlite3 openssl`
LDLIBS := `pkg-config --libs-only-l libcjson_utils libcjson sqlite3 openssl` -lpthread

OBJ_DIR := obj
SRC_DIR := src
DB := mangodb.db
DEPLOY := deploy
DEPLOY_OBJS := $(OBJ_DIR)/deploy.o
SERVER := uchat_server
SERVER_OBJS := $(filter-out $(DEPLOY_OBJS),$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c)))

.PHONY: all
all: $(DB) $(DEPLOY) $(SERVER)

.PHONY: clean
clean:
	$(MAKE) -C ../utils clean
	$(RM) -r $(OBJ_DIR)

.PHONY: uninstall
uninstall: clean
	$(MAKE) -C ../utils uninstall
	$(RM) $(DB) $(DEPLOY) $(SERVER)

.PHONY: reinstall
reinstall: uninstall all

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DB): $(DEPLOY)
	./$(DEPLOY) $@

$(DEPLOY): $(DEPLOY_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $^

$(SERVER): $(SERVER_OBJS)
	$(MAKE) -C ../utils
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $^ ../utils/utils.a

