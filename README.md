# Sleep fast

Put your machine to sleep fast. One or two key presses: hold `SUPER + ESC`
to cycle through power options — screensaver, lock, suspend, hibernate,
logout, reboot, shutdown — release, and the highlighted action runs on its
own after a short countdown. No need to confirm or reach for the Enter key.

## Install

    omarchy plugin add https://github.com/YOURNAME/omarchy-sleep-fast.git --enable

Then finish setup:

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

## Usage

- `SUPER + ESC` opens the menu.
- Tap again to move down the list. Hold to cycle fast.
- Release and wait: the highlighted action runs by itself when the
  countdown ends. A badge in the header counts down 3…2…1.
- Press during the countdown to cancel it and keep browsing.
- `ESC` closes the menu. `Enter` activates immediately.

## Configure

Countdown seconds live in `~/.config/omarchy/shell.json`:

    {
      "idle": {
        "systemMenuCountdown": 3
      }
    }

Set it to 1 for the impatient, 5 for the cautious. Changes apply on the
next release.

## Remove

    omarchy plugin remove sleep-fast

Then delete the keybindings from `~/.config/hypr/bindings.lua` and run
`omarchy plugin enable omarchy.menu` to restore the stock menu.
