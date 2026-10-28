# snes360-enhanced

Snes360-enhanced is an Xbox 360 port of the Snes9x 1.51 SNES emulator. It uses
Direct3D 9 and HLSL shaders for video, XAudio2 for audio, Xbox 360 controllers,
and XUI scenes for the interface. The changes focus on the Xbox 360 front end
and build support. The SNES emulation core comes from Snes9x.

Current version: 0.37 Beta.

Achievements are removed. The source has no achievement handlers, triggers,
generated headers, or SPA data. The XEX image settings do not embed a SPA
resource section, and the menu has no achievement entry.

![ROM Browser](/img/rombrowser.jpg)

![Favorites](/img/favorites.jpg)

![About](/img/about.jpg)

These screenshots show earlier versions of the interface.

## Features

- Browse ROMs and switch between USB, HDD, and other configured devices.
- Press Y in the ROM browser to search by name. Search ignores case, and the
  filter stays active when switching devices or refreshing the list. Submit an
  empty search to show all ROMs.
- Press LB in the ROM browser to add or remove a favorite, or use the Add to
  Favorites button. Favorites have a `[Favorite]` tag in the alphabetically
  sorted ROM list and appear on a separate Favorites page. They are stored in
  `favorites.txt` beside the executable.
- Enter up to three Game Genie codes when launching a game from Favorites.
  Separate codes with commas, spaces, or a mixture of both. For example:
  `C222-D4DD,8B99-17DD,DD8C-C7A7`. Codes use the `XXXX-XXXX` format. Invalid codes
  show an error and prevent launch; valid codes are applied after ROM loading.
  Cancel the keyboard with B to launch without codes. Launching from the
  regular ROM list skips this keyboard.
- Enable the Game Genie keyboard with `<GameGenieEnabled>true</GameGenieEnabled>`
  in `settings.xml`. Set it to `false` to launch from Favorites immediately.
  Newly created settings enable it; an existing file without the setting
  leaves it disabled.
- Open the in-game menu by pressing both thumbsticks. It includes save and load
  states, video filters, aspect ratio, point filtering, audio muting, screen
  adjustment, preview capture, and exit controls.
- Toggle Display FPS in the in-game menu. The top-right counter shows current
  and target FPS as `XX/YY`; the preference is saved between sessions.

The ROM-search and Game Genie keyboards use asynchronous requests checked each
frame so the interface can keep updating while the keyboard is open.

## Changes by version

- 0.37 Beta removes the remaining achievement code, metadata, and menu entry.
  It also removes calls to the deleted presence function, fixes the TinyXML
  build dependency and library path, and updates the displayed version.
- 0.36 Beta adds Game Genie input and validation to Favorites using Snes9x's
  `S9xGameGenieToRaw()`, `S9xAddCheat()`, and `S9xApplyCheats()` functions.
- 0.35 Beta adds ROM search with the Y button.
- 0.34 Beta adds the FPS toggle and Direct3D FPS display support.
- 0.33 Beta adds favorites and a skin-packaging script for custom resources.

Earlier build fixes added a standalone CRC32 implementation, C89-compatible
variable declarations, and Xbox 360 project definitions. More details for the
current release are in [the release notes](docs/release-0.37.md).

## Install or update

Use an Xbox 360 that can run homebrew XEX files, such as an RGH or JTAG console
or a development kit. Copy the contents of the release package to a folder
such as `Hdd1:\Emulators\Snes360\` using FTP or your usual file manager.

The executable and its resources must stay together:

```text
Snes360.xex
media/
  Snes360.xzp
  Snes360.ttf
  XuiEffect_Texture.png
  saveicon.png
  Shaders/
    ShadeXuiEffectVertex.xvu
    ShadeXuiEffectPixel.xpu
  sounds/
    background.mp3
saveicon.png
Roms/
previews/
```

Launch `Snes360.xex` from your dashboard, for example XeXMenu, Aurora, or
FreeStyle Dash. Add your ROMs to `Roms` or configure their paths in
`settings.xml`. The application creates default settings if the file is absent.
Background music is optional when building your own package.

Before updating, back up `settings.xml`, `favorites.txt`, saves, and states.
Replace the executable and `media` folder together and keep your existing
settings and favorites. ROMs and personal data are not included in releases.

## Controls

| Screen | Control | Action |
| --- | --- | --- |
| ROM browser | D-pad or left stick | Move through the list |
| ROM browser | A | Load the selected ROM |
| ROM browser | B | Go back |
| ROM browser | Y | Search ROM names |
| ROM browser | LB | Toggle the selected ROM's favorite status |
| Main menu | Favorites | Open the favorites list |
| ROM browser | Next Device | Switch storage devices |
| Favorites | A | Launch, with the Game Genie keyboard if enabled |
| Game Genie keyboard | B | Cancel and launch without codes |
| In game | D-pad or left stick | SNES D-pad |
| In game | A, B, X, Y | SNES face buttons |
| In game | LB, RB | SNES shoulder buttons |
| In game | Start, Back | SNES Start and Select |
| In game | Press both thumbsticks | Open the in-game menu |

Other mappings can depend on the port configuration. Use the in-game menu for
preview capture and display options.

## Build

The Xbox 360 project uses Visual Studio 2008 SP1 and Xbox 360 XDK 2.0.9328.0.
Install the XDK's Visual Studio integration and set `XEDK` to its installation
directory. The repository has also previously been built with SDK 21256.3;
newer Visual Studio/XDK combinations may need project conversion. Version 9328
is the baseline for these instructions.

The repository includes zlib, libPNG, JMA, and TinyXML sources. No separate
FMOD or desktop DirectX installation is needed for the Xbox 360 Release build.

From PowerShell at the repository root:

```powershell
.\tools\Build-Release.ps1
```

The script rebuilds `Release|Xbox 360`, creates a new directory under `dist`,
packages the XUI skin from source, compiles the two UI shaders, and copies the
font and texture resources. To include background music, pass a local MP3:

```powershell
.\tools\Build-Release.ps1 -BackgroundMusic C:\path\to\background.mp3
```

Use `-Devenv` to select another `devenv.com` path and `-OutputDirectory` to
choose a new output directory. The script refuses to overwrite an existing
package directory. Build logs are stored beside the package. Review the files
and run the audit before making a ZIP:

```powershell
python tools\audit-release.py --package dist\build-YYYYMMDD-HHMMSS\snes360-enhanced
```

For an IDE build, open
`snes9x-1.51-src-d3d\win32\snes9xw.sln`, select `Release|Xbox 360`, and rebuild.
The solution builds TinyXML before the emulator. The XEX output is
`snes9x-1.51-src-d3d\win32\Xbox 360\Release\Snes360.xex`. The intermediate
`win32\snes9x.exe` is a PE image used to create the XEX, not the deployment file.
`Debug|Xbox 360` is also present for development; the release package uses
Release. The legacy `C core` configuration is not the release build.

The standalone root `snes9xw.vcproj` is a legacy project copy. Use the solution
under `snes9x-1.51-src-d3d\win32` for these instructions.

## Skin and build details

XUI scene sources are in `snes9x-1.51-src-d3d/xbox/Skin`:

- `Mario.xui` defines the main menu.
- `RomList.xui` defines the ROM browser, including device switching, favorites,
  previews, and search.
- `FavoritesListScene.xui` defines the Favorites page.
- `InGameOptions.xui` defines the in-game menu.

The build script uses the XDK's `xuipkg` to convert XUI files and package
`Snes360.xzp`. The archive keeps paths beginning with `..\Xbox\Skin\`, as used
by the application's scene-loading calls. Copy the resulting XZP to `media`
beside the executable. Test custom skins on the console after changing them.
XZP Tool 2.0 can inspect or extract an existing archive.

To rebuild only the skin, run `tools/Build_XZP.bat` from the repository root.
It writes `Snes360.xzp` in the root and refuses to overwrite an existing file.
For another output path, run `tools/Build-Skin.ps1 -OutputFile C:\path\to\Snes360.xzp`.
Both helpers compile scenes from their XUI sources and skip older XUR copies
when a source exists. No extracted skin tree or duplicate entries are needed.

The Xbox 360 build defines `_XBOX`, `EMUCRC32_STANDALONE`, `USE_DIRECTX3D`,
`JMA_SUPPORT`, and `HAVE_LIBPNG`. `zLib/emucrc32.c` supplies `emu_crc32()`,
`calc_crc32()`, and a `crc32()` wrapper for ZIP support. It uses zlib's types
with a standalone implementation; `zLib/crc32.c` is excluded from Xbox builds
to avoid duplicate symbols. `jma/crc32.cpp` also has the compiler compatibility
changes needed by this toolchain.

## Validation

Run `python tools/audit-release.py` to scan tracked source, project settings,
and UI files. With `--package`, the audit also checks the XEX resource table,
the required package files, and the XZP for old achievement strings.
Pass `--map "snes9x-1.51-src-d3d/win32/Xbox 360/Release/Snes360.map"` to check
that the linker discarded achievement functions from the SDK as well.

The 0.37 release is compiled with Visual Studio 2008 SP1 and XDK 2.0.9328.0.
Build and package inspection do not establish console runtime behavior.
Console launch, menu navigation, and gameplay still need hardware testing.

## Troubleshooting

If the application will not launch, check that the console runs other homebrew
XEX files, verify the download checksum, and copy the complete `media` folder.
For a blank or broken interface, check `media/Snes360.xzp`, the font, UI texture,
and compiled shaders. For audio problems, check the mute setting and console
audio configuration.

The old toolchain emits warnings, including disabled RTTI and macro
redefinitions. Review warnings and errors in the build log; a generated file
alone does not establish a successful build. Builds should not attempt console
deployment when no development console is configured.

## Repository layout

| Path | Contents |
| --- | --- |
| `snes9x-1.51-src-d3d/` | Snes9x core and port sources |
| `snes9x-1.51-src-d3d/win32/` | Xbox 360 front end, solution, and projects |
| `snes9x-1.51-src-d3d/xbox/` | Skin sources, shaders, and UI assets |
| `snes9x-1.51-src-d3d/TinyXML/` | XML parser sources |
| `snes9x-1.51-src-d3d/zLib/` | Compression and CRC32 code |
| `snes9x-1.51-src-d3d/libPNG/` | PNG support |
| `snes9x-1.51-src-d3d/jma/` | JMA archive support |
| `snes9x-1.51-src-d3d/unzip/` | ZIP extraction |
| `snes9x-1.51-src-d3d/doc/` | Upstream manuals, history, and licenses |
| `tools/` | Build and audit scripts |
| `docs/` | Release notes |
| `dist/` | Local build packages, excluded from Git |

## Credits

The original Xbox 360 port is Snes360 V0.32 Beta, dated July 16, 2010 and
credited to Anonymous. It introduced Direct3D rendering, XAudio2 audio,
controller support, and XUI integration. ModernVintageGamer (Dimitris) worked
on the original port with other developers and provided its source code.

The original port also credits r0wdy, Arak0n, kl0wn, idc, direw0lf, PeteNub,
MomDad, Odb718, Angerwound, Redline99, TJ_CRS, Xenon7, Xantium, and _skitzo_.

frankischilling maintains this enhanced version, including achievement removal,
build and compatibility fixes, default settings creation, UI updates,
favorites, ROM search, FPS display, and Game Genie support.

The Snes9x team developed the emulator core. See
`snes9x-1.51-src-d3d/copyright.h` and source headers for the full contributor
list. The project uses zlib, libPNG, and JMA, and builds with Microsoft's Visual
Studio and Xbox 360 XDK tools.

## License

See [LICENSE](LICENSE) and the original license files in
`snes9x-1.51-src-d3d/doc`, including `snes9x-license.txt`, `GPL-2`, and
`LGPL-2.1`. Bundled libraries retain their own notices. The upstream manuals,
release history, and license texts are preserved as supplied.

This software is intended for educational and homebrew development. Use it at
your own risk.
