# Sleep fast

A System menu for Omarchy that activates the highlighted entry after a
release countdown. Clone of the built-in `omarchy.menu` plugin with two
fixes and one feature:

- The stock menu mishandles shell IPC arguments (they arrive as strings,
  which breaks the selection math). This fork coerces them to numbers.
- The menu now answers visibility questions over IPC (`cycleOrOpen`,
  `armIfOpen`, `debugState`), so keybindings never have to guess.
- Release countdown: hold `SUPER + ESC` to cycle through the entries,
  release to arm the countdown, and the highlighted entry activates when
  it runs out. Press again to cancel the countdown and keep browsing.

## Install

    omarchy plugin add https://github.com/YOURNAME/omarchy-sleep-fast.git --enable

The plugin replaces nothing by itself. Finish setup with three steps.

1. Disable the stock menu:

       omarchy plugin disable omarchy.menu

2. Add the keybindings to `~/.config/hypr/bindings.lua`:

       hl.unbind("SUPER + ESCAPE")
       o.bind("SUPER + ESCAPE", "Sleep fast (cycle)",
         "$HOME/.config/omarchy/plugins/sleep-fast/omarchy-menu-cycle",
         { repeating = true })
       o.bind("SUPER + ESCAPE", "Sleep fast — arm countdown",
         "$HOME/.config/omarchy/plugins/sleep-fast/omarchy-menu-cycle release",
         { release = true })

3. Reload Hyprland (`hyprctl reload`) or log out and back in.

Requires `jq` (reads one config value; preinstalled on Omarchy).

## Usage

- `SUPER + ESC` opens the System menu.
- Tap again to move the highlight down. Hold to cycle fast.
- Release starts the countdown. A badge in the menu header shows the
  seconds left, then the highlighted entry activates.
- Press during the countdown to cancel it and keep browsing.
- `ESC` closes the menu. `Enter` activates the highlighted entry.

## Configure

Countdown seconds live in `~/.config/omarchy/shell.json`:

    {
      "idle": {
        "systemMenuCountdown": 3
      }
    }

Changes apply on the next release. Missing or invalid values fall back
to 3.

## Remove

    omarchy plugin remove sleep-fast

Then delete the keybindings from `~/.config/hypr/bindings.lua` and run
`omarchy plugin enable omarchy.menu` to restore the stock menu.

## Debugging

Menu state over IPC (the third argument is required even for
zero-argument functions):

    omarchy-shell shell call sleep-fast debugState x

Recovery:

- Menu opens with no entries: run `omarchy menu refresh` to re-read the
  menu JSONC files. (With the stock menu disabled, use
  `omarchy-shell shell call sleep-fast refresh x` instead.)
- Menu does not appear at all: restart the shell. A plugin hot-reload
  can leave the window dead while IPC keeps answering.
- After editing `Menu.qml`, changes may not apply: Quickshell caches
  compiled QML in `~/.cache/quickshell/qmlcache`. Remove that folder and
  restart the shell.
