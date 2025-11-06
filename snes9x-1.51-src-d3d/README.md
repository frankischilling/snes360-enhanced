# snes360-enhanced (Snes9x-360 tweaks)

Enhanced Xbox 360 port of the Snes9x 1.51 SNES emulator focused on Direct3D rendering and Xbox 360-specific optimizations. Core emulation code remains intact; improvements are limited to the Xbox UI layer (XUI scenes, Direct3D rendering, XAudio2 audio, input handling).

> **Note:** Based on the original Xbox 360 port (Snes360 V0.32 Beta, credited to "Anonymous" in source code). **ModernVintageGamer (Dimitris)** is known to have contributed to the original port, which was a collaborative effort with multiple developers. This enhanced version **removes all achievement functionality** to prevent Xbox Live bans, along with build fixes, C89 compatibility improvements, and **Many more features and improvements are planned for future releases.**

* **Toolchain:** Visual Studio 2008 SP1

* **SDK:** Xbox 360 XDK 2.0.9328.0

* **Target:** Xbox 360 (RGH/JTAG/Dev Kits), retail-runnable `.xex`

* **Original Port:** Anonymous (Snes360 V0.32 Beta, 07-16-2010) - Original Xbox 360 port of Snes9x
* **Known Contributors:** ModernVintageGamer (Dimitris) and others (see Credits section)

---

## Table of Contents

- [Features Showcase](#features-showcase)
- [What's New](#whats-new)
- [Repository Layout](#repository-layout)
- [Build](#build)
- [Deploy to Xbox 360](#deploy-to-xbox-360-rghjtagdev-kits)
- [Controls](#controls)
- [Technical Notes](#technical-notes)
- [Troubleshooting](#troubleshooting)
- [Credits](#credits)

---

## Features Showcase

### Direct3D Rendering
Hardware-accelerated SNES rendering using Direct3D 9, optimized for Xbox 360's GPU with custom HLSL shaders.

### XAudio2 Audio
Low-latency audio output using XAudio2, taking full advantage of Xbox 360's audio hardware.

### Xbox 360 Controller Support
Native Xbox 360 controller input with full button and trigger support.

### Custom Xbox 360 UI
XUI-based user interface with custom skin resources and Xbox 360 dashboard integration.

---

## What's New

### ⚠️ CRITICAL: Achievement System Completely Removed

* **All Xbox Live Achievement Functionality Removed:** Complete removal of achievement system to prevent Xbox Live bans
  * **Why This Matters:** Achievement systems in homebrew applications can trigger Xbox Live account bans and console bans
  * **Safety First:** This build is safe to use without risk of Xbox Live enforcement actions
  * **Complete Removal:**
    * All achievement header files excluded from build (`Achievements.spa.h`, `Snes 360.xlast`)
    * Achievement compilation step (SPA compiler) completely disabled
    * All achievement function calls removed (`DoAchievo()`, `EnumerateAchievements()`, `XShowAchievementsUI()`)
    * Achievement-related variables, structures, and declarations removed
    * Achievement UI button functionality disabled
    * All game-specific achievement triggers removed (Super Metroid, Super Mario Kart, etc.)
  * **Files Modified:** `Main.cpp`, `InGameOptions.cpp`, `memmap.cpp`, `wsnes9x.h`, `RomList.cpp`, `snes9xw.vcproj`
  * **Replacement:** Created `XboxContext.h` with minimal context/property definitions for Xbox Live presence only (zero achievement code)

> **Important:** If you're using a modded Xbox 360 console, using achievement systems in homebrew can result in permanent Xbox Live bans. This build eliminates that risk entirely.

### Build Fixes & Enhancements

* **CRC32 Standalone Implementation:** Fixed linker errors by implementing standalone CRC32 for Xbox 360 builds
  * **Problem:** Xbox 360 build was trying to link against zlib's `crc32()` function which wasn't available
  * **Solution:** Added `EMUCRC32_STANDALONE` preprocessor definition and standalone CRC32 implementation
  * **Impact:** Builds successfully without requiring zlib's crc32 symbol
  * **Technical:** `zLib/emucrc32.c` now provides `crc32()` wrapper for unzip code when `EMUCRC32_STANDALONE` is defined

* **C89 Compatibility:** Fixed C99 compatibility issues for Xbox 360 compiler
  * **Problem:** Xbox 360 compiler (based on older MSVC) doesn't support C99 features
  * **Solution:** Converted all variable declarations to C89 style (declared at top of function blocks)
  * **Impact:** Code compiles successfully with Xbox 360 toolchain
  * **Files Modified:** `zLib/emucrc32.c`, `jma/crc32.cpp`

* **Project Configuration:** Optimized build settings for Xbox 360
  * **Excluded Files:** `zLib/crc32.c` excluded from Xbox 360 builds to prevent symbol conflicts
  * **Preprocessor Definitions:** Added `EMUCRC32_STANDALONE` to all Xbox 360 configurations
  * **Build Configurations:** Release, Debug, and C core configurations all properly configured

---

## Repository Layout

```
snes360-enhanced/
├── win32/                    # Windows/Xbox 360 project files
│   ├── snes9xw.vcproj        # Visual Studio 2008 project file
│   └── Xbox 360/             # Xbox 360 build outputs
│       ├── Release/           # Release build output
│       └── Debug/             # Debug build output
├── zLib/                     # zlib source code
│   ├── emucrc32.c            # CRC32 wrapper (standalone mode for Xbox 360)
│   └── emucrc32.h            # CRC32 wrapper header
├── jma/                      # JMA archive support
│   ├── crc32.cpp             # JMA CRC32 implementation
│   └── [other JMA files]
├── libPNG/                   # PNG library source
├── unzip/                    # ZIP extraction support
├── xbox/                     # Xbox 360-specific resources
│   ├── Shaders/              # HLSL shader files for Direct3D
│   └── Skin/                 # XUI skin resources
└── [Snes9x core files]       # Core emulation code (untouched)
```

---

## Build

### Prerequisites

1. **Microsoft Visual Studio 2008 SP1**
   - Must be Service Pack 1 or later
   - Provides the C/C++ compiler and build tools needed for Xbox 360 development
   - Download from Microsoft (legacy downloads)

2. **Xbox 360 XDK (Xbox Development Kit) 9328**
   - Version 9328 is required for compatibility
   - Provides Xbox 360-specific libraries, headers, and tools
   - Includes the Xbox 360 compiler toolchain (VCCLX360CompilerTool)
   - Required for building Xbox 360 executables

3. **Dependencies (Included in Repository)**
   - **zlib** - Compression library (included in `zLib/` directory)
   - **libPNG** - PNG image support library (included in `libPNG/` directory)
   - Both libraries are included in the repository and do not need to be downloaded separately

### Build Steps

1. **Open the Project**
   - Launch Visual Studio 2008
   - Open `win32/snes9xw.vcproj`

2. **Select Configuration**
   - Choose **Release | Xbox 360** from the configuration dropdown
   - Alternative configurations:
     - `Debug | Xbox 360` - For debugging (slower, includes debug symbols)
     - `C core | Xbox 360` - Alternative core configuration

3. **Build the Project**
   - Select **Build → Rebuild Solution** (or press `Ctrl+Shift+B`)
   - The build process will:
     - Compile all C/C++ source files
     - Link against Xbox 360 libraries
     - Generate `snes9x.exe` (actually a `.xex` file) in `win32\Xbox 360\Release\`

4. **Output Location**
   - Release build: `win32\Xbox 360\Release\snes9x.exe`
   - Debug build: `win32\Xbox 360\Debug\snes9x.exe`

### Build Configuration Notes

The Xbox 360 build uses the following key preprocessor definitions:
- `_XBOX` - Identifies Xbox 360 platform
- `EMUCRC32_STANDALONE` - Uses standalone CRC32 implementation (no zlib dependency)
- `USE_DIRECTX3D` - Direct3D rendering
- `JMA_SUPPORT` - JMA archive support
- `HAVE_LIBPNG` - PNG screenshot support

### Build Notes

* Post-build may warn about Xbox 360 connection - this is expected if Neighborhood isn't configured. The `.xex` still builds successfully.

* Typical era/toolchain warnings are harmless (e.g., `/GR-` RTTI notes, `FASTCALL` macro noise).

---

## Deploy to Xbox 360 (RGH/JTAG/Dev Kits)

### Requirements
- Xbox 360 console capable of running `.xex` files (RGH, JTAG, Dev Kit, etc.)
- FTP server running on the console (e.g., via XeXMenu, Aurora, or FreeStyle Dash)
- Network connection to the console

### Deployment Steps

1. **Transfer the Executable**
   - Copy `snes9x.exe` (from `win32\Xbox 360\Release\`) to your Xbox 360 via FTP
   - Recommended location: `Hdd1:\Emulators\Snes360\` or similar

2. **Transfer Required Files**
   - Copy ROM files to an accessible location (e.g., `Hdd1:\Emulators\Snes360\roms\`)
   - Ensure any required configuration files are present
   - Copy Xbox 360 resources if needed:
     - `xbox/Shaders/` - HLSL shader files for Direct3D rendering
     - `xbox/Skin/` - XUI skin resources

3. **Launch the Application**
   - Use a dashboard (XeXMenu, Aurora, FreeStyle Dash, etc.) to launch `snes9x.exe`
   - Or create a shortcut in your dashboard's menu

### Running on Modded Consoles

The compiled `.xex` executable can run on any Xbox 360 console that supports unsigned code execution. This includes:

- **RGH consoles** (RGH1, RGH2, RGH3) - Hardware modification allowing unsigned code
- **JTAG consoles** - Early hardware modification method
- **Dev Kits** - Official Microsoft development hardware
- **Other modded consoles** - Any Xbox 360 with the ability to run `.xex` files

**Note:** Modifying Xbox 360 hardware may void warranties and violate terms of service. Use at your own risk. This software is intended for educational and homebrew development purposes.

---

## Controls

### Front-End (ROM Browser)
* **D-pad / Left Stick:** Navigate ROM list
* **A:** Load selected game & start emulation
* **B:** Back / Return to previous screen
* **Next Device Button:** Switch between storage devices (USB, HDD, etc.)

### In-Game
* **D-pad / Left Stick:** SNES D-pad
* **A, B, X, Y:** SNES buttons
* **LB, RB:** SNES shoulder buttons
* **START, BACK:** SNES Start/Select
* **LEFT_THUMB + LT:** Screenshot (if implemented)
* **START + BACK:** OSD/Menu (if implemented)

*Note: Control mappings may vary based on the specific port implementation.*

---

## Technical Notes

### Dependencies

**zlib and libPNG are included in the repository:**
- **zlib** - Located in `zLib/` directory, used for compression/decompression
- **libPNG** - Located in `libPNG/` directory, used for PNG screenshot support
- Both libraries are compiled as part of the build process
- No additional downloads or installation required

### CRC32 Implementation

The Xbox 360 build uses a standalone CRC32 implementation to avoid linking against zlib's `crc32()` function. This is handled by:

- `zLib/emucrc32.c` - Provides `crc32()` wrapper function for unzip code
- `EMUCRC32_STANDALONE` preprocessor definition enables standalone mode
- `zLib/crc32.c` is excluded from Xbox 360 builds to prevent symbol conflicts

**Technical Details:**
- When `EMUCRC32_STANDALONE` is defined, `emucrc32.c` checks for it first (before zlib)
- Provides both `emu_crc32()` and `calc_crc32()` functions
- Also provides `crc32()` wrapper matching zlib's signature for unzip code compatibility
- Includes zlib.h for type definitions (`uLong`, `Bytef`, `uInt`) but uses standalone implementation

---

## Troubleshooting

### Runtime Issues

**Application won't launch on modded console**
- Verify the console can run other `.xex` files (test with a known working homebrew)
- Check that the executable is not corrupted
- Ensure all required dependencies are present
- Verify the console's mod is functioning correctly

**Graphics issues**
- Verify Direct3D shaders are present in `xbox/Shaders/` directory
- Check Xbox 360 video settings
- Ensure Direct3D initialization is successful

**Audio issues**
- Verify XAudio2 is properly initialized
- Check Xbox 360 audio settings
- Ensure audio buffers are properly configured

---

## Credits

### Original Xbox 360 Port
- **Anonymous** - Original Xbox 360 port developer (chose to remain anonymous)
  - Initial port to Xbox 360 platform (Snes360 V0.32 Beta, 07-16-2010)
  - Direct3D rendering implementation
  - XAudio2 audio integration
  - Xbox 360 controller support
  - XUI integration
- **ModernVintageGamer (Dimitris)** - Known contributor to the Xbox 360 port
  - Worked on the original Xbox 360 port alongside other developers
  - Note: The original port was a collaborative effort with multiple contributors

### Original Port Contributors & Credits
The original Xbox 360 port credits acknowledge the following contributors:
- r0wdy, Arak0n, kl0wn, idc, direw0lf, PeteNub, MomDad, Odb718, Angerwound, Redline99, TJ_CRS, Xenon7, Xantium, _skitzo_

*Note: Many original Xbox 360 homebrew developers from that era chose to remain anonymous for various reasons. The original port was credited to "Anonymous" in the source code.*

### Enhanced Version
- **frankischilling** - Current maintainer
  - Achievements removed for safety (prevents Xbox Live bans)
  - Build fixes and compatibility improvements
  - Documentation improvements
  - Default settings.xml creation
  - UI improvements

### Core Emulator
- **Snes9x Team** - Original Snes9x emulator
  - Core SNES emulation
  - Game compatibility
  - Feature development
  - See `copyright.h` and source file headers for complete contributor list

### Libraries & Tools
- **zlib** - Compression library
- **libPNG** - PNG image support
- **JMA** - JMA archive format support
- **Microsoft** - Visual Studio 2008, Xbox 360 XDK

---

## License

This project is based on Snes9x, which is licensed under various open-source licenses. See the `doc/` directory for license information, including:
- `doc/snes9x-license.txt` - Main Snes9x license information
- `doc/GPL-2` - GNU General Public License v2
- `doc/LGPL-2.1` - GNU Lesser General Public License v2.1

---

## Disclaimer

This software is for educational and homebrew development purposes. Use of this software on modified Xbox 360 consoles may violate Microsoft's terms of service. Use at your own risk.

---

**Last Updated:** 2025
**Build System:** Visual Studio 2008 SP1 + Xbox 360 XDK 9328  
**Target Platform:** Xbox 360 (.xex compatible consoles - RGH, JTAG, Dev Kits, etc.)  
**Original Port:** ModernVintageGamer (Dimitris)