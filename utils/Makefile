CC := clang
CFLAGS := -std=c11 -Wall -Wextra -Werror -Wpedantic -D_GNU_SOURCE 

UTILS := utils.a
OBJ_DIR := obj
SRC_DIR := src

.PHONY: all
all: $(UTILS)

.PHONY: clean
clean:
	$(RM) -r $(OBJ_DIR)

.PHONY: uninstall
uninstall: clean
	$(RM) $(UTILS)

.PHONY: reinstall
reinstall: uninstall all

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(UTILS): $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
	$(AR) -rs $@ $?

