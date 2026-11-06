#include "snes9x.h"
#include "memmap.h"
#include "ppu.h"
#include "controls.h"
#include "display.h"
#include "movie.h"
#include "snapshot.h"
#include "cpuexec.h"
#include "XboxJoypads.h"
#include <cassert>
#include <iostream>

SSettings Settings = {};
CMemory Memory;
SPPU PPU = {};
InternalPPU IPPU = {};
uint8 OpenBus = 0;
extern uint16 MovieGetJoypad(int);

// Host services are outside this component test. Joypad commands, polling,
// serial reads, automatic reads and frame updates use the production core.
bool8 S9xMoviePlaying() { return FALSE; }
bool8 S9xMovieActive() { return FALSE; }
void S9xMovieUpdate(bool) {}
void S9xMovieUpdateOnReset() { assert(false); }
void S9xMovieStop(bool8) { assert(false); }
int S9xMovieCreate(const char*, uint8, uint8, const wchar_t*, int) { assert(false); return 0; }
int S9xMovieOpen(const char*, bool8) { assert(false); return 0; }
uint32 S9xMovieGetFrameCounter() { return 0; }
const char *S9xChooseMovieFilename(bool8) { assert(false); return ""; }
void S9xMessage(int, int, const char*) { assert(false); }
void S9xSetInfoString(const char*) { assert(false); }
void S9xToggleSoundChannel(int) { assert(false); }
const char *S9xStringInput(const char*) { assert(false); return ""; }
const char *S9xChooseFilename(bool8) { assert(false); return ""; }
const char *S9xGetDirectory(s9x_getdirtype) { assert(false); return ""; }
void S9xExit() { assert(false); }
void S9xReset() { assert(false); }
void S9xSoftReset() { assert(false); }
bool8 S9xFreezeGame(const char*) { assert(false); return FALSE; }
bool8 S9xUnfreezeGame(const char*) { assert(false); return FALSE; }
bool8 S9xLoadSnapshot(const char*) { assert(false); return FALSE; }
bool8 S9xSPCDump(const char*) { assert(false); return FALSE; }
bool8 Snapshot(const char*) { assert(false); return FALSE; }
void _splitpath(const char*, char*, char*, char*, char*) { assert(false); }
void _makepath(char*, const char*, const char*, const char*, const char*) { assert(false); }
void Do7110Logging() { assert(false); }
bool S9xPollButton(uint32, bool*) { assert(false); return false; }
bool S9xPollAxis(uint32, int16*) { assert(false); return false; }
bool S9xPollPointer(uint32, int16*, int16*) { assert(false); return false; }
void S9xHandlePortCommand(s9xcommand_t, int16, int16) { assert(false); }
const char *S9xGetCrosshair(int) { assert(false); return ""; }
void S9xDrawCrosshair(const char*, uint8, uint8, int16, int16) { assert(false); }

static void report(int pad, uint16 mask)
{
    for (int bit = 4; bit < 16; ++bit)
        S9xReportButton(pad * 16 + bit, (mask & (1 << bit)) != 0);
}

static void latch()
{
    S9xSetJoypadLatch(true);
    assert((S9xReadJOYSERn(1) & 3) == 2);
    S9xSetJoypadLatch(false);
}

static void readPair(bool first, uint16 a, uint16 b)
{
    Memory.FillRAM[0x4213] = first ? 0x80 : 0;
    uint16 actualA = 0, actualB = 0;
    for (int bit = 0; bit < 16; ++bit)
    {
        uint8 value = S9xReadJOYSERn(0x4017);
        actualA = (actualA << 1) | (value & 1);
        actualB = (actualB << 1) | ((value >> 1) & 1);
    }
    assert(actualA == a && actualB == b);
}

int main()
{
    static uint8 ram[0x8000] = {};
    Memory.FillRAM = ram;
    Settings.UpAndDown = true;
    Settings.MultiPlayer5Master = true;
    S9xUnmapAllControls();
    SetXboxJoypadPorts(true);
    S9xControlsSoftReset();
    for (int pad = 0; pad < 4; ++pad)
        for (int bit = 4; bit < 16; ++bit)
        {
            s9xcommand_t command = {};
            command.type = S9xButtonJoypad;
            command.button.joypad.idx = pad;
            command.button.joypad.buttons = 1 << bit;
            assert(S9xMapButton(pad * 16 + bit, command, false));
        }

    controllers kind;
    int8 a, b, c, d;
    S9xGetController(1, &kind, &a, &b, &c, &d);
    assert(kind == CTL_MP5 && a == 1 && b == 2 && c == 3 && d == -1);

    uint32 random = 12345;
    for (int round = 0; round < 256; ++round)
    {
        uint16 masks[4];
        for (int pad = 0; pad < 4; ++pad)
        {
            random = random * 1664525u + 1013904223u;
            masks[pad] = (random >> 16) & 0xfff0;
            report(pad, masks[pad]);
            assert(MovieGetJoypad(pad) == masks[pad]);
        }
        latch();
        readPair(true, masks[1], masks[2]);
        readPair(false, masks[3], 0);
        // Only the three attached adapter pads return trailing ones.
        for (int i = 0; i < 600; ++i)
            assert((S9xReadJOYSERn(1) & 3) == 1);
        Memory.FillRAM[0x4213] = 0x80;
        assert((S9xReadJOYSERn(1) & 3) == 3);
        S9xDoAutoJoypad();
        assert(READ_WORD(ram + 0x4218) == masks[0]);
        assert(READ_WORD(ram + 0x421a) == masks[1]);
        assert(READ_WORD(ram + 0x421e) == masks[2]);
        Memory.FillRAM[0x4213] = 0;
        S9xDoAutoJoypad();
        assert(READ_WORD(ram + 0x421a) == masks[3]);
        assert(READ_WORD(ram + 0x421e) == 0);
        S9xControlEOF();
    }

    // A controller disconnect reports no buttons without moving other players.
    report(2, 0);
    report(3, 0x8000);
    latch();
    readPair(true, MovieGetJoypad(1), 0);
    readPair(false, 0x8000, 0);
    // Banks retain independent positions when a game switches pairs mid-read.
    latch();
    Memory.FillRAM[0x4213] = 0;
    assert((S9xReadJOYSERn(1) & 1) == 1);
    Memory.FillRAM[0x4213] = 0x80;
    S9xReadJOYSERn(1);
    Memory.FillRAM[0x4213] = 0;
    assert((S9xReadJOYSERn(1) & 1) == 0);

    // Loading a state can restore pad IDs from a different layout. Reapply
    // the selected ports, as the menu does after loading a state.
    SControlSnapshot state;
    S9xSetController(1, CTL_MP5, -1, -1, -1, -1);
    S9xControlPreSave(&state);
    SetXboxJoypadPorts(true);
    S9xControlPostLoad(&state);
    SetXboxJoypadPorts(true);
    latch();
    readPair(true, MovieGetJoypad(1), MovieGetJoypad(2));
    readPair(false, MovieGetJoypad(3), 0);

    SetXboxJoypadPorts(false);
    S9xControlsSoftReset();
    S9xGetController(1, &kind, &a, &b, &c, &d);
    assert(kind == CTL_JOYPAD && a == 1);
    S9xDoAutoJoypad();
    assert(READ_WORD(ram + 0x421a) == MovieGetJoypad(1));
    assert(READ_WORD(ram + 0x421e) == 0);
    S9xUnmapAllControls();
    std::cout << "Multitap core tests passed: four pads, both banks, detection, serial and auto reads, disconnect, reset and two-player mode.\n";
}
