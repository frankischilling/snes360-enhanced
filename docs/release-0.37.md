# Version 0.37 Beta

Achievements are removed from the source, menu, and executable metadata.

This release deletes the old achievement routines, triggers, generated headers,
SPA data, and Xbox Live submission projects. Release and Debug image settings no
longer embed the SPA resource section. The main menu skips the removed entry
when moving between Favorites and Change Storage Device.

The build also removes calls to the deleted presence function, builds TinyXML
before the emulator, and links the library from the selected Release directory.
Release linking discards unused SDK functions. Skin packaging compiles scenes
from XUI sources and skips older copies of the same scene.
The menu and About dialog show version 0.37 Beta.

The release package contains a newly compiled `Snes360.xex`, a rebuilt
`media/Snes360.xzp`, shaders, fonts, and UI resources. Existing features include
ROM search, favorites, Game Genie codes, and the FPS display.

Back up `settings.xml`, `favorites.txt`, saved games, and states before updating.
Copy the executable and `media` folder together. Keep your existing settings
and favorites files.

Validation used Visual Studio 2008 SP1 and XDK 2.0.9328.0. A clean Release
rebuild completed with both projects successful, zero errors, and 10 compiler
warnings. The source audit checked 279 source, UI, and project files. Package
checks found no embedded resource table in the XEX, no achievement functions
in the linker map, and no old achievement strings in the skin archive.

Console launch and gameplay have not been tested on Xbox 360 hardware for this
release. See the repository README for build and installation instructions.
