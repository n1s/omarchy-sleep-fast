/*
 * omarchy-menu-cycle — keybinding helper for the sleep-fast plugin.
 *
 *   press    one IPC call: cycle the menu if visible, else open it
 *   release  one IPC call: arm the countdown
 *            (seconds from ~/.config/omarchy/shell.json -> idle.systemMenuCountdown,
 *             falls back to 3)
 *
 * Replaces the bash version: no bash/jq spawns, and the IPC call is fired
 * without waiting for the reply (delivery order is FIFO per socket, so
 * press/release sequences stay ordered). If omarchy-shell is missing we
 * exit silently, matching `omarchy-shell -q`.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_COUNTDOWN 3

/* Fork + exec omarchy-shell; parent returns at once, child exits silently
 * if the binary is missing (parity with `omarchy-shell -q`). */
static void run_ipc(char *const args[])
{
	pid_t pid = fork();
	if (pid != 0)
		return;
	execvp(args[0], args);
	_exit(0);
}

/* Read idle.systemMenuCountdown from shell.json. Minimal targeted scan for
 * the one key we own; anything missing or malformed yields the default. */
static int read_countdown(void)
{
	const char *xdg = getenv("XDG_CONFIG_HOME");
	const char *home = getenv("HOME");
	if (!home || !*home)
		return DEFAULT_COUNTDOWN;

	char path[512];
	snprintf(path, sizeof(path), "%s/omarchy/shell.json",
		 (xdg && *xdg) ? xdg : home);

	FILE *f = fopen(path, "r");
	if (!f)
		return DEFAULT_COUNTDOWN;

	char buf[65536];
	size_t n = fread(buf, 1, sizeof(buf) - 1, f);
	fclose(f);
	buf[n] = '\0';

	const char *key = strstr(buf, "\"systemMenuCountdown\"");
	if (!key)
		return DEFAULT_COUNTDOWN;

	/* The key token contains no colon, so search from its start. */
	const char *p = strchr(key, ':');
	if (!p)
		return DEFAULT_COUNTDOWN;

	while (*p == ':' || *p == ' ' || *p == '\t')
		p++;
	if (*p < '0' || *p > '9')
		return DEFAULT_COUNTDOWN;

	return (int)strtol(p, NULL, 10);
}

int main(int argc, char **argv)
{
	const char *mode = argc > 1 ? argv[1] : "press";

	if (strcmp(mode, "press") == 0) {
		char *const args[] = { "omarchy-shell", "-q", "shell", "call",
			"sleep-fast", "cycleOrOpen", "{\"menu\":\"system\"}", NULL };
		run_ipc(args);
	} else if (strcmp(mode, "release") == 0) {
		char cds[16];
		snprintf(cds, sizeof(cds), "%d", read_countdown());
		char *const args[] = { "omarchy-shell", "-q", "shell", "call",
			"sleep-fast", "armIfOpen", cds, NULL };
		run_ipc(args);
	} else {
		fprintf(stderr, "usage: omarchy-menu-cycle [press|release]\n");
		return 2;
	}
	return 0;
}
