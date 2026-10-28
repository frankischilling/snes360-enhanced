# Snes9x 1.51 Xbox 360 sources

This directory contains the Snes9x core, the Xbox 360 front end, bundled
libraries, and UI resources used by snes360-enhanced.

See the [project README](../README.md) for features, controls, installation,
build instructions, validation, credits, and license information. The current
version is 0.38 Beta, and achievements are removed.

Open `win32/snes9xw.sln` for the Visual Studio 2008 Xbox 360 build, or run
`tools/Build-Release.ps1` from the repository root. Build `Release|Xbox 360`;
the solution builds TinyXML before linking the emulator.

The `doc` directory contains the original Snes9x manuals and release history.
Those files describe upstream Snes9x and other platforms as well as this
port's starting point. Use the root README for the Xbox 360 release procedure.
Keep the original copyright and license notices with any redistributed files.
