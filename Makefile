# Build the C keybinding helper.
#
#   make build          # -> bin/omarchy-menu-cycle
#   make install        # copy into the installed plugin folder as
#                       #   omarchy-menu-cycle-fast  (used by bindings)
#
# Keybindings (see README "Install"):
#   $HOME/.config/omarchy/plugins/sleep-fast/omarchy-menu-cycle-fast
#   $HOME/.config/omarchy/plugins/sleep-fast/omarchy-menu-cycle-fast release

PLUGIN_DIR := $(HOME)/.config/omarchy/plugins/sleep-fast
CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -std=c11
BIN := bin/omarchy-menu-cycle

.PHONY: build install clean

build: $(BIN)

$(BIN): omarchy-menu-cycle.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin:
	mkdir -p bin

install: build
	install -m 755 $(BIN) $(PLUGIN_DIR)/omarchy-menu-cycle-fast

clean:
	rm -rf bin
