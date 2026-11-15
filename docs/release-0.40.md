# Version 0.40 Beta

Pressing B or the Guide button on the Game Genie keyboard now launches the
selected ROM without the codes being entered. This addresses the skip failure
reported in [issue #10](https://github.com/frankischilling/snes360-enhanced/issues/10)
in both the ROM browser and Favorites. The launch code checks the Guide's
extended completion status, where keyboard cancellation is reported. Other
keyboard errors still stop the launch and display an error.

This release also repairs several faults in the front end and storage code:

- Missing drive attributes, empty preview settings and removed device entries
  no longer cause invalid string access or an invalid device lookup. Safe
  defaults keep the ROM browser usable. Malformed or unreadable settings files
  are left intact so custom entries can be recovered.
- Favorites updates use a temporary file and check the full write, flush,
  close and replacement. A failed update preserves the saved list. Read
  failures preserve the active list; oversized files and overlong entries
  cannot overrun filename buffers.
- Favorite-name comparisons and preview URLs no longer copy text into fixed
  buffers. Oversized preview filenames are rejected.
- Failed save-stream opens return no stream and unmount opened content.
  Callers keep ownership of their completion events, and failed reads and
  writes return zero bytes instead of an error code that could be mistaken
  for a byte count. Incomplete saved configuration data is ignored.
- SRAM loads use the byte count returned by the storage reader and keep a
  successfully loaded console save. Multi-cartridge saves use separate names.
  Save failures and incomplete writes are reported to the caller.
- S-RTC clock data uses the full 64-bit timestamp when saving and loading.
  Loading replaces the previous timestamp, and SRAM allocation includes room
  for the clock data even at the maximum save size.
- Failed ROM loads return to the browser with an error. Graphics setup returns
  failure correctly and releases shader compilation buffers.

Achievements remain removed.

Back up `settings.xml`, `favorites.txt`, saves and states before updating.
Copy `Snes360.xex` and the complete `media` folder together. Keep your settings
and personal data. The package includes the rebuilt skin, shaders, font and
UI resources, plus the required licenses. ROMs and personal saves are not
included.

Regression tests cover extended keyboard cancellation, invalid unfinished
codes, repeated completion polling, malformed settings, Favorites storage
failures, save-stream cleanup and S-RTC timestamp round trips. The frontend,
settings, storage, S-RTC and multitap component tests run with address and
undefined behavior sanitizers.
UI checks inspect scene navigation.

The Xbox 360 release build uses Visual Studio 2008 SP1 and XDK 2.0.9328.0,
followed by source, XEX, linker-map and skin-package checks. Console launch,
keyboard behavior and gameplay still need hardware testing. Issue #10 remains
open for confirmation from the reporter.
