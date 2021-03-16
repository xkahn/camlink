CC=gcc
CFLAGS= -Wall -Wextra -Wwrite-strings -fPIC
LFLAGS= -Wall -Wextra -Wwrite-strings -shared
BUILD_DIR = build
LIBS = camlink
INSTALL_DIR = /usr/local/lib/camlink
SOURCES = $(wildcard *.c)

all: build

LIB_TARGETS = $(addprefix $(BUILD_DIR)/, $(addsuffix .so, $(LIBS)))
OBJECT_TARGETS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SOURCES))))

.PHONY: build
build: $(LIB_TARGETS) $(OBJECT_TARGETS)

$(BUILD_DIR)/%.so: $(BUILD_DIR)/%.o
	$(CC) $(LFLAGS) -o $@ $< -ldl

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

INSTALL_TARGETS = $(addprefix $(INSTALL_DIR)/, camlink.so)

.PHONY: install
install: $(INSTALL_TARGETS)

$(INSTALL_DIR)/%.so: $(BUILD_DIR)/%.so $(INSTALL_DIR)
	cp $< $@

$(INSTALL_DIR):
	mkdir -p $(INSTALL_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: uninstall
uninstall:
	rm -rf $(INSTALL_DIR)
