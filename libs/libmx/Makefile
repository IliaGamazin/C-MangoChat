CC := clang
CFLAGS := -std=c11 -Wall -Wextra -Werror -Wpedantic
SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
RESOURCES := $(wildcard ./inc/*.h) $(SRC_FILES)
OBJ_DIR := obj
LIBMX := libmx.a

.PHONY: all
all: $(LIBMX)

$(LIBMX): $(RESOURCES)
	rm -rf $(OBJ_DIR)
	mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_FILES)
	mv ./*.o $(OBJ_DIR)
	ar -rc $(LIBMX) $(OBJ_DIR)/*.o

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)

.PHONY: uninstall
uninstall: clean
	rm -rf $(LIBMX)

.PHONY: reinstall
reinstall: uninstall all
