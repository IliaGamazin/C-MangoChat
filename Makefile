SERVER_DIR = server
CLIENT_DIR = client

.PHONY: all
all:
	$(MAKE) -C $(SERVER_DIR)
	$(MAKE) -C $(CLIENT_DIR)

.PHONY: clean
clean:
	$(MAKE) -C $(SERVER_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean

.PHONY: uninstall
uninstall:
	$(MAKE) -C $(SERVER_DIR) uninstall
	$(MAKE) -C $(CLIENT_DIR) uninstall

.PHONY: reinstall
reinstall:
	$(MAKE) -C $(SERVER_DIR) reinstall
	$(MAKE) -C $(CLIENT_DIR) reinstall

