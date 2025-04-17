CC := clang
CFLAGS := -std=c11 -Wall -Wextra -Werror -Wpedantic -D_GNU_SOURCE `pkg-config --cflags-only-I gtk+-3.0 libcjson sqlite3 openssl`
LDFLAGS := `pkg-config --libs-only-L gtk+-3.0 libcjson sqlite3 openssl`
LDLIBS := `pkg-config --libs-only-l gtk+-3.0 libcjson sqlite3 openssl` -lpthread

OBJ_DIR := obj
SRC_DIR := src
CLIENT := uchat

.PHONY: all
all: $(CLIENT)

.PHONY: clean
clean:
	$(MAKE) -C ../utils clean
	$(RM) -r $(OBJ_DIR)

.PHONY: uninstall
uninstall: clean
	$(MAKE) -C ../utils uninstall
	$(RM) $(CLIENT)

.PHONY: reinstall
reinstall: uninstall all

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(CLIENT): $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
	$(MAKE) -C ../utils
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $^ ../utils/utils.a

