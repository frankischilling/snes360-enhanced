# Version 0.38 Beta

This release addresses Favorites removal, Game Genie support in the ROM
browser, and navigation to the Display FPS checkbox.

- For [issue #2](https://github.com/frankischilling/snes360-enhanced/issues/2),
  Favorites now has a Remove favorite button and an LB shortcut. Removing a
  game updates the list and `favorites.txt`. Filename comparisons ignore case,
  and removing the last item returns focus to Main Menu.
- For [issue #4](https://github.com/frankischilling/snes360-enhanced/issues/4),
  the ROM browser and Favorites use the same Game Genie keyboard and launch
  flow. The selected ROM path is copied before the keyboard opens. You can
  enter up to three codes, cancel to launch without codes, or disable the
  prompt with `<GameGenieEnabled>false</GameGenieEnabled>` in `settings.xml`.
  Invalid input leaves you in the browser so you can try again.
- For [issue #5](https://github.com/frankischilling/snes360-enhanced/issues/5),
  the menu navigation now moves from Keep Aspect Ratio to Display FPS, then
  into the filter list. The checkbox starts with the saved on or off state.

The ROM browser also fixes a title-conversion overflow that could overwrite
memory when a filename or its `[Favorite]` label exceeded 41 characters.
Search and rescan updates rebuild list rows once and guard empty selections.
Keyboard requests are cancelled safely when their scene is destroyed.

Achievements remain removed.

The package includes a newly compiled `Snes360.xex`, rebuilt skin archive,
compiled shaders, font, and UI resources. Copy the executable and `media`
folder together. Back up settings, favorites, saves, and states before
updating, and keep your existing personal files.

Component tests cover the shared launch code, keyboard errors and cancellation,
selection from a 6,000-entry list, long titles, filtering, and favorite-file
persistence. The native tests use SDK substitutes with address and undefined
behavior sanitizers; they do not run the Xbox UI. Navigation checks inspect
the XUI scene sources. The release build uses Visual Studio 2008 SP1 and Xbox
360 XDK 2.0.9328.0, followed by source, XEX, linker-map, and skin-package audits.

Xbox 360 launch, controller navigation, FPS persistence, and gameplay still
need testing on hardware.
