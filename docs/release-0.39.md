# Version 0.39 Beta

This release adds the three- and four-player multitap option requested in
[issue #8](https://github.com/frankischilling/snes360-enhanced/issues/8).

Start a game, press both thumbsticks to open the in-game menu, and enable
**Multitap (3 / 4 players)**. From Display FPS, press right to reach it. The
choice is saved for that ROM in `settings.xml`. If the game only checks for
controllers at startup, choose **Reset Game** after changing the option.
Reset Game restarts play, so save your progress first.

Xbox controller slots 1 through 4 map to SNES players 1 through 4. Player one
uses the first SNES port; the adapter on the second port carries players two,
three and four. Its unused fifth-player socket is disconnected. Turn the
option off for the usual two-controller setup. Each game keeps its own choice,
and ROMs with controller metadata keep that default until you save a choice.

The controller core also fixes two adapter loops that could read beyond their
four-entry array. Adapter serial counters stop at their maximum instead of
wrapping back to button data, and reset applies the selected port layout.
The existing Reset Game button now restarts the game. Loading a state
reapplies the selected controller layout.

Settings updates preserve unrelated XML entries and use a temporary file
before replacing `settings.xml`. A failed save leaves the active mode and
saved settings unchanged and displays an error.

Achievements remain removed.

Copy `Snes360.xex` and the complete `media` folder together. Back up settings,
favorites, saves and states before updating. The package includes the compiled
XEX, rebuilt skin, shaders, font and UI resources. ROMs and personal saves are
not included.

Component tests exercise the production controller core with four independent
button streams, both adapter banks, device detection, serial and automatic
reads, long serial reads, disconnects, state loading, reset and two-player mode. Settings
tests cover per-game isolation, case-insensitive paths, XML escaping, preserved
settings, partial writes, replacement failures and temporary-file conflicts.
These tests run with address and undefined behavior sanitizers. UI checks
inspect navigation in the scene source.

The release build uses Visual Studio 2008 SP1 and Xbox 360 XDK 2.0.9328.0,
followed by source, XEX, linker-map and skin-package audits. Console gameplay
with three or four controllers still needs hardware testing. A successful
build and component tests do not establish game-specific compatibility.
