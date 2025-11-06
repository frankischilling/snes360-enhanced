/**********************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com),
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005  Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005  Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006  John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006  funkyass (funkyass@spam.shaw.ca),
                             Kris Bleakley (codeviolation@hotmail.com)

  (c) Copyright 2002 - 2007  Brad Jorsch (anomie@users.sourceforge.net),
                             Nach (n-a-c-h@users.sourceforge.net),
                             zones (kasumitokoduck@yahoo.com)

  (c) Copyright 2006 - 2007  nitsuja


  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com)
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti


  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley,
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001-2006    byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight,

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound DSP emulator code is derived from SNEeSe and OpenSPC:
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x, HQ3x, HQ4x filters
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  Win32 GUI code
  (c) Copyright 2003 - 2006  blip,
                             funkyass,
                             Matthew Kendora,
                             Nach,
                             nitsuja

  Mac OS GUI code
  (c) Copyright 1998 - 2001  John Stiles
  (c) Copyright 2001 - 2007  zones


  Specific ports contains the works of other authors. See headers in
  individual files.


  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without
  fee, providing that this license information and copyright notice appear
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
**********************************************************************************/

#pragma comment(linker, \
    "\"/manifestdependency:type='Win32' "\
    "name='Microsoft.Windows.Common-Controls' "\
    "version='6.0.0.0' "\
    "processorArchitecture='*' "\
    "publicKeyToken='6595b64144ccf1df' "\
    "language='*'\"")


//  Win32 GUI code
//  (c) Copyright 2003-2006 blip, Nach, Matthew Kendora, funkyass and nitsuja

#ifdef __MINGW32__
#define _WIN32_IE 0x0501
#define _WIN32_WINNT 0x0501
#endif


#include <xtl.h>
#include <xui.h>
#include <process.h>

#include "Main.h"
#include "InGameOptions.h"

#include "wsnes9x.h"
#include "../snes9x.h"
#include "../memmap.h"
#include "../cpuexec.h"
#include "../display.h"
#include "../3d.h"
#include "../cheats.h"
#include "../netplay.h"
#include "../apu.h"
#include "../movie.h"
#include "../controls.h"
#include "../conffile.h"
#include "../soundux.h"
#include "AVIOutput.h"
#include "InputCustom.h"
#include <vector>

#include "direct3d.h"

#include <tchar.h>

#if (((defined(_MSC_VER) && _MSC_VER >= 1300)) || defined(__MINGW32__))
	// both MINGW and VS.NET use fstream instead of fstream.h which is deprecated
	#include <fstream>
	using namespace std;
#else
	// for VC++ 6
	#include <fstream.h>
#endif

#include <sys/stat.h>
//#include "string_cache.h"
#include "wlanguage.h"
#include "../language.h"

//uncomment to find memory leaks, with a line in WinMain
//#define CHECK_MEMORY_LEAKS

#ifdef CHECK_MEMORY_LEAKS
	#include <crtdbg.h>
#endif

#include <io.h>
#include <time.h>
#include <direct.h>

extern SNPServer NPServer;
//HANDLE InputThread;
//HANDLE eventHandle; 

#ifdef USE_OPENGL
OpenGLData OpenGL;
#endif

#include <ctype.h>

#ifdef _MSC_VER
#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4
#endif

__int64 PCBase, PCFrameTime, PCFrameTimeNTSC, PCFrameTimePAL, PCStart, PCEnd;
DWORD PCStartTicks, PCEndTicks;

#ifdef RTC_DEBUGGER
int CALLBACK SPC7110rtc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
int CALLBACK DlgSP7PackConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgSoundConf(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgInfoProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgAboutProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgEmulatorProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

int CALLBACK DlgOpenROMProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgMultiROMProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgChildSplitProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgNPProgress(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgPackConfigProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgNetConnect(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgNPOptions(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgFunky(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgInputConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgHotkeyConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCheater(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCheatSearch(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCheatSearchAdd(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCreateMovie(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgOpenMovie(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
//HRESULT CALLBACK EnumModesCallback( LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext);

int CALLBACK test(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


#ifdef RTC_DEBUGGER
//Messages for sliders (for some reason they don't get included during the build)
#define TBM_GETPOS			(WM_USER)
#define TBM_GETRANGEMIN		(WM_USER+1)
#define TBM_GETRANGEMAX		(WM_USER+2)
#define TBM_GETTIC			(WM_USER+3)
#define TBM_SETTIC			(WM_USER+4)
#define TBM_SETPOS			(WM_USER+5)
#define TBM_SETRANGE			(WM_USER+6)
#define TBM_SETRANGEMIN		(WM_USER+7)
#define TBM_SETRANGEMAX		(WM_USER+8)
#define TBM_CLEARTICS		(WM_USER+9)
#define TBM_SETSEL			(WM_USER+10)
#define TBM_SETSELSTART		(WM_USER+11)
#define TBM_SETSELEND		(WM_USER+12)
#define TBM_GETPTICS			(WM_USER+14)
#define TBM_GETTICPOS		(WM_USER+15)
#define TBM_GETNUMTICS		(WM_USER+16)
#define TBM_GETSELSTART		(WM_USER+17)
#define TBM_GETSELEND		(WM_USER+18)
#define TBM_CLEARSEL			(WM_USER+19)
#define TBM_SETTICFREQ		(WM_USER+20)
#define TBM_SETPAGESIZE		(WM_USER+21)
#define TBM_GETPAGESIZE		(WM_USER+22)
#define TBM_SETLINESIZE		(WM_USER+23)
#define TBM_GETLINESIZE		(WM_USER+24)
#define TBM_GETTHUMBRECT	(WM_USER+25)
#define TBM_GETCHANNELRECT	(WM_USER+26)
#define TBM_SETTHUMBLENGTH	(WM_USER+27)
#define TBM_GETTHUMBLENGTH	(WM_USER+28)
#endif

#define NOTKNOWN "Unknown Company "
#define HEADER_SIZE 512
#define INFO_LEN (0xFF - 0xC0)

#define WM_CUSTKEYDOWN	(WM_USER+50)
#define WM_CUSTKEYUP	(WM_USER+51)
 
/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/
struct sGUI GUI;
bool wasInXUI;
bool exitFromXUI;
int bAdjustScreen;

typedef struct sExtList
{
	TCHAR* extension;
	bool compressed;
	struct sExtList* next;
} ExtList;
HANDLE SoundEvent;

ExtList* valid_ext=NULL;
void MakeExtFile(void);
void LoadExts(void);

extern FILE *trace_fs;
extern SCheatData Cheat;
extern bool8 do_frame_adjust;
extern HXUIOBJ phObj;
extern HXUIOBJ hScene;

HINSTANCE g_hInst;

#ifdef DEBUGGER
extern "C" void Trace ();
#endif



static const char *rom_filename = NULL;
volatile bool exitInputThread;

CDirect3D Direct3D;  

struct SJoypad Joypad[16] = {
    {
        true,					/* Joypad 1 enabled */
			VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,	/* Left, Right, Up, Down */
			0, 0, 0, 0,             /* Left_Up, Left_Down, Right_Up, Right_Down */
			VK_SPACE, VK_RETURN,    /* Start, Select */
			'V', 'C',				/* A B */
			'D', 'X',				/* X Y */
			'A', 'S'				/* L R */
    },
    {
			true,                                  /* Joypad 2 enabled */
				'J', 'L', 'I', 'K',	/* Left, Right, Up, Down */
				0, 0, 0, 0,         /* Left_Up, Left_Down, Right_Up, Right_Down */
				'P', 'O',          /* Start, Select */
				'H', 'G',			/* A B */
				'T', 'F',			/* X Y */
				'Y', 'U'			/* L R */
		},
		{
				false,                                  /* Joypad 3 disabled */
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0,
					0, 0, 0, 0, 0, 0
			},
			{
					false,                                  /* Joypad 4 disabled */
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0,
						0, 0, 0, 0, 0, 0
				},
				{
						false,                                  /* Joypad 5 disabled */
							0, 0, 0, 0,
							0, 0, 0, 0,
							0, 0,
							0, 0, 0, 0, 0, 0
					},
				{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },/* Joypad 6 disabled */
				{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },/* Joypad 7 disabled */
				{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },/* Joypad 8 disabled */
	{
			false,                                  /* Joypad 1 Turbo disabled */
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,                                  /* Joypad 2 Turbo disabled */
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,                                  /* Joypad 3 Turbo disabled */
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,                                  /* Joypad 4 Turbo disabled */
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,                                  /* Joypad 5 Turbo disabled */
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },/* Joypad 6 Turbo disabled */
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },/* Joypad 7 Turbo disabled */
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },/* Joypad 8 Turbo disabled */
};

// stores on/off toggle info for each key of each controller
SJoypad ToggleJoypadStorage [8] = {
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },
};

SJoypad TurboToggleJoypadStorage [8] = {
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
	{
			false,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		},
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },
			{ false, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0 },
};

struct SCustomKeys CustomKeys = {
	{/*VK_OEM_PLUS*/0xBB,0}, // speed+ (=)
	{/*VK_OEM_MINUS*/0xBD,0}, // speed- (-)
	{VK_PAUSE,0}, // pause (PAUSE)
	{/*VK_OEM_5*/0xDC,0}, // frame advance (\)
	{/*VK_OEM_PLUS*/0xBB,CUSTKEY_SHIFT_MASK}, // skip+ (_)
	{/*VK_OEM_MINUS*/0xBD,CUSTKEY_SHIFT_MASK}, // skip- (+)
	{/*VK_OEM_3*/0xC0,0}, // superscope turbo (`)
	{/*VK_OEM_2*/0xBF,0}, // superscope pause (/)
	{/*VK_OEM_PERIOD*/0xBE,0}, // frame counter (.)
	{'8',CUSTKEY_SHIFT_MASK}, // movie read-only (*)
	{{VK_F1,CUSTKEY_SHIFT_MASK}, // save keys
	 {VK_F2,CUSTKEY_SHIFT_MASK},
	 {VK_F3,CUSTKEY_SHIFT_MASK},
	 {VK_F4,CUSTKEY_SHIFT_MASK},
	 {VK_F5,CUSTKEY_SHIFT_MASK},
	 {VK_F6,CUSTKEY_SHIFT_MASK},
	 {VK_F7,CUSTKEY_SHIFT_MASK},
	 {VK_F8,CUSTKEY_SHIFT_MASK},
	 {VK_F9,CUSTKEY_SHIFT_MASK},
	 {VK_F10,CUSTKEY_SHIFT_MASK}},
	{{VK_F1,0}, // load keys
	 {VK_F2,0},
	 {VK_F3,0},
	 {VK_F4,0},
	 {VK_F5,0},
	 {VK_F6,0},
	 {VK_F7,0},
	 {VK_F8,0},
	 {VK_F9,0},
	 {VK_F10,0}},
	{VK_TAB,0}, // fast forward (TAB)
	{/*VK_OEM_COMMA*/0xBC,0}, // show pressed keys/buttons (,)
	{VK_F12,0}, // save screenshot (F12)
	{0,0}, // slot plus (disabled by default)
	{0,0}, // slot minus (disabled by default)
	{0,0}, // slot save (disabled by default)
	{0,0}, // slot load (disabled by default)
	{'1',0}, // background layer 1
	{'2',0}, // background layer 2
	{'3',0}, // background layer 3
	{'4',0}, // background layer 4
	{'5',0}, // sprite layer
	{'8',0}, // Clipping Windows
//	{'8',0}, // BG Layering hack
	{'9',0}, // Transparency
	{'0',0}, // HDMA Emulation
	{'6',CUSTKEY_SHIFT_MASK}, // GLCube Mode
//	{'9',CUSTKEY_SHIFT_MASK}, // Interpolate Mode 7
	{'6',0}, // Joypad Swap
	{'7',0}, // Switch Controllers
	{VK_NEXT,CUSTKEY_SHIFT_MASK}, // Turbo A
	{VK_END,CUSTKEY_SHIFT_MASK}, // Turbo B
	{VK_HOME,CUSTKEY_SHIFT_MASK}, // Turbo Y
	{VK_PRIOR,CUSTKEY_SHIFT_MASK}, // Turbo X
	{VK_INSERT,CUSTKEY_SHIFT_MASK}, // Turbo L
	{VK_DELETE,CUSTKEY_SHIFT_MASK}, // Turbo R
	{0,0}, // Turbo Start
	{0,0}, // Turbo Select
	{0,0}, // Turbo Left
	{0,0}, // Turbo Up
	{0,0}, // Turbo Right
	{0,0}, // Turbo Down
	{{0,0}, // Select save slot 0
	 {0,0}, // Select save slot 1
	 {0,0}, // Select save slot 2
	 {0,0}, // Select save slot 3
	 {0,0}, // Select save slot 4
	 {0,0}, // Select save slot 5
	 {0,0}, // Select save slot 6
	 {0,0}, // Select save slot 7
	 {0,0}, // Select save slot 8
	 {0,0}}, // Select save slot 9
	{'R',CUSTKEY_CTRL_MASK|CUSTKEY_SHIFT_MASK}, // Reset Game
	{0,0}, // Toggle Cheats
};


struct SSoundRates
{
    uint32 rate;
    int ident;
} SoundRates[9] = {
    { 8000, ID_SOUND_8000HZ},
    {11025, ID_SOUND_11025HZ},
    {16000, ID_SOUND_16000HZ},
    {22050, ID_SOUND_22050HZ},
    {30000, ID_SOUND_30000HZ},
	{32000, ID_SOUND_32000HZ},
    {35000, ID_SOUND_35000HZ},
    {44100, ID_SOUND_44100HZ},
    {48000, ID_SOUND_48000HZ}
};

static uint32 FrameTimings[] = {
	4000, 4000, 8333, 11667, 16667, 20000, 33333, 66667, 133333, 300000, 500000, 1000000, 1000000
};

// Languages supported by Snes9X: Windows
// 0 - English [Default]
// 1 - Dutch/Nederlands
struct sLanguages Languages[] = {
	{ IDR_MENU_US,
		TEXT("DirectX failed to initialize!"),
		TEXT("DirectDraw failed to set the selected display mode!"),
		TEXT("DirectSound failed to initialize; no sound will be played."),
		TEXT("These settings won't take effect until you restart the emulator."),
		TEXT("The frame timer failed to initialize, please do NOT select the automatic framerate option or Snes9X will crash!")},
	{ IDR_MENU_NL,
	TEXT("Er is een fout opgetreden tijdens het initalizeren van DirectX!"),
	TEXT("Er is een fout opgetreden tijdens het verander van scherm modus!"),
	TEXT("Er is een fout opgetreden tijdens het initializeren van DirectSound, er zal geen geluid worden afgespeeld."),
	TEXT("Deze opties worden pas toegepast als de emulator opnieuw is opgestart."),
	TEXT("Er is een fout opgetreden tijdens het initializeren van de frame timer, kies NIET de automatische framerate optie want dan zal Snes9X crashen!")}
};

struct OpenMovieParams
{
	char Path[_MAX_PATH];
	bool8 ReadOnly;
	bool8 DisplayInput;
	uint8 ControllersMask;
	uint8 Opts;
	uint8 SyncFlags;
	wchar_t Metadata[MOVIE_MAX_METADATA];
};





std::vector<dMode> dm;
/*****************************************************************************/
/* WinProc                                                                   */
/*****************************************************************************/
void DoAVIOpen(const char* filename);
void DoAVIClose(int reason);
bool ReInitSound(int mode);
bool SetupSound(void);
void RestoreGUIDisplay ();
void RestoreSNESDisplay ();
void FreezeUnfreeze (int slot, bool8 freeze);
void CheckDirectoryIsWritable (const char *filename);
static void CheckMenuStates ();
static void ResetFrameTimer ();
bool8 LoadROM (const char *filename);
bool8 LoadMultiROM (const char *filename, const char *filename2);
BOOL run_loop;
#ifdef NETPLAY_SUPPORT
static void EnableServer (bool8 enable);
#endif
void WinDeleteRecentGamesList ();
const char* WinParseCommandLineAndLoadConfigFile (char *line);
void WinRegisterConfigItems ();
void WinSaveConfigFile ();
 
void WinLockConfigFile ();
void WinUnlockConfigFile ();
void WinCleanupConfigData ();

#include "../ppu.h"
#include "../snapshot.h"
extern "C" const char *S9xGetFilename (const char *, enum s9x_getdirtype dirtype);
const char *S9xGetFilenameInc (const char *);
void S9xSetRecentGames ();
void S9xAddToRecentGames (const char *filename);
void S9xRemoveFromRecentGames (int i);

extern void S9xReRefresh();
extern CSnes360App app;

static void absToRel(char* relPath, const char* absPath, const char* baseDir)
{
	strcpy(relPath, absPath);
	if(!strncasecmp(absPath, baseDir, strlen(baseDir)))
	{
		char temp [MAX_PATH];
		temp[MAX_PATH-3]='\0';
		const char* relative = absPath+strlen(baseDir);
		while(relative[0]=='\\' || relative[0]=='/')
			relative++;
		relPath[0]='.'; relPath[1]='\\';
		strcpy(relPath+2, relative);
	}
}

void S9xMouseOn ()
{
	if(Settings.StopEmulation)
		return;

    if (GUI.ControllerOption==SNES_MOUSE || GUI.ControllerOption==SNES_MOUSE_SWAPPED)
    {
		 
    }
    else if (GUI.ControllerOption!=SNES_SUPERSCOPE && GUI.ControllerOption!=SNES_JUSTIFIER && GUI.ControllerOption!=SNES_JUSTIFIER_2)
    {
        
        GUI.CursorTimer = 60;
    }
    else
	{
		 
	}
}

void ChangeInputDevice(void)
{
	Settings.MouseMaster = false;
	Settings.JustifierMaster = false;
	Settings.SuperScopeMaster = false;
	Settings.MultiPlayer5Master = false;

/*	CheckMenuItem(GUI.hMenu, IDM_ENABLE_MULTITAP, MFS_UNCHECKED);
	CheckMenuItem(GUI.hMenu, IDM_JUSTIFIER, MFS_UNCHECKED);
	CheckMenuItem(GUI.hMenu, IDM_MOUSE_TOGGLE, MFS_UNCHECKED);
	CheckMenuItem(GUI.hMenu, IDM_SCOPE_TOGGLE, MFS_UNCHECKED);
	CheckMenuItem(GUI.hMenu, IDM_MOUSE_SWAPPED, MFS_UNCHECKED);
	CheckMenuItem(GUI.hMenu, IDM_JUSTIFIERS, MFS_UNCHECKED);
	CheckMenuItem(GUI.hMenu, IDM_MULTITAP8, MFS_UNCHECKED);
	CheckMenuItem(GUI.hMenu, IDM_SNES_JOYPAD, MFS_UNCHECKED);*/

	switch(GUI.ControllerOption)
	{
	case SNES_MOUSE:
		Settings.MouseMaster = true;
		S9xSetController(0, CTL_MOUSE,      0, 0, 0, 0);
		S9xSetController(1, CTL_JOYPAD,     1, 0, 0, 0);
		//CheckMenuItem(GUI.hMenu, IDM_MOUSE_TOGGLE, MFS_CHECKED);
		break;
	case SNES_MOUSE_SWAPPED:
		Settings.MouseMaster = true;
		S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
		S9xSetController(1, CTL_MOUSE,      1, 0, 0, 0);
		//CheckMenuItem(GUI.hMenu, IDM_MOUSE_SWAPPED, MFS_CHECKED);
		break;
	case SNES_SUPERSCOPE:
		Settings.SuperScopeMaster = true;
		S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
		S9xSetController(1, CTL_SUPERSCOPE, 0, 0, 0, 0);
		//CheckMenuItem(GUI.hMenu, IDM_SCOPE_TOGGLE, MFS_CHECKED);
		break;
	case SNES_MULTIPLAYER5:
		Settings.MultiPlayer5Master = true;
		S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
		S9xSetController(1, CTL_MP5,        1, 2, 3, 4);
		//CheckMenuItem(GUI.hMenu, IDM_ENABLE_MULTITAP, MFS_CHECKED);
		break;
	case SNES_MULTIPLAYER8:
		Settings.MultiPlayer5Master = true;
		S9xSetController(0, CTL_MP5,        0, 1, 2, 3);
		S9xSetController(1, CTL_MP5,        4, 5, 6, 7);
		//CheckMenuItem(GUI.hMenu, IDM_ENABLE_MULTITAP, MFS_CHECKED);
		break;
	case SNES_JUSTIFIER:
		Settings.JustifierMaster = true;
		S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
		S9xSetController(1, CTL_JUSTIFIER,  0, 0, 0, 0);
		//CheckMenuItem(GUI.hMenu, IDM_JUSTIFIER, MFS_CHECKED);
		break;
	case SNES_JUSTIFIER_2:
		Settings.JustifierMaster = true;
		S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
		S9xSetController(1, CTL_JUSTIFIER,  1, 0, 0, 0);
		//CheckMenuItem(GUI.hMenu, IDM_JUSTIFIERS, MFS_CHECKED);
		break;
	default:
	case SNES_JOYPAD:
		S9xSetController(0, CTL_JOYPAD,     0, 0, 0, 0);
		S9xSetController(1, CTL_JOYPAD,     1, 0, 0, 0);
		//CheckMenuItem(GUI.hMenu, IDM_SNES_JOYPAD, MFS_CHECKED);
		break;
	}

    GUI.ControlForced = 0xff;
}

static void CenterCursor()
{
 
}


void S9xRestoreWindowTitle ()
{
    TCHAR buf [100];
    sprintf (buf, TEXT(WINDOW_TITLE), VERSION);
  
}

void SwitchToGDI()
{
	 
}


void UpdateBackBuffer()
{
  /*  GUI.ScreenCleared = true;

    if (GUI.outputMethod==DIRECTDRAW && !VOODOO_MODE && !OPENGL_MODE && GUI.FullScreen)
    {
        SwitchToGDI();

        DDBLTFX fx;

        XMemSet (&fx, 0, sizeof (fx));
        fx.dwSize = sizeof (fx);

        while (DirectDraw.lpDDSPrimary2->Blt (NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx) == DDERR_SURFACELOST)
            DirectDraw.lpDDSPrimary2->Restore ();

        if (GetMenu (GUI.hWnd) != NULL)
            DrawMenuBar (GUI.hWnd);

        GUI.FlipCounter = 0;
        DDSCAPS caps;
        caps.dwCaps = DDSCAPS_BACKBUFFER;

        LPDIRECTDRAWSURFACE2 pDDSurface;

        if (DirectDraw.lpDDSPrimary2->GetAttachedSurface (&caps, &pDDSurface) == DD_OK &&
            pDDSurface != NULL)
        {
            S9xClearSurface (pDDSurface);
			DirectDraw.lpDDSPrimary2->Flip (NULL, GUI.Vsync?DDFLIP_WAIT:DDFLIP_NOVSYNC);
            while (DirectDraw.lpDDSPrimary2->GetFlipStatus (DDGFS_ISFLIPDONE) != DD_OK)
                Sleep (0);
			if(DirectDraw.DoubleBuffered)
	            S9xClearSurface (pDDSurface);
        }
    }
    else
    {
        if (GetMenu( GUI.hWnd) != NULL)
            DrawMenuBar (GUI.hWnd);
    } */
}

void ToggleFullScreen ()
{
    S9xSetPause (PAUSE_TOGGLE_FULL_SCREEN);

if(GUI.outputMethod==DIRECT3D) {
			Direct3D.setFullscreen(true);
			}
			if(!GUI.FullScreen) {				 
				Direct3D.setFullscreen(false);
			}

	S9xGraphicsDeinit();
	S9xSetWinPixelFormat ();
	S9xInitUpdate();
	S9xGraphicsInit();
	IPPU.RenderThisFrame = true;

	UpdateBackBuffer();

	S9xClearPause (PAUSE_TOGGLE_FULL_SCREEN);
}

void S9xDisplayStateChange (const char *str, bool8 on)
{
    static char string [100];

    sprintf (string, "%s %s", str, on ? "on" : "off");
    S9xSetInfoString (string);
}

static void UpdateScale(RenderFilter & Scale, RenderFilter & NextScale)
{
	 
		S9xGraphicsDeinit();
		Scale = NextScale;
		Settings.OpenGLEnable = IS_GL_MODE(Scale);
		Settings.GlideEnable = IS_GLIDE_MODE(Scale);
		S9xSetWinPixelFormat ();
		S9xInitUpdate();
		S9xGraphicsInit();
		 
	 
}

static char InfoString [100];
static uint32 prevPadReadFrame = (uint32)-1;
static bool skipNextFrameStop = false;

int HandleKeyMessage(WPARAM wParam, LPARAM lParam)
{
 
	return 1;
}

static bool DoOpenRomDialog(char filename [_MAX_PATH], bool noCustomDlg = false)
{
	/*if(GUI.CustomRomOpen && !noCustomDlg)
	{
		try
		{
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC   = ICC_LISTVIEW_CLASSES|ICC_TREEVIEW_CLASSES;
			InitCommonControlsEx(&icex); // this could cause failure if the common control DLL isn't found

			return (1 <= DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_OPEN_ROM), GUI.hWnd, DlgOpenROMProc, (LPARAM)filename));
		}
		catch(...) {} // use standard dialog if the special one fails

		GUI.CustomRomOpen = false; // if crashed, turn off custom for next time
	}

	// standard file dialog
	{
		OPENFILENAME ofn;
		static char szFileName[MAX_PATH] = {0};
		char szPathName[MAX_PATH];
		_fullpath(szPathName, S9xGetDirectory(ROM_DIR), MAX_PATH);

		// a limited strcat that doesn't mind null characters
#define strcat0(to,from) do{XMemCpy(to,from,sizeof(from)-1);to+=sizeof(from)-1;}while(false)

		// make filter string using entries in valid_ext
		char lpfilter [8192] = {0};
		char* lpfilterptr = (char*)lpfilter;
		for(int i=0; i<2; i++)
		{
			if(!i)
				strcat0(lpfilterptr, FILE_INFO_ROM_FILE_TYPE);
			else
				strcat0(lpfilterptr, FILE_INFO_UNCROM_FILE_TYPE);
			strcat0(lpfilterptr, "\0");
			if(valid_ext) // add valid extensions to string
			{
				ExtList* ext = valid_ext;
				int extlen_approx = 0;
				bool first = true;
				while(ext && (extlen_approx < 2048))
				{
					if((!i || !ext->compressed) && ext->extension && strlen(ext->extension) < 256)
					{
						if(!first)
							strcat(lpfilterptr, ";*.");
						else
						{
							strcat(lpfilterptr, "*.");
							first = false;
						}
						strcat(lpfilterptr, ext->extension);
						extlen_approx += strlen(ext->extension) + 3;
					}
					ext = ext->next;
				}
				lpfilterptr += strlen(lpfilterptr);
			}
			else
				strcat0(lpfilterptr, "*.smc");
			strcat0(lpfilterptr, "\0");
		}
		strcat0(lpfilterptr, FILE_INFO_ANY_FILE_TYPE);
		strcat0(lpfilterptr, "\0*.*\0\0");

		ZeroMemory((LPVOID)&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GUI.hWnd;
		ofn.lpstrFilter = lpfilter;
		ofn.lpstrFile = szFileName;
		ofn.lpstrDefExt = "smc";
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
		ofn.lpstrInitialDir = szPathName;
		if(GetOpenFileName(&ofn))
		{
			strncpy(filename, ofn.lpstrFile, _MAX_PATH);
			return true;
		}
		return false;
	} */

	return true;
}


char multiRomA [MAX_PATH] = {0}; // lazy, should put in sGUI and add init to {0} somewhere
char multiRomB [MAX_PATH] = {0};


static bool startingMovie = false;

HWND cheatSearchHWND = NULL;


#define MOVIE_LOCKED_SETTING	if(S9xMovieActive()) {MessageBox(GUI.hWnd,TEXT("That setting is locked while a movie is active."),TEXT("Notice"),MB_OK|MB_ICONEXCLAMATION); break;}

/*
LRESULT CALLBACK WinProc(
						 HWND hWnd,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam)
{
    unsigned int i;
    //bool showFPS;
#ifdef NETPLAY_SUPPORT
    char hostname [100];
#endif
    switch (uMsg)
    {
	case WM_CREATE:
		g_hInst = ((LPCREATESTRUCT)lParam)->hInstance;
#ifndef MK_APU
		DeleteMenu(GUI.hMenu,IDM_CATCH_UP_SOUND,MF_BYCOMMAND);
#endif
		return 0;
	case WM_KEYDOWN:
		if(GUI.BackgroundKeyHotkeys)
			break;
	case WM_CUSTKEYDOWN:
	case WM_SYSKEYDOWN:
		{
			if(!HandleKeyMessage(wParam,lParam))
				return 0;
	        break;
		}

	case WM_KEYUP:
	case WM_CUSTKEYUP:
		{
			int modifiers = 0;
			if(GetAsyncKeyState(VK_MENU))
				modifiers |= CUSTKEY_ALT_MASK;
			if(GetAsyncKeyState(VK_CONTROL))
				modifiers |= CUSTKEY_CTRL_MASK;
			if(GetAsyncKeyState(VK_SHIFT))
				modifiers |= CUSTKEY_SHIFT_MASK;

			if(wParam == CustomKeys.FastForward.key
			&& modifiers == CustomKeys.FastForward.modifiers)
			{
				if (!GUI.TurboModeToggle)
					Settings.TurboMode = FALSE;
			}
			if(wParam == CustomKeys.ScopePause.key
			&& modifiers == CustomKeys.ScopePause.modifiers)
			{
				GUI.superscope_pause = 0;
			}
		}
		break;
	case WM_COMMAND:
		switch (wParam & 0xffff)
		{
		case ID_FILE_WRITE_AVI:
			{
				RestoreGUIDisplay ();  //exit DirectX
				OPENFILENAME  ofn;
				char  szFileName[MAX_PATH];
				char  szPathName[MAX_PATH];
				SetCurrentDirectory(S9xGetDirectory(DEFAULT_DIR));
				_fullpath(szPathName, GUI.MovieDir, MAX_PATH);
				mkdir(szPathName);

				szFileName[0] = '\0';

				ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME) );
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = GUI.hWnd;
				ofn.lpstrFilter = FILE_INFO_AVI_FILE_TYPE "\0*.avi\0" FILE_INFO_ANY_FILE_TYPE "\0*.*\0\0";
				ofn.lpstrFile = szFileName;
				ofn.lpstrDefExt = "avi";
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
				ofn.lpstrInitialDir = szPathName;
				if(GetSaveFileName( &ofn ))
				{
					ReInitSound(0);			// disable sound output 
					DoAVIOpen(szFileName);
				}
				RestoreSNESDisplay ();// re-enter after dialog
			}
			break;
		case ID_FILE_STOP_AVI:
			DoAVIClose(0);
			ReInitSound(1);				// reenable sound output
			break;
		case ID_FILE_MOVIE_STOP:
			S9xMovieStop(FALSE);
			break;
		case ID_FILE_MOVIE_PLAY:
			{
				RestoreGUIDisplay ();  //exit DirectX
				OpenMovieParams op;
				XMemSet(&op, 0, sizeof(op));
				if(DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_OPENMOVIE), hWnd, DlgOpenMovie, (LPARAM)&op) &&
					op.Path[0]!='\0')
				{
					int err=S9xMovieOpen (op.Path, op.ReadOnly);
					if(err!=SUCCESS)
					{
						_TCHAR* err_string=TEXT(MOVIE_ERR_COULD_NOT_OPEN);
						switch(err)
						{
						case FILE_NOT_FOUND:
							err_string=TEXT(MOVIE_ERR_NOT_FOUND);
							break;
						case WRONG_FORMAT:
							err_string=TEXT(MOVIE_ERR_WRONG_FORMAT);
							break;
						case WRONG_VERSION:
							err_string=TEXT(MOVIE_ERR_WRONG_VERSION);
							break;
						}
						MessageBox( hWnd, err_string, TEXT(SNES9X_INFO), MB_OK);
					}
				}
				RestoreSNESDisplay ();// re-enter after dialog
			}
			break;
		case ID_FILE_MOVIE_RECORD:
			{
				RestoreGUIDisplay ();  //exit DirectX
				OpenMovieParams op;
				XMemSet(&op, 0, sizeof(op));
				if(DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_CREATEMOVIE), hWnd, DlgCreateMovie, (LPARAM)&op) &&
					op.Path[0]!='\0')
				{
					if(Settings.ShutdownMaster)
					{
						static bool seenItOnce = false;
						if(!seenItOnce)
						{
							seenItOnce = true;
							MessageBox(hWnd, MOVIE_SHUTDOWNMASTER_WARNING, SNES9X_WARN, MB_OK);
						}
					}

					startingMovie = true;
					int err=S9xMovieCreate (op.Path, op.ControllersMask, op.Opts, op.Metadata, wcslen(op.Metadata));
					startingMovie = false;
					if(err!=SUCCESS)
					{
						_TCHAR* err_string=TEXT(MOVIE_ERR_COULD_NOT_OPEN);
						switch(err)
						{
						case FILE_NOT_FOUND:
							err_string=TEXT(MOVIE_ERR_NOT_FOUND);
							break;
						case WRONG_FORMAT:
							err_string=TEXT(MOVIE_ERR_WRONG_FORMAT);
							break;
						case WRONG_VERSION:
							err_string=TEXT(MOVIE_ERR_WRONG_VERSION);
							break;
						}
						MessageBox( hWnd, err_string, TEXT(SNES9X_INFO), MB_OK);
					}
				}
				RestoreSNESDisplay ();// re-enter after dialog
			}
			break;
		case IDM_GFX_PACKS:
			RestoreGUIDisplay ();  //exit DirectX
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_GFX_PACK), hWnd, DlgPackConfigProc);
			RestoreSNESDisplay ();// re-enter after dialog
			break;
		case IDM_CATCH_UP_SOUND:
			Settings.SampleCatchup=!Settings.SampleCatchup;
			if(Settings.SampleCatchup)
				CheckMenuItem(GUI.hMenu, IDM_CATCH_UP_SOUND, MFS_CHECKED);
			else CheckMenuItem(GUI.hMenu, IDM_CATCH_UP_SOUND, MFS_UNCHECKED);
			break;
		case IDM_SNES_JOYPAD:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_JOYPAD;
			ChangeInputDevice();
			break;
		case IDM_ENABLE_MULTITAP:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_MULTIPLAYER5;
			ChangeInputDevice();
			break;
		case IDM_SCOPE_TOGGLE:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_SUPERSCOPE;
			ChangeInputDevice();
			break;
		case IDM_JUSTIFIER:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_JUSTIFIER;
			ChangeInputDevice();
			break;
		case IDM_MOUSE_TOGGLE:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_MOUSE;
			ChangeInputDevice();
			break;
		case IDM_MOUSE_SWAPPED:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_MOUSE_SWAPPED;
			ChangeInputDevice();
			break;
		case IDM_MULTITAP8:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_MULTIPLAYER8;
			ChangeInputDevice();
			break;
		case IDM_JUSTIFIERS:
			MOVIE_LOCKED_SETTING
			GUI.ControllerOption = SNES_JUSTIFIER_2;
			ChangeInputDevice();
			break;

			//start turbo
		case ID_TURBO_R:
			GUI.TurboMask^=TURBO_R_MASK;
			if(GUI.TurboMask&TURBO_R_MASK)
				S9xSetInfoString (WINPROC_TURBO_R_ON);
			else S9xSetInfoString (WINPROC_TURBO_R_OFF);
			break;
		case ID_TURBO_L:
			GUI.TurboMask^=TURBO_L_MASK;
			if(GUI.TurboMask&TURBO_L_MASK)
				S9xSetInfoString (WINPROC_TURBO_L_ON);
			else S9xSetInfoString (WINPROC_TURBO_L_OFF);
			break;
		case ID_TURBO_A:
			GUI.TurboMask^=TURBO_A_MASK;
			if(GUI.TurboMask&TURBO_A_MASK)
				S9xSetInfoString (WINPROC_TURBO_A_ON);
			else S9xSetInfoString (WINPROC_TURBO_A_OFF);
			break;
		case ID_TURBO_B:
			GUI.TurboMask^=TURBO_B_MASK;
			if(GUI.TurboMask&TURBO_B_MASK)
				S9xSetInfoString (WINPROC_TURBO_B_ON);
			else S9xSetInfoString (WINPROC_TURBO_B_OFF);
			break;
		case ID_TURBO_Y:
			GUI.TurboMask^=TURBO_Y_MASK;
			if(GUI.TurboMask&TURBO_Y_MASK)
				S9xSetInfoString (WINPROC_TURBO_Y_ON);
			else S9xSetInfoString (WINPROC_TURBO_Y_OFF);
			break;
		case ID_TURBO_X:
			GUI.TurboMask^=TURBO_X_MASK;
			if(GUI.TurboMask&TURBO_X_MASK)
				S9xSetInfoString (WINPROC_TURBO_X_ON);
			else S9xSetInfoString (WINPROC_TURBO_X_OFF);
			break;
		case ID_TURBO_START:
			GUI.TurboMask^=TURBO_STA_MASK;
			if(GUI.TurboMask&TURBO_STA_MASK)
				S9xSetInfoString (WINPROC_TURBO_START_ON);
			else S9xSetInfoString (WINPROC_TURBO_START_OFF);
			break;
		case ID_TURBO_SELECT:
			GUI.TurboMask^=TURBO_SEL_MASK;
			if(GUI.TurboMask&TURBO_SEL_MASK)
				S9xSetInfoString (WINPROC_TURBO_SEL_ON);
			else S9xSetInfoString (WINPROC_TURBO_SEL_OFF);
			break;
		case ID_TURBO_LEFT:
			GUI.TurboMask^=TURBO_LEFT_MASK;
			if(GUI.TurboMask&TURBO_LEFT_MASK)
				S9xSetInfoString (WINPROC_TURBO_LEFT_ON);
			else S9xSetInfoString (WINPROC_TURBO_LEFT_OFF);
			break;
		case ID_TURBO_UP:
			GUI.TurboMask^=TURBO_UP_MASK;
			if(GUI.TurboMask&TURBO_UP_MASK)
				S9xSetInfoString (WINPROC_TURBO_UP_ON);
			else S9xSetInfoString (WINPROC_TURBO_UP_OFF);
			break;
		case ID_TURBO_RIGHT:
			GUI.TurboMask^=TURBO_RIGHT_MASK;
			if(GUI.TurboMask&TURBO_RIGHT_MASK)
				S9xSetInfoString (WINPROC_TURBO_RIGHT_ON);
			else S9xSetInfoString (WINPROC_TURBO_RIGHT_OFF);
			break;
		case ID_TURBO_DOWN:
			GUI.TurboMask^=TURBO_DOWN_MASK;
			if(GUI.TurboMask&TURBO_DOWN_MASK)
				S9xSetInfoString (WINPROC_TURBO_DOWN_ON);
			else S9xSetInfoString (WINPROC_TURBO_DOWN_OFF);
			break;
			//end turbo
		case ID_OPTIONS_DISPLAY:
			{
				int old_scale = GUI.NextScale;
//				bool old_stretch = GUI.Stretch;
				bool wasFullScreen = GUI.FullScreen;

				RestoreGUIDisplay ();
				//showFPS = Settings.DisplayFrameRate ? true : false;
				//if (!VOODOO_MODE && !GUI.FullScreen)
				//	GetWindowRect (GUI.hWnd, &GUI.window_size);
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NEWDISPLAY), hWnd, DlgFunky);
				//_DirectXConfig (DirectX.lpDD, &Settings, &GUI, &showFPS);

				//Settings.DisplayFrameRate = showFPS;
				SwitchToGDI();
				if (GUI.NextScale != old_scale)
				{
					UpdateScale((RenderFilter &)old_scale, GUI.NextScale);
				}
				GUI.ScaleHiRes = GUI.NextScaleHiRes;
				RestoreSNESDisplay ();
				if (!GUI.FullScreen && wasFullScreen)
				{
					MoveWindow (GUI.hWnd, GUI.window_size.left,
						GUI.window_size.top,
						GUI.window_size.right - GUI.window_size.left,
						GUI.window_size.bottom - GUI.window_size.top, TRUE);
				}
				S9xGraphicsDeinit();
//				S9xDeinitUpdate();
				S9xSetWinPixelFormat ();
				S9xInitUpdate();
				S9xGraphicsInit();

				IPPU.RenderThisFrame = false;

//				if (old_stretch != GUI.Stretch || old_scale != GUI.Scale)
				{
					RECT rect;
					GetClientRect (GUI.hWnd, &rect);
					InvalidateRect (GUI.hWnd, &rect, true);
				}
				break;
			}

		case ID_OPTIONS_JOYPAD:
            RestoreGUIDisplay ();
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_INPUTCONFIG), hWnd, DlgInputConfig);
            RestoreSNESDisplay ();
            break;

		case ID_OPTIONS_KEYCUSTOM:
            RestoreGUIDisplay ();
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_KEYCUSTOM), hWnd, DlgHotkeyConfig);
            RestoreSNESDisplay ();
            break;

		case ID_FILE_LOADMULTICART:
			{
#ifdef NETPLAY_SUPPORT
				if (Settings.NetPlay && !Settings.NetPlayServer)
				{
					S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER, WINPROC_DISCONNECT);
					break;
				}
#endif
				RestoreGUIDisplay ();

				const bool ok = (1 <= DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_MULTICART), GUI.hWnd, DlgMultiROMProc, (LPARAM)NULL));

				if(ok)
				{
					if (!Settings.StopEmulation)
					{
						Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
						S9xSaveCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
					}
					Settings.StopEmulation = !LoadMultiROM (multiRomA, multiRomB);
					if (!Settings.StopEmulation)
					{
						bool8 loadedSRAM = Memory.LoadSRAM (S9xGetFilename (".srm", SRAM_DIR));
						if(!loadedSRAM) // help migration from earlier Snes9x versions by checking ROM directory for savestates
							Memory.LoadSRAM (S9xGetFilename (".srm", ROMFILENAME_DIR));
						S9xLoadCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
//						S9xAddToRecentGames (multiRomA, multiRomB);
						CheckDirectoryIsWritable (S9xGetFilename (".---", SNAPSHOT_DIR));
						CheckMenuStates ();
#ifdef NETPLAY_SUPPORT
						// still valid with multicart ???
						if (NPServer.SendROMImageOnConnect)
							S9xNPServerQueueSendingROMImage ();
						else
							S9xNPServerQueueSendingLoadROMRequest (Memory.ROMName);
#endif
					}

					if(GUI.ControllerOption == SNES_SUPERSCOPE)
						SetCursor (GUI.GunSight);
					else
					{
						SetCursor (GUI.Arrow);
						GUI.CursorTimer = 60;
					}
					Settings.Paused = false;
				}

				RestoreSNESDisplay ();
				GUI.ScreenCleared = true;
			}
			break;

		case ID_FILE_LOAD_GAME:
			{
				char filename [_MAX_PATH];

#ifdef NETPLAY_SUPPORT
				if (Settings.NetPlay && !Settings.NetPlayServer)
				{
					S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER,
						WINPROC_DISCONNECT);
					break;
				}
#endif
				RestoreGUIDisplay ();

				if(DoOpenRomDialog(filename))
				{
					if (!Settings.StopEmulation)
					{
						Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
						S9xSaveCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
					}
					Settings.StopEmulation = !LoadROM (filename);
					if (!Settings.StopEmulation)
					{
						bool8 loadedSRAM = Memory.LoadSRAM (S9xGetFilename (".srm", SRAM_DIR));
						if(!loadedSRAM) // help migration from earlier Snes9x versions by checking ROM directory for savestates
							Memory.LoadSRAM (S9xGetFilename (".srm", ROMFILENAME_DIR));
						S9xLoadCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
						S9xAddToRecentGames (filename);
						CheckDirectoryIsWritable (S9xGetFilename (".---", SNAPSHOT_DIR));
						CheckMenuStates ();
#ifdef NETPLAY_SUPPORT
						if (NPServer.SendROMImageOnConnect)
							S9xNPServerQueueSendingROMImage ();
						else
							S9xNPServerQueueSendingLoadROMRequest (Memory.ROMName);
#endif
					}

					if(GUI.ControllerOption == SNES_SUPERSCOPE)
						SetCursor (GUI.GunSight);
					else
					{
						SetCursor (GUI.Arrow);
						GUI.CursorTimer = 60;
					}
					Settings.Paused = false;
				}

				RestoreSNESDisplay ();
				GUI.ScreenCleared = true;
			}
			break;

		case ID_FILE_EXIT:
            S9xSetPause (PAUSE_EXIT);
#ifdef USE_GLIDE
            S9xGlideEnable (FALSE);
#endif
            PostMessage (hWnd, WM_DESTROY, 0, 0);
            break;

		case ID_WINDOW_HIDEMENUBAR:
            if( GetMenu( GUI.hWnd) == NULL)
                SetMenu( GUI.hWnd, GUI.hMenu);
            else
                SetMenu( GUI.hWnd, NULL);
            GUI.ScreenCleared = true;
            break;

		case ID_LANGUAGE_ENGLISH:
            GUI.Language = 0;

            SetMenu( GUI.hWnd, LoadMenu( GUI.hInstance, MAKEINTRESOURCE( Languages[ GUI.Language].idMenu)));
            DestroyMenu( GUI.hMenu);
            GUI.hMenu = GetMenu( GUI.hWnd);
            break;
		case ID_LANGUAGE_NEDERLANDS:
            GUI.Language = 1;

            SetMenu( GUI.hWnd, LoadMenu( GUI.hInstance, MAKEINTRESOURCE( Languages[ GUI.Language].idMenu)));
            DestroyMenu( GUI.hMenu);
            GUI.hMenu = GetMenu( GUI.hWnd);
            break;
#ifdef NETPLAY_SUPPORT
		case ID_NETPLAY_SERVER:
            S9xRestoreWindowTitle ();
            EnableServer (!Settings.NetPlayServer);
			if(Settings.NetPlayServer)
			{
				char localhostmsg [512];
				// FIXME: need winsock2.h for this, don't know how to include it
				//struct addrinfo *aiList = NULL;
				//if(getaddrinfo("localhost", Settings.Port, NULL, &aiList) == 0)
				//{
				//	sprintf(localhostmsg, "Your server address is: %s", aiList->ai_canonname);
				//	MessageBox(GUI.hWnd,localhostmsg,"Note",MB_OK);
				//}
				//else
				{
					char localhostname [256];
					gethostname(localhostname,256);
					sprintf(localhostmsg, "Your host name is: %s\nYour port number is: %d", localhostname, Settings.Port);
					MessageBox(GUI.hWnd,localhostmsg,"Note",MB_OK);
				}
			}
            break;
        case ID_NETPLAY_CONNECT:
            RestoreGUIDisplay ();
			if(1<=DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_NETCONNECT), hWnd, DlgNetConnect,(LPARAM)&hostname))

            {


				S9xSetPause (PAUSE_NETPLAY_CONNECT);

                if (!S9xNPConnectToServer (hostname, Settings.Port,
					Memory.ROMName))
                {
                    S9xClearPause (PAUSE_NETPLAY_CONNECT);
                }
            }

			RestoreSNESDisplay ();
            break;
        case ID_NETPLAY_DISCONNECT:
            if (Settings.NetPlay)
            {
                Settings.NetPlay = FALSE;
                S9xNPDisconnect ();
            }
            if (Settings.NetPlayServer)
            {
                Settings.NetPlayServer = FALSE;
                S9xNPStopServer ();
            }
            break;
        case ID_NETPLAY_OPTIONS:
			{
				bool8 old_netplay_server = Settings.NetPlayServer;
				RestoreGUIDisplay ();
				if(1<=DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NPOPTIONS), hWnd, DlgNPOptions))
				{
					if (old_netplay_server != Settings.NetPlayServer)
					{
						Settings.NetPlayServer = old_netplay_server;
						S9xRestoreWindowTitle ();
						EnableServer (!Settings.NetPlayServer);
					}
				}
				RestoreSNESDisplay ();
				break;
			}
        case ID_NETPLAY_SYNC:
            S9xNPServerQueueSyncAll ();
            break;
        case ID_NETPLAY_ROM:
            if (NPServer.SyncByReset)
            {
			if (MessageBox (GUI.hWnd, TEXT(WINPROC_NET_RESTART), TEXT(SNES9X_WARN),
											MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
											break;
            }
            S9xNPServerQueueSendingROMImage ();
            break;
        case ID_NETPLAY_SEND_ROM_ON_CONNECT:
            NPServer.SendROMImageOnConnect ^= TRUE;
            break;
        case ID_NETPLAY_SYNC_BY_RESET:
            NPServer.SyncByReset ^= TRUE;
            break;
#endif
        case ID_SOUND_8000HZ:
		case ID_SOUND_11025HZ:
		case ID_SOUND_16000HZ:
		case ID_SOUND_22050HZ:
		case ID_SOUND_30000HZ:
		case ID_SOUND_35000HZ:
		case ID_SOUND_44100HZ:
		case ID_SOUND_48000HZ:
		case ID_SOUND_32000HZ:
			Settings.Mute = FALSE;
            for( i = 0; i < COUNT(SoundRates); i ++)
				if (SoundRates[i].ident == (int) wParam)
				{
                    Settings.SoundPlaybackRate = SoundRates [i].rate;
					if (!SetupSound())
                    {	MessageBox( GUI.hWnd, Languages[ GUI.Language].errInitDS, TEXT(SNES9X_DXS), MB_OK | MB_ICONINFORMATION);	}
                    break;
				}
				break;

        case ID_CHANNELS_CHANNEL1: S9xToggleSoundChannel(0); break;
        case ID_CHANNELS_CHANNEL2: S9xToggleSoundChannel(1); break;
        case ID_CHANNELS_CHANNEL3: S9xToggleSoundChannel(2); break;
        case ID_CHANNELS_CHANNEL4: S9xToggleSoundChannel(3); break;
        case ID_CHANNELS_CHANNEL5: S9xToggleSoundChannel(4); break;
        case ID_CHANNELS_CHANNEL6: S9xToggleSoundChannel(5); break;
        case ID_CHANNELS_CHANNEL7: S9xToggleSoundChannel(6); break;
        case ID_CHANNELS_CHANNEL8: S9xToggleSoundChannel(7); break;
        case ID_CHANNELS_ENABLEALL: S9xToggleSoundChannel(8); break;

		case ID_SOUND_NOSOUND:
			Settings.Mute = TRUE;
			Settings.SoundPlaybackRate = 32000;
			ReInitSound(1);
            break;
        case ID_SOUND_25MS:
            Settings.SoundBufferSize = 1;
            SetupSound();
            break;
        case ID_SOUND_50MS:
            Settings.SoundBufferSize = 2;
            SetupSound();
            break;
        case ID_SOUND_100MS:
            Settings.SoundBufferSize = 4;
            SetupSound();
            break;
        case ID_SOUND_200MS:
            Settings.SoundBufferSize = 8;
            SetupSound();
            break;
        case ID_SOUND_500MS:
            Settings.SoundBufferSize = 16;
            SetupSound();
            break;
        case ID_SOUND_1S:
            Settings.SoundBufferSize = 32;
            SetupSound();
            break;
        case ID_SOUND_2S:
            Settings.SoundBufferSize = 64;
            SetupSound();
            break;

        case ID_SOUND_STEREO:
            Settings.Stereo = !Settings.Stereo;
            ReInitSound(1);
            break;
        case ID_SOUND_REVERSE_STEREO:
            Settings.ReverseStereo = !Settings.ReverseStereo;
            break;
        case ID_SOUND_16BIT:
            Settings.SixteenBitSound = !Settings.SixteenBitSound;
            ReInitSound(1);
            break;
        case ID_SOUND_INTERPOLATED:
            Settings.InterpolatedSound = !Settings.InterpolatedSound;
			S9xDisplayStateChange (WINPROC_INTERPOLATED_SND, Settings.InterpolatedSound);
            break;
        case ID_SOUND_SYNC:
            Settings.SoundSync = !Settings.SoundSync;
			S9xDisplayStateChange (WINPROC_SYNC_SND, Settings.SoundSync);
            break;
        case ID_SOUND_OPTIONS:
			{
				struct SSettings orig = Settings;
				RestoreGUIDisplay ();
				if(1<=DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_SOUND_OPTS),hWnd,DlgSoundConf, (LPARAM)&Settings))
				{
					if (orig.NextAPUEnabled != Settings.NextAPUEnabled)
					{
						if (!Settings.NextAPUEnabled)
						{
							if (MessageBox (GUI.hWnd, TEXT(WINPROC_SND_OFF),
															TEXT(SNES9X_SNDQ),
															MB_YESNO | MB_ICONQUESTION) == IDNO)
							{
								Settings.NextAPUEnabled = orig.NextAPUEnabled;
							}
							else
							{
								Settings.APUEnabled = FALSE;
								ReInitSound(0);
							}
						}
						else
						{
							if (!Settings.StopEmulation)
							{
                            MessageBox (GUI.hWnd, TEXT(WINPROC_SND_RESTART), TEXT(SNES9X_SNDQ),
														MB_OK | MB_ICONINFORMATION);
							}
							else
								Settings.APUEnabled = Settings.NextAPUEnabled;
						}
					}
					else
						if (memcmp(&orig,&Settings,sizeof(SSettings)))
						{
							ReInitSound(1);
						}
				}
				RestoreSNESDisplay ();
				break;
			}
#ifdef RTC_DEBUGGER
				case IDM_7110_RTC:
					{
						struct SPC7110RTC origrtc = s7r.rtc;
						RestoreGUIDisplay ();
						if(1<=DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_7110_RTC),hWnd,SPC7110rtc, (LPARAM)&origrtc))
						{
							rtc_f9.reg[0x00]=origrtc.reg[0x00];
							rtc_f9.reg[0x01]=origrtc.reg[0x01];
							rtc_f9.reg[0x02]=origrtc.reg[0x02];
							rtc_f9.reg[0x03]=origrtc.reg[0x03];
							rtc_f9.reg[0x04]=origrtc.reg[0x04];
							rtc_f9.reg[0x05]=origrtc.reg[0x05];
							rtc_f9.reg[0x06]=origrtc.reg[0x06];
							rtc_f9.reg[0x07]=origrtc.reg[0x07];
							rtc_f9.reg[0x08]=origrtc.reg[0x08];
							rtc_f9.reg[0x09]=origrtc.reg[0x09];
							rtc_f9.reg[0x0A]=origrtc.reg[0x0A];
							rtc_f9.reg[0x0B]=origrtc.reg[0x0B];
							rtc_f9.reg[0x0C]=origrtc.reg[0x0C];

							rtc_f9.reg[0x0D]=origrtc.reg[0x0D];

							rtc_f9.reg[0x0E]=origrtc.reg[0x0E];
							rtc_f9.reg[0x0F]=origrtc.reg[0x0F];
							rtc_f9.last_used=time(NULL);

						}
						RestoreSNESDisplay ();
						break;
					}
#endif
						case ID_WINDOW_FULLSCREEN:
							ToggleFullScreen ();
							break;
						case ID_WINDOW_STRETCH: {
							GUI.Stretch = !GUI.Stretch;
							if(!GUI.Stretch != !GUI.VideoMemory) {
								GUI.VideoMemory = !GUI.VideoMemory;
								RestoreSNESDisplay ();
							}
							RECT rect;
							GetClientRect (GUI.hWnd, &rect);
							InvalidateRect (GUI.hWnd, &rect, true);
						}	break;
						case ID_WINDOW_ASPECTRATIO: {
							GUI.AspectRatio = !GUI.AspectRatio;
							RECT rect;
							GetClientRect (GUI.hWnd, &rect);
							InvalidateRect (GUI.hWnd, &rect, true);
						}	break;
						case ID_WINDOW_VIDMEM: {
							GUI.VideoMemory = !GUI.VideoMemory;
							RestoreSNESDisplay ();
							RECT rect;
							GetClientRect (GUI.hWnd, &rect);
							InvalidateRect (GUI.hWnd, &rect, true);
						}	break;
						case ID_FILE_SAVE_SPC_DATA:
							spc_is_dumping = 1;
							//                    S9xSPCDump (S9xGetFilenameInc (".spc"));
							break;
						case ID_SAVESCREENSHOT:
							Settings.TakeScreenshot=true;
							break;
						case ID_FILE_SAVE_SRAM_DATA: {
							bool8 success = Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
							if(!success)
								S9xMessage(S9X_ERROR, S9X_FREEZE_FILE_INFO, SRM_SAVE_FAILED);
						}	break;
						case ID_FILE_RESET:
#ifdef NETPLAY_SUPPORT
							if (Settings.NetPlayServer)
							{
								S9xNPReset ();
								ReInitSound(1);
							}
							else
								if (!Settings.NetPlay)
#endif
								{
									S9xMovieUpdateOnReset ();
									if(S9xMoviePlaying())
										S9xMovieStop (TRUE);
									S9xSoftReset ();
									ReInitSound(1);
								}
								if(!S9xMovieRecording())
									Settings.Paused = false;
								break;
						case ID_FILE_PAUSE:
							Settings.Paused = !Settings.Paused;
							Settings.FrameAdvance = false;
							GUI.FrameAdvanceJustPressed = 0;
							break;
						case ID_FILE_LOAD1:
							FreezeUnfreeze (0, FALSE);
							break;
						case ID_FILE_LOAD2:
							FreezeUnfreeze (1, FALSE);
							break;
						case ID_FILE_LOAD3:
							FreezeUnfreeze (2, FALSE);
							break;
						case ID_FILE_LOAD4:
							FreezeUnfreeze (3, FALSE);
							break;
						case ID_FILE_LOAD5:
							FreezeUnfreeze (4, FALSE);
							break;
						case ID_FILE_LOAD6:
							FreezeUnfreeze (5, FALSE);
							break;
						case ID_FILE_LOAD7:
							FreezeUnfreeze (6, FALSE);
							break;
						case ID_FILE_LOAD8:
							FreezeUnfreeze (7, FALSE);
							break;
						case ID_FILE_LOAD9:
							FreezeUnfreeze (8, FALSE);
							break;
						case ID_FILE_SAVE1:
							FreezeUnfreeze (0, TRUE);
							break;
						case ID_FILE_SAVE2:
							FreezeUnfreeze (1, TRUE);
							break;
						case ID_FILE_SAVE3:
							FreezeUnfreeze (2, TRUE);
							break;
						case ID_FILE_SAVE4:
							FreezeUnfreeze (3, TRUE);
							break;
						case ID_FILE_SAVE5:
							FreezeUnfreeze (4, TRUE);
							break;
						case ID_FILE_SAVE6:
							FreezeUnfreeze (5, TRUE);
							break;
						case ID_FILE_SAVE7:
							FreezeUnfreeze (6, TRUE);
							break;
						case ID_FILE_SAVE8:
							FreezeUnfreeze (7, TRUE);
							break;
						case ID_FILE_SAVE9:
							FreezeUnfreeze (8, TRUE);
							break;
						case ID_CHEAT_ENTER:
							RestoreGUIDisplay ();
							S9xRemoveCheats ();
							DialogBox(g_hInst, MAKEINTRESOURCE(IDD_CHEATER), hWnd, DlgCheater);
							S9xSaveCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
							S9xApplyCheats ();
							RestoreSNESDisplay ();
							break;
						case ID_CHEAT_SEARCH:
							RestoreGUIDisplay ();
							if(!cheatSearchHWND) // create and show non-modal cheat search window
							{
								cheatSearchHWND = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_CHEAT_SEARCH), hWnd, DlgCheatSearch); // non-modal/modeless
								ShowWindow(cheatSearchHWND, SW_SHOW);
							}
							else // already open so just reactivate the window
							{
								SetActiveWindow(cheatSearchHWND);
							}
							RestoreSNESDisplay ();
							break;
						case ID_CHEAT_SEARCH_MODAL:
							RestoreGUIDisplay ();
							DialogBox(g_hInst, MAKEINTRESOURCE(IDD_CHEAT_SEARCH), hWnd, DlgCheatSearch); // modal
							S9xSaveCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
							RestoreSNESDisplay ();
							break;
						case ID_CHEAT_APPLY:
							Settings.ApplyCheats = !Settings.ApplyCheats;
							if (!Settings.ApplyCheats){
								S9xRemoveCheats ();
								S9xMessage (S9X_INFO, S9X_GAME_GENIE_CODE_ERROR, CHEATS_INFO_DISABLED);
							}else{
								S9xApplyCheats ();
								bool on = false;
								extern struct SCheatData Cheat;
								for (uint32 i = 0; i < Cheat.num_cheats && !on; i++)
									if (Cheat.c [i].enabled)
										on = true;
								S9xMessage (S9X_INFO, S9X_GAME_GENIE_CODE_ERROR, on ? CHEATS_INFO_ENABLED : CHEATS_INFO_ENABLED_NONE);
							}
							break;
						case ID_OPTIONS_SETTINGS:
							RestoreGUIDisplay ();
							DialogBox(g_hInst, MAKEINTRESOURCE(IDD_EMU_SETTINGS), hWnd, DlgEmulatorProc);
							RestoreSNESDisplay ();
							break;
						case ID_HELP_ABOUT:
							RestoreGUIDisplay ();
							DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, DlgAboutProc);
							RestoreSNESDisplay ();
							break;
#ifdef DEBUGGER
						case ID_DEBUG_TRACE:
							{
								Trace ();
								break;
							}
						case ID_DEBUG_FRAME_ADVANCE:
							CPU.Flags |= FRAME_ADVANCE_FLAG;
							ICPU.FrameAdvanceCount = 1;
							Settings.Paused = FALSE;
							break;
#endif
						case IDM_7110_CACHE:
							RestoreGUIDisplay ();
							DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SPC7110_CACHE), hWnd, DlgSP7PackConfig);
							RestoreSNESDisplay ();
							break;
						case IDM_LOG_7110:
							Do7110Logging();
							break;
						case IDM_ROM_INFO:
							RestoreGUIDisplay ();
							DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ROM_INFO), hWnd, DlgInfoProc);
							RestoreSNESDisplay ();
							break;
						default:
							if ((wParam & 0xffff) >= 0xFF00)
							{
								int i = (wParam & 0xffff) - 0xFF00;
								int j = 0;
								{
									while (j < MAX_RECENT_GAMES_LIST_SIZE && j != i)
										j++;
									if (i == j)
									{
#ifdef NETPLAY_SUPPORT
										if (Settings.NetPlay && !Settings.NetPlayServer)
										{
											S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER,
												WINPROC_DISCONNECT);
											break;
										}
#endif
										if (!Settings.StopEmulation)
										{
											Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
											S9xSaveCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
										}
										Settings.StopEmulation = !LoadROM (GUI.RecentGames [i]);
										if (!Settings.StopEmulation)
										{
											bool8 loadedSRAM = Memory.LoadSRAM (S9xGetFilename (".srm", SRAM_DIR));
											if(!loadedSRAM) // help migration from earlier Snes9x versions by checking ROM directory for savestates
												Memory.LoadSRAM (S9xGetFilename (".srm", ROMFILENAME_DIR));
											S9xLoadCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
											S9xAddToRecentGames (GUI.RecentGames [i]);
											CheckDirectoryIsWritable (S9xGetFilename (".---", SNAPSHOT_DIR));
											CheckMenuStates ();
#ifdef NETPLAY_SUPPORT
											if (NPServer.SendROMImageOnConnect)
												S9xNPServerQueueSendingROMImage ();
											else
												S9xNPServerQueueSendingLoadROMRequest (Memory.ROMName);
#endif
											Settings.Paused = false;
										}
										else
										{
											sprintf (String, ERR_ROM_NOT_FOUND, GUI.RecentGames [i]);
											S9xMessage (S9X_ERROR, S9X_ROM_NOT_FOUND, String);
											S9xRemoveFromRecentGames(i);
										}
									}
								}
							}
							break;
            }
            break;

	case WM_EXITMENULOOP:
		UpdateWindow(GUI.hWnd);
		UpdateBackBuffer();
		S9xClearPause (PAUSE_MENU);
		break;

	case WM_ENTERMENULOOP:
		S9xSetPause (PAUSE_MENU);
#ifdef USE_GLIDE
		S9xGlideEnable (FALSE);
#endif
		CheckMenuStates ();

		SwitchToGDI();
		DrawMenuBar( GUI.hWnd);
		break;

	case WM_CLOSE: {
		bool maximized = GUI.window_maximized;
		ShowWindow(GUI.hWnd, SW_RESTORE);
		GUI.window_maximized = maximized;
		if (!VOODOO_MODE && !GUI.FullScreen && !GUI.EmulatedFullScreen)
			GetWindowRect (GUI.hWnd, &GUI.window_size);
	}	break;

	case WM_DESTROY:
		Memory.SaveSRAM(S9xGetFilename(".srm", SRAM_DIR));
		if(CleanUp7110)
			(*CleanUp7110)();
		GUI.hWnd = NULL;
		PostQuitMessage (0);
		return (0);
	case WM_PAINT:
        {
            PAINTSTRUCT paint;

            BeginPaint (GUI.hWnd, &paint);

			// refresh screen
			if(DirectDraw.Clipped) S9xReRefresh();

			EndPaint (GUI.hWnd, &paint);
            break;
        }
	case WM_SYSCOMMAND:
        {
            // Prevent screen saver from starting if not paused
			//kode54 says add the ! to fix the screensaver pevention.
            if (!(Settings.ForcedPause || Settings.StopEmulation ||
				(Settings.Paused && !Settings.FrameAdvance)) &&
                (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER))
                return (0);
            break;
        }
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
#ifdef USE_GLIDE
			if (VOODOO_MODE)
			{
				S9xGlideEnable (FALSE);
#if 0
				MoveWindow (GUI.hWnd, GUI.window_size.left,
					GUI.window_size.top,
					GUI.window_size.right - GUI.window_size.left,
					GUI.window_size.bottom - GUI.window_size.top,
					TRUE);
#endif
			}
#endif
			if(GUI.InactivePause)
			{
				S9xSetPause (PAUSE_INACTIVE_WINDOW);
			}
		}
		else
		{
			Settings.TurboMode = false;
///			if(GUI.InactivePause)
			{
				S9xClearPause (PAUSE_INACTIVE_WINDOW);
			}
			IPPU.ColorsChanged = TRUE;
		}
		break;
	case WM_QUERYNEWPALETTE:
		//            if (!GUI.FullScreen && GUI.ScreenDepth == 8)
		//                RealizePalette (GUI.WindowDC);
		break;
	case WM_SIZE:
		if (wParam == SIZE_MAXIMIZED)
		{
			GUI.window_maximized = true;
		}
		if (wParam == SIZE_RESTORED)
		{
			GUI.window_maximized = false;
			if(GUI.InactivePause)
			{
				S9xClearPause (PAUSE_WINDOW_ICONISED);
			}
		}
		if (wParam == SIZE_MINIMIZED || wParam == SIZE_MAXHIDE)
		{
#ifdef USE_GLIDE
			S9xGlideEnable (FALSE);
#endif
///			if(GUI.InactivePause)
			{
				S9xClearPause (PAUSE_WINDOW_ICONISED);
			}
		}
		if(GUI.outputMethod==DIRECT3D)
			Direct3D.changeRenderSize(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_MOVE:
		//if (!VOODOO_MODE && !GUI.FullScreen && !Settings.ForcedPause)
		//{
		//	GetWindowRect (GUI.hWnd, &GUI.window_size);
		//}

		break;
	case WM_ENTERSIZEMOVE:
		S9xSetPause(PAUSE_MENU);
		break;
	case WM_EXITSIZEMOVE:
		S9xClearPause(PAUSE_MENU);
		break;
	case WM_DISPLAYCHANGE:
		if (!GUI.FullScreen)
		{
			if (!VOODOO_MODE && !OPENGL_MODE &&
				(GUI.outputMethod==DIRECT3D)?Direct3D.changeRenderSize(0,0):
				DirectDraw.SetDisplayMode (GUI.Width, GUI.Height, max(GetFilterScale(GUI.Scale), GetFilterScale(GUI.ScaleHiRes)), GUI.Depth, GUI.RefreshRate,
				!GUI.FullScreen, GUI.DoubleBuffered))
			{
				S9xGraphicsDeinit();
				S9xSetWinPixelFormat ();
				S9xInitUpdate();
				S9xGraphicsInit();
			}
		}
		break;
	case WM_MOUSEMOVE:
		if(Settings.StopEmulation)
		{
			SetCursor (GUI.Arrow);
			break;
		}
		// Lo-word of lparam is xpos, hi-word is ypos
//		if (!GUI.IgnoreNextMouseMove)
		{
			//POINT p;
			//p.x = GET_X_LPARAM(lParam);
			//p.y = GET_Y_LPARAM(lParam);
			//ClientToScreen (GUI.hWnd, &p);
			if ((!Settings.ForcedPause && !Settings.StopEmulation &&
				!(Settings.Paused && !Settings.FrameAdvance)) &&
				(GUI.ControllerOption==SNES_MOUSE || GUI.ControllerOption==SNES_MOUSE_SWAPPED)
			   )
			{
				CenterCursor();
			}
			else if (GUI.ControllerOption==SNES_SUPERSCOPE || GUI.ControllerOption==SNES_JUSTIFIER || GUI.ControllerOption==SNES_JUSTIFIER_2)
			{
				RECT size;
				GetClientRect (GUI.hWnd, &size);
				if(!(GUI.Scale)&&!(GUI.Stretch))
				{
					int x,y, startx, starty;
					x=GET_X_LPARAM(lParam);
					y=GET_Y_LPARAM(lParam);

//					int theight;
//					(IPPU.RenderedScreenHeight> 256)? theight= SNES_HEIGHT_EXTENDED<<1: theight = SNES_HEIGHT_EXTENDED;
					int theight = GUI.HeightExtend ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT;
					if(IPPU.RenderedScreenHeight > SNES_HEIGHT_EXTENDED) theight <<= 1;

					startx= size.right-IPPU.RenderedScreenWidth;
					startx/=2;
					starty= size.bottom-theight;
					starty/=2;

					if(x<startx)
						GUI.MouseX=0;
					else if(x>(startx+IPPU.RenderedScreenWidth))
						GUI.MouseX=IPPU.RenderedScreenWidth;
					else GUI.MouseX=x-startx;

					if(y<starty)
						GUI.MouseY=0;
					else if(y>(starty+theight))
						GUI.MouseY=theight;
					else GUI.MouseY=y-starty;
				}
				else if(!(GUI.Stretch)&&!VOODOO_MODE && !OPENGL_MODE)
				{
					int x,y, startx, starty, sizex, sizey;
					x=GET_X_LPARAM(lParam);
					y=GET_Y_LPARAM(lParam);

					if (IPPU.RenderedScreenWidth>256)
						sizex=IPPU.RenderedScreenWidth;
					else sizex=IPPU.RenderedScreenWidth*2;

					int theight = GUI.HeightExtend ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT;
					sizey = (IPPU.RenderedScreenHeight > SNES_HEIGHT_EXTENDED) ? theight : (theight << 1);

					startx= size.right-sizex;
					startx/=2;
					starty= size.bottom-sizey;
					starty/=2;
					if(x<startx)
						GUI.MouseX=0;
					else if(x>(startx+sizex))
						GUI.MouseX=sizex;
					else GUI.MouseX=x-startx;

					if(y<starty)
						GUI.MouseY=0;
					else if(y>(starty+sizey))
						GUI.MouseY=sizey;
					else GUI.MouseY=y-starty;

					GUI.MouseX=(GUI.MouseX*IPPU.RenderedScreenWidth)/sizex;
					GUI.MouseY=(GUI.MouseY*IPPU.RenderedScreenHeight)/sizey;
				}
				else
				{
					int sizex = IPPU.RenderedScreenWidth;
					int sizey = GUI.HeightExtend ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT;
					sizey = (IPPU.RenderedScreenHeight > SNES_HEIGHT_EXTENDED) ? (sizey << 1) : sizey;
					int width = size.right, height = size.bottom, xdiff = 0, ydiff = 0;
					if(GUI.AspectRatio)
					{
						if(width > sizex*height/sizey)
						{
							xdiff = width - sizex*height/sizey;
							width -= xdiff;
							xdiff >>= 1;
						}
						else if(height > sizey*width/sizex)
						{
							ydiff = height - sizey*width/sizex;
							height -= ydiff;
							ydiff >>= 1;
						}
					}
					GUI.MouseX=(GET_X_LPARAM(lParam)-xdiff)*sizex/width;
					GUI.MouseY=(GET_Y_LPARAM(lParam)-ydiff)*sizey/height;
				}
			}
			else
			{
//				GUI.MouseX = p.x;
//				GUI.MouseY = p.y;
			}
		}
//		else
//			GUI.IgnoreNextMouseMove = false;

		if(!GUI.IgnoreNextMouseMove)
			S9xMouseOn ();
		else
			GUI.IgnoreNextMouseMove = false;
		return 0;
	case WM_LBUTTONDOWN:
		S9xMouseOn ();
		GUI.MouseButtons |= 1;
		break;
	case WM_LBUTTONUP:
		S9xMouseOn ();
		GUI.MouseButtons &= ~1;
		break;
	case WM_RBUTTONDOWN:
		S9xMouseOn ();
		GUI.MouseButtons |= 2;
		break;
	case WM_RBUTTONUP:
		S9xMouseOn ();
		GUI.MouseButtons &= ~2;
		if(GUI.ControllerOption==SNES_JUSTIFIER || GUI.ControllerOption==SNES_JUSTIFIER_2)
		{
			RECT size;
			GetClientRect (GUI.hWnd, &size);
			GUI.MouseButtons&=~1;
			GUI.MouseX=(IPPU.RenderedScreenWidth*(lParam & 0xffff))/(size.right-size.left);
			GUI.MouseY=(((lParam >> 16) & 0xffff)*IPPU.RenderedScreenHeight)/(size.bottom-size.top);
		}
		break;
	case WM_MBUTTONDOWN:
		S9xMouseOn ();
		GUI.MouseButtons |= 4;
		break;
	case WM_MBUTTONUP:
		S9xMouseOn ();
		GUI.MouseButtons &= ~4;
		break;
#ifdef NETPLAY_SUPPORT
	case WM_USER + 3:
		NetPlay.Answer = S9xLoadROMImage ((const char *) lParam);
		SetEvent (NetPlay.ReplyEvent);
		break;
	case WM_USER + 2:
		S9xMessage (0, 0, NetPlay.WarningMsg);
		break;
	case WM_USER + 1:
		RestoreGUIDisplay ();
		S9xRestoreWindowTitle ();
		MessageBox (GUI.hWnd, NetPlay.ErrorMsg,
			TEXT(SNES9X_NP_ERROR), MB_OK | MB_ICONSTOP);
		RestoreSNESDisplay ();
		break;
	case WM_USER:
		if (NetPlay.ActionMsg [0] == 0)
			S9xRestoreWindowTitle ();
		else
		{
			TCHAR buf [NP_MAX_ACTION_LEN + 10];

			sprintf (buf, TEXT("%s %3d%%"), NetPlay.ActionMsg, (int) lParam);
			SetWindowText (GUI.hWnd, buf);
		}
#if 0
		if ((int) lParam >= 0)
		{
			RestoreGUIDisplay ();
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NETPLAYPROGRESS), hWnd, DlgNPProgress);
		}
		else
		{
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NETPLAYPROGRESS), hWnd, DlgNPProgress);
			RestoreSNESDisplay ();
		}
#endif
		break;
#endif
    }
    return DefWindowProc (hWnd, uMsg, wParam, lParam);
}

*/
 



void S9xExtraUsage ()
{
}

/*
// handles joystick hotkey presses
VOID CALLBACK HotkeyTimer( UINT idEvent, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
//	static int lastTime = timeGetTime();
//	if(timeGetTime() - lastTime > 5)
	{
		bool S9xGetState (WORD KeyIdent);

		if(GUI.JoystickHotkeys)
		{
			static int counter = 0;
			static uint32 joyState [256];
			for(int i = 0 ; i < 255 ; i++)
			{
				if(counter%2 && !joyState[i])
					continue;

				bool active = !S9xGetState(0x8000|i);
				if(active)
				{
					if(joyState[i] < ULONG_MAX) // 0xffffffffUL
						joyState[i]++;
					if(joyState[i] == 1 || joyState[i] >= 12)
						PostMessage(GUI.hWnd, WM_CUSTKEYDOWN, (WPARAM)(0x8000|i),(LPARAM)(NULL));
				}
				else
					if(joyState[i])
					{
						joyState[i] = 0;
						PostMessage(GUI.hWnd, WM_CUSTKEYUP, (WPARAM)(0x8000|i),(LPARAM)(NULL));
					}
			}
			counter++;
		}
		if(GUI.BackgroundKeyHotkeys)
		{
			static int counter = 0;
			static uint32 joyState [256];
			for(int i = 0 ; i < 255 ; i++)
			{
				if(counter%2 && !joyState[i])
					continue;

				bool active = !S9xGetState(i);
				if(active)
				{
					if(joyState[i] < ULONG_MAX) // 0xffffffffUL
						joyState[i]++;
					if(joyState[i] == 1 || joyState[i] >= 12)
						PostMessage(GUI.hWnd, WM_CUSTKEYDOWN, (WPARAM)(i),(LPARAM)(NULL));
				}
				else
					joyState[i] = 0;
			}
			counter++;
		}
//		lastTime = timeGetTime();
	}
}
*/
void CALLBACK FrameTimer()
{
	// QueryPerformanceCounter is unreliable on newfangled frequency-switching computers,
	// yet is absolutely necessary for best performance on somewhat older computers (even ones that are capable of frequency switching but don't do it very often).
	// Thus, we keep two timers and use the QueryPerformanceCounter one unless the other (more accurate but less precise)
	// one differs from it by more than a few milliseconds.

    QueryPerformanceCounter((LARGE_INTEGER*)&PCEnd);
	PCEndTicks = GetTickCount()*1000;

	const __int64 PCElapsedPrecise = PCEnd - PCStart;
	const __int64 PCElapsedAccurate = (__int64)(PCEndTicks - PCStartTicks) * PCBase / 1000000;
	const bool useTicksTimer = (abs((int)(PCElapsedPrecise - PCElapsedAccurate)) > (PCBase >> 7)); // if > 7.8 ms difference, settle for accuracy at the sacrifice of precision

    while ((!useTicksTimer && (PCEnd      - PCStart     ) >= PCFrameTime) ||
		   ( useTicksTimer && (PCEndTicks - PCStartTicks) >= PCFrameTime * 1000000 / PCBase))
	{
        if (GUI.FrameCount == GUI.LastFrameCount)
            GUI.IdleCount++;
        else
        {
            GUI.IdleCount = 0;
            GUI.LastFrameCount = GUI.FrameCount;
        }

#ifdef NETPLAY_SUPPORT
		//    if (Settings.NetPlay && !Settings.NetPlayServer)
		//        return;
        if (Settings.NetPlay && !Settings.NetPlayServer)
            return;

		//-    if (Settings.NetPlayServer)
		//-    {
		//-        if (Settings.Paused || Settings.StopEmulation || Settings.ForcedPause)
        if (Settings.NetPlayServer)
		{
			//-            WaitForSingleObject (GUI.ServerTimerSemaphore, 0);
            if ((Settings.Paused && !Settings.FrameAdvance) || Settings.StopEmulation || Settings.ForcedPause)
            {
                WaitForSingleObject (GUI.ServerTimerSemaphore, 0);
                return;
            }
            ReleaseSemaphore (GUI.ServerTimerSemaphore, 1, NULL);

            if (Settings.NetPlay)
                return;
        }
        else
#endif
		{
			if (Settings.SkipFrames != AUTO_FRAMERATE || Settings.TurboMode ||
				(Settings.Paused /*&& !Settings.FrameAdvance*/) || Settings.StopEmulation || Settings.ForcedPause)
			{
				WaitForSingleObject (GUI.FrameTimerSemaphore, 0);
				PCStart = PCEnd;
				PCStartTicks = PCEndTicks;
				 
			}
			//        ReleaseSemaphore (GUI.ServerTimerSemaphore, 1, NULL);
			ReleaseSemaphore (GUI.FrameTimerSemaphore, 1, NULL);

			//        if (Settings.NetPlay)
			//            return;
			//    }
			//    else
			//#endif
			//    if (Settings.SkipFrames != AUTO_FRAMERATE || Settings.TurboMode ||
			//        Settings.Paused || Settings.StopEmulation || Settings.ForcedPause)
			//    {
			//        WaitForSingleObject (GUI.FrameTimerSemaphore, 0);
			//        return;
			//    }
			//    ReleaseSemaphore (GUI.FrameTimerSemaphore, 1, NULL);
			PCStart += PCFrameTime;
			PCStartTicks += (DWORD)(PCFrameTime * 1000000 / PCBase);
		}
	}
}

static void EnsureInputDisplayUpdated()
{
	if(GUI.FrameAdvanceJustPressed==1 && Settings.Paused && Settings.DisplayPressedKeys==2 && GUI.ControllerOption != SNES_JOYPAD && GUI.ControllerOption != SNES_MULTIPLAYER5 && GUI.ControllerOption != SNES_MULTIPLAYER8)
		S9xReRefresh();
}

// for "frame advance skips non-input frames" feature
void S9xOnSNESPadRead()
{
	/*if(!GUI.FASkipsNonInput)
		return;

	if(prevPadReadFrame != IPPU.TotalEmulatedFrames) // we want <= 1 calls per frame
	{
		prevPadReadFrame = IPPU.TotalEmulatedFrames;

		if(Settings.FrameAdvance && Settings.Paused && !skipNextFrameStop)
		{
			Settings.FrameAdvance = false;
			ICPU.SavedAtOp = TRUE;

			EnsureInputDisplayUpdated();

			// wait until either unpause or next frame advance
			// note: using GUI.hWnd instead of NULL for PeekMessage/GetMessage breaks some non-modal dialogs
			MSG msg;
			while (Settings.StopEmulation || (Settings.Paused && !Settings.FrameAdvance) ||
				Settings.ForcedPause ||
				PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!GetMessage (&msg, NULL, 0, 0))
				{
					PostMessage(GUI.hWnd, WM_QUIT, 0,0);
					return;
				}

				if (!TranslateAccelerator (GUI.hWnd, GUI.Accelerators, &msg))
				{
					TranslateMessage (&msg);
					DispatchMessage (&msg);
				}
			}

			ICPU.SavedAtOp = !ICPU.SavedAtOp;
		}
		else
		{
			skipNextFrameStop = false;
		}
	} */
}


enum
{
	k_HD = 0x80000000,

	k_JP = 0x01000000,
	k_MO = 0x02000000,
	k_SS = 0x04000000,
	k_LG = 0x08000000,

	k_BT = 0x00100000,
	k_PT = 0x00200000,
	k_PS = 0x00400000,

	k_C1 = 0x00000100,
	k_C2 = 0x00000200,
	k_C3 = 0x00000400,
	k_C4 = 0x00000800,
	k_C5 = 0x00001000,
	k_C6 = 0x00002000,
	k_C7 = 0x00004000,
	k_C8 = 0x00008000
};

enum
{
	kWinCMapPad1PX            = k_HD | k_BT | k_JP | k_C1,
	kWinCMapPad1PA,
	kWinCMapPad1PB,
	kWinCMapPad1PY,
	kWinCMapPad1PL,
	kWinCMapPad1PR,
	kWinCMapPad1PSelect,
	kWinCMapPad1PStart,
	kWinCMapPad1PUp,
	kWinCMapPad1PDown,
	kWinCMapPad1PLeft,
	kWinCMapPad1PRight,

	kWinCMapPad2PX            = k_HD | k_BT | k_JP | k_C2,
	kWinCMapPad2PA,
	kWinCMapPad2PB,
	kWinCMapPad2PY,
	kWinCMapPad2PL,
	kWinCMapPad2PR,
	kWinCMapPad2PSelect,
	kWinCMapPad2PStart,
	kWinCMapPad2PUp,
	kWinCMapPad2PDown,
	kWinCMapPad2PLeft,
	kWinCMapPad2PRight,

	kWinCMapPad3PX            = k_HD | k_BT | k_JP | k_C3,
	kWinCMapPad3PA,
	kWinCMapPad3PB,
	kWinCMapPad3PY,
	kWinCMapPad3PL,
	kWinCMapPad3PR,
	kWinCMapPad3PSelect,
	kWinCMapPad3PStart,
	kWinCMapPad3PUp,
	kWinCMapPad3PDown,
	kWinCMapPad3PLeft,
	kWinCMapPad3PRight,

	kWinCMapPad4PX            = k_HD | k_BT | k_JP | k_C4,
	kWinCMapPad4PA,
	kWinCMapPad4PB,
	kWinCMapPad4PY,
	kWinCMapPad4PL,
	kWinCMapPad4PR,
	kWinCMapPad4PSelect,
	kWinCMapPad4PStart,
	kWinCMapPad4PUp,
	kWinCMapPad4PDown,
	kWinCMapPad4PLeft,
	kWinCMapPad4PRight,

	kWinCMapPad5PX            = k_HD | k_BT | k_JP | k_C5,
	kWinCMapPad5PA,
	kWinCMapPad5PB,
	kWinCMapPad5PY,
	kWinCMapPad5PL,
	kWinCMapPad5PR,
	kWinCMapPad5PSelect,
	kWinCMapPad5PStart,
	kWinCMapPad5PUp,
	kWinCMapPad5PDown,
	kWinCMapPad5PLeft,
	kWinCMapPad5PRight,

	kWinCMapPad6PX            = k_HD | k_BT | k_JP | k_C6,
	kWinCMapPad6PA,
	kWinCMapPad6PB,
	kWinCMapPad6PY,
	kWinCMapPad6PL,
	kWinCMapPad6PR,
	kWinCMapPad6PSelect,
	kWinCMapPad6PStart,
	kWinCMapPad6PUp,
	kWinCMapPad6PDown,
	kWinCMapPad6PLeft,
	kWinCMapPad6PRight,

	kWinCMapPad7PX            = k_HD | k_BT | k_JP | k_C7,
	kWinCMapPad7PA,
	kWinCMapPad7PB,
	kWinCMapPad7PY,
	kWinCMapPad7PL,
	kWinCMapPad7PR,
	kWinCMapPad7PSelect,
	kWinCMapPad7PStart,
	kWinCMapPad7PUp,
	kWinCMapPad7PDown,
	kWinCMapPad7PLeft,
	kWinCMapPad7PRight,

	kWinCMapPad8PX            = k_HD | k_BT | k_JP | k_C8,
	kWinCMapPad8PA,
	kWinCMapPad8PB,
	kWinCMapPad8PY,
	kWinCMapPad8PL,
	kWinCMapPad8PR,
	kWinCMapPad8PSelect,
	kWinCMapPad8PStart,
	kWinCMapPad8PUp,
	kWinCMapPad8PDown,
	kWinCMapPad8PLeft,
	kWinCMapPad8PRight,

	kWinCMapMouse1PL          = k_HD | k_BT | k_MO | k_C1,
	kWinCMapMouse1PR,
	kWinCMapMouse2PL          = k_HD | k_BT | k_MO | k_C2,
	kWinCMapMouse2PR,

	kWinCMapScopeOffscreen    = k_HD | k_BT | k_SS | k_C1,
	kWinCMapScopeFire,
	kWinCMapScopeCursor,
	kWinCMapScopeTurbo,
	kWinCMapScopePause,

	kWinCMapLGun1Offscreen    = k_HD | k_BT | k_LG | k_C1,
	kWinCMapLGun1Trigger,
	kWinCMapLGun1Start,
	kWinCMapLGun2Offscreen    = k_HD | k_BT | k_LG | k_C2,
	kWinCMapLGun2Trigger,
	kWinCMapLGun2Start,

	kWinCMapMouse1Pointer     = k_HD | k_PT | k_MO | k_C1,
	kWinCMapMouse2Pointer     = k_HD | k_PT | k_MO | k_C2,
	kWinCMapSuperscopePointer = k_HD | k_PT | k_SS | k_C1,
	kWinCMapJustifier1Pointer = k_HD | k_PT | k_LG | k_C1,

	kWinCMapPseudoPtrBase     = k_HD | k_PS | k_LG | k_C2	// for Justifier 2P
};



#define	ASSIGN_BUTTONf(n, s)	S9xMapButton (n, cmd = S9xGetCommandT(s), false)
#define	ASSIGN_BUTTONt(n, s)	S9xMapButton (n, cmd = S9xGetCommandT(s), true)
#define	ASSIGN_POINTRf(n, s)	S9xMapPointer(n, cmd = S9xGetCommandT(s), false)
#define	ASSIGN_POINTRt(n, s)	S9xMapPointer(n, cmd = S9xGetCommandT(s), true)

#define KeyIsPressed(km, k)		(1 & (((unsigned char *) km) [(k) >> 3] >> ((k) & 7)))

void S9xSetupDefaultKeymap(void)
{
	s9xcommand_t	cmd;

	ASSIGN_BUTTONf(kWinCMapPad1PX,         "Joypad1 X");
	ASSIGN_BUTTONf(kWinCMapPad1PA,         "Joypad1 A");
	ASSIGN_BUTTONf(kWinCMapPad1PB,         "Joypad1 B");
	ASSIGN_BUTTONf(kWinCMapPad1PY,         "Joypad1 Y");
	ASSIGN_BUTTONf(kWinCMapPad1PL,         "Joypad1 L");
	ASSIGN_BUTTONf(kWinCMapPad1PR,         "Joypad1 R");
	ASSIGN_BUTTONf(kWinCMapPad1PSelect,    "Joypad1 Select");
	ASSIGN_BUTTONf(kWinCMapPad1PStart,     "Joypad1 Start");
	ASSIGN_BUTTONf(kWinCMapPad1PUp,        "Joypad1 Up");
	ASSIGN_BUTTONf(kWinCMapPad1PDown,      "Joypad1 Down");
	ASSIGN_BUTTONf(kWinCMapPad1PLeft,      "Joypad1 Left");
	ASSIGN_BUTTONf(kWinCMapPad1PRight,     "Joypad1 Right");

	ASSIGN_BUTTONf(kWinCMapPad2PX,         "Joypad2 X");
	ASSIGN_BUTTONf(kWinCMapPad2PA,         "Joypad2 A");
	ASSIGN_BUTTONf(kWinCMapPad2PB,         "Joypad2 B");
	ASSIGN_BUTTONf(kWinCMapPad2PY,         "Joypad2 Y");
	ASSIGN_BUTTONf(kWinCMapPad2PL,         "Joypad2 L");
	ASSIGN_BUTTONf(kWinCMapPad2PR,         "Joypad2 R");
	ASSIGN_BUTTONf(kWinCMapPad2PSelect,    "Joypad2 Select");
	ASSIGN_BUTTONf(kWinCMapPad2PStart,     "Joypad2 Start");
	ASSIGN_BUTTONf(kWinCMapPad2PUp,        "Joypad2 Up");
	ASSIGN_BUTTONf(kWinCMapPad2PDown,      "Joypad2 Down");
	ASSIGN_BUTTONf(kWinCMapPad2PLeft,      "Joypad2 Left");
	ASSIGN_BUTTONf(kWinCMapPad2PRight,     "Joypad2 Right");

	ASSIGN_BUTTONf(kWinCMapPad3PX,         "Joypad3 X");
	ASSIGN_BUTTONf(kWinCMapPad3PA,         "Joypad3 A");
	ASSIGN_BUTTONf(kWinCMapPad3PB,         "Joypad3 B");
	ASSIGN_BUTTONf(kWinCMapPad3PY,         "Joypad3 Y");
	ASSIGN_BUTTONf(kWinCMapPad3PL,         "Joypad3 L");
	ASSIGN_BUTTONf(kWinCMapPad3PR,         "Joypad3 R");
	ASSIGN_BUTTONf(kWinCMapPad3PSelect,    "Joypad3 Select");
	ASSIGN_BUTTONf(kWinCMapPad3PStart,     "Joypad3 Start");
	ASSIGN_BUTTONf(kWinCMapPad3PUp,        "Joypad3 Up");
	ASSIGN_BUTTONf(kWinCMapPad3PDown,      "Joypad3 Down");
	ASSIGN_BUTTONf(kWinCMapPad3PLeft,      "Joypad3 Left");
	ASSIGN_BUTTONf(kWinCMapPad3PRight,     "Joypad3 Right");

	ASSIGN_BUTTONf(kWinCMapPad4PX,         "Joypad4 X");
	ASSIGN_BUTTONf(kWinCMapPad4PA,         "Joypad4 A");
	ASSIGN_BUTTONf(kWinCMapPad4PB,         "Joypad4 B");
	ASSIGN_BUTTONf(kWinCMapPad4PY,         "Joypad4 Y");
	ASSIGN_BUTTONf(kWinCMapPad4PL,         "Joypad4 L");
	ASSIGN_BUTTONf(kWinCMapPad4PR,         "Joypad4 R");
	ASSIGN_BUTTONf(kWinCMapPad4PSelect,    "Joypad4 Select");
	ASSIGN_BUTTONf(kWinCMapPad4PStart,     "Joypad4 Start");
	ASSIGN_BUTTONf(kWinCMapPad4PUp,        "Joypad4 Up");
	ASSIGN_BUTTONf(kWinCMapPad4PDown,      "Joypad4 Down");
	ASSIGN_BUTTONf(kWinCMapPad4PLeft,      "Joypad4 Left");
	ASSIGN_BUTTONf(kWinCMapPad4PRight,     "Joypad4 Right");

	ASSIGN_BUTTONf(kWinCMapPad5PX,         "Joypad5 X");
	ASSIGN_BUTTONf(kWinCMapPad5PA,         "Joypad5 A");
	ASSIGN_BUTTONf(kWinCMapPad5PB,         "Joypad5 B");
	ASSIGN_BUTTONf(kWinCMapPad5PY,         "Joypad5 Y");
	ASSIGN_BUTTONf(kWinCMapPad5PL,         "Joypad5 L");
	ASSIGN_BUTTONf(kWinCMapPad5PR,         "Joypad5 R");
	ASSIGN_BUTTONf(kWinCMapPad5PSelect,    "Joypad5 Select");
	ASSIGN_BUTTONf(kWinCMapPad5PStart,     "Joypad5 Start");
	ASSIGN_BUTTONf(kWinCMapPad5PUp,        "Joypad5 Up");
	ASSIGN_BUTTONf(kWinCMapPad5PDown,      "Joypad5 Down");
	ASSIGN_BUTTONf(kWinCMapPad5PLeft,      "Joypad5 Left");
	ASSIGN_BUTTONf(kWinCMapPad5PRight,     "Joypad5 Right");

	ASSIGN_BUTTONf(kWinCMapPad6PX,         "Joypad6 X");
	ASSIGN_BUTTONf(kWinCMapPad6PA,         "Joypad6 A");
	ASSIGN_BUTTONf(kWinCMapPad6PB,         "Joypad6 B");
	ASSIGN_BUTTONf(kWinCMapPad6PY,         "Joypad6 Y");
	ASSIGN_BUTTONf(kWinCMapPad6PL,         "Joypad6 L");
	ASSIGN_BUTTONf(kWinCMapPad6PR,         "Joypad6 R");
	ASSIGN_BUTTONf(kWinCMapPad6PSelect,    "Joypad6 Select");
	ASSIGN_BUTTONf(kWinCMapPad6PStart,     "Joypad6 Start");
	ASSIGN_BUTTONf(kWinCMapPad6PUp,        "Joypad6 Up");
	ASSIGN_BUTTONf(kWinCMapPad6PDown,      "Joypad6 Down");
	ASSIGN_BUTTONf(kWinCMapPad6PLeft,      "Joypad6 Left");
	ASSIGN_BUTTONf(kWinCMapPad6PRight,     "Joypad6 Right");

	ASSIGN_BUTTONf(kWinCMapPad7PX,         "Joypad7 X");
	ASSIGN_BUTTONf(kWinCMapPad7PA,         "Joypad7 A");
	ASSIGN_BUTTONf(kWinCMapPad7PB,         "Joypad7 B");
	ASSIGN_BUTTONf(kWinCMapPad7PY,         "Joypad7 Y");
	ASSIGN_BUTTONf(kWinCMapPad7PL,         "Joypad7 L");
	ASSIGN_BUTTONf(kWinCMapPad7PR,         "Joypad7 R");
	ASSIGN_BUTTONf(kWinCMapPad7PSelect,    "Joypad7 Select");
	ASSIGN_BUTTONf(kWinCMapPad7PStart,     "Joypad7 Start");
	ASSIGN_BUTTONf(kWinCMapPad7PUp,        "Joypad7 Up");
	ASSIGN_BUTTONf(kWinCMapPad7PDown,      "Joypad7 Down");
	ASSIGN_BUTTONf(kWinCMapPad7PLeft,      "Joypad7 Left");
	ASSIGN_BUTTONf(kWinCMapPad7PRight,     "Joypad7 Right");

	ASSIGN_BUTTONf(kWinCMapPad8PX,         "Joypad8 X");
	ASSIGN_BUTTONf(kWinCMapPad8PA,         "Joypad8 A");
	ASSIGN_BUTTONf(kWinCMapPad8PB,         "Joypad8 B");
	ASSIGN_BUTTONf(kWinCMapPad8PY,         "Joypad8 Y");
	ASSIGN_BUTTONf(kWinCMapPad8PL,         "Joypad8 L");
	ASSIGN_BUTTONf(kWinCMapPad8PR,         "Joypad8 R");
	ASSIGN_BUTTONf(kWinCMapPad8PSelect,    "Joypad8 Select");
	ASSIGN_BUTTONf(kWinCMapPad8PStart,     "Joypad8 Start");
	ASSIGN_BUTTONf(kWinCMapPad8PUp,        "Joypad8 Up");
	ASSIGN_BUTTONf(kWinCMapPad8PDown,      "Joypad8 Down");
	ASSIGN_BUTTONf(kWinCMapPad8PLeft,      "Joypad8 Left");
	ASSIGN_BUTTONf(kWinCMapPad8PRight,     "Joypad8 Right");

	ASSIGN_BUTTONt(kWinCMapMouse1PL,       "Mouse1 L");
	ASSIGN_BUTTONt(kWinCMapMouse1PR,       "Mouse1 R");
	ASSIGN_BUTTONt(kWinCMapMouse2PL,       "Mouse2 L");
	ASSIGN_BUTTONt(kWinCMapMouse2PR,       "Mouse2 R");

	ASSIGN_BUTTONt(kWinCMapScopeOffscreen, "Superscope AimOffscreen");
	ASSIGN_BUTTONt(kWinCMapScopeFire,      "Superscope Fire");
	ASSIGN_BUTTONt(kWinCMapScopeCursor,    "Superscope Cursor");
	ASSIGN_BUTTONt(kWinCMapScopeTurbo,     "Superscope ToggleTurbo");
	ASSIGN_BUTTONt(kWinCMapScopePause,     "Superscope Pause");

	ASSIGN_BUTTONt(kWinCMapLGun1Offscreen, "Justifier1 AimOffscreen");
	ASSIGN_BUTTONt(kWinCMapLGun1Trigger,   "Justifier1 Trigger");
	ASSIGN_BUTTONt(kWinCMapLGun1Start,     "Justifier1 Start");
	ASSIGN_BUTTONt(kWinCMapLGun2Offscreen, "Justifier2 AimOffscreen");
	ASSIGN_BUTTONt(kWinCMapLGun2Trigger,   "Justifier2 Trigger");
	ASSIGN_BUTTONt(kWinCMapLGun2Start,     "Justifier2 Start");

	ASSIGN_POINTRt(kWinCMapMouse1Pointer,     "Pointer Mouse1");
	ASSIGN_POINTRt(kWinCMapMouse2Pointer,     "Pointer Mouse2");
	ASSIGN_POINTRt(kWinCMapSuperscopePointer, "Pointer Superscope");
	ASSIGN_POINTRt(kWinCMapJustifier1Pointer, "Pointer Justifier1");

	ASSIGN_POINTRf(PseudoPointerBase,         "Pointer Justifier2");
	ASSIGN_BUTTONf(kWinCMapPseudoPtrBase + 0, "ButtonToPointer 1u Med");
	ASSIGN_BUTTONf(kWinCMapPseudoPtrBase + 1, "ButtonToPointer 1d Med");
	ASSIGN_BUTTONf(kWinCMapPseudoPtrBase + 2, "ButtonToPointer 1l Med");
	ASSIGN_BUTTONf(kWinCMapPseudoPtrBase + 3, "ButtonToPointer 1r Med");
}

void ControlPadFlagsToS9xReportButtons(int n, uint32 p)
{
	uint32	base = k_HD | k_BT | k_JP | (0x100 << n);

	S9xReportButton(base +  0, (p & 0x0040) != 0);
	S9xReportButton(base +  1, (p & 0x0080) != 0);
	S9xReportButton(base +  2, (p & 0x8000) != 0);
	S9xReportButton(base +  3, (p & 0x4000) != 0);
	S9xReportButton(base +  4, (p & 0x0020) != 0);
	S9xReportButton(base +  5, (p & 0x0010) != 0);
	S9xReportButton(base +  6, (p & 0x2000) != 0);
	S9xReportButton(base +  7, (p & 0x1000) != 0);
	S9xReportButton(base +  8, (p & 0x0800) != 0);
	S9xReportButton(base +  9, (p & 0x0400) != 0);
	S9xReportButton(base + 10, (p & 0x0200) != 0);
	S9xReportButton(base + 11, (p & 0x0100) != 0);
}

void ControlPadFlagsToS9xPseudoPointer(uint32 p)
{
	// prevent screwiness caused by trying to move the pointer left+right or up+down
	if((p & 0x0c00) == 0x0c00) p &= ~0x0c00;
	if((p & 0x0300) == 0x0300) p &= ~0x0300;

	// checks added to prevent a lack of right/down movement from breaking left/up movement
	if(!(p & 0x0400))
		S9xReportButton(kWinCMapPseudoPtrBase + 0, (p & 0x0800) != 0);
	if(!(p & 0x0800))
		S9xReportButton(kWinCMapPseudoPtrBase + 1, (p & 0x0400) != 0);
	if(!(p & 0x0100))
		S9xReportButton(kWinCMapPseudoPtrBase + 2, (p & 0x0200) != 0);
	if(!(p & 0x0200))
		S9xReportButton(kWinCMapPseudoPtrBase + 3, (p & 0x0100) != 0);
}

void ExitInputThread()
{
 
/*	if(InputThread) {
		exitInputThread = true;
		SetEvent(eventHandle);							// signal the thread in case it is waiting
		WaitForSingleObject(InputThread,INFINITE);		// wait for the thread to exit gracefully
		exitInputThread = false;
		CloseHandle(InputThread);
		InputThread = NULL;
	}
	if(eventHandle) {
		CloseHandle(eventHandle);
		eventHandle = NULL;
	}*/
    
}

static void ProcessInputs(void)
{

	extern void S9xWinScanJoypads ();	
  
	 
	S9xWinScanJoypads ();

	extern uint32 joypads [8];
	for(int i = 0 ; i < 8 ; i++)
		ControlPadFlagsToS9xReportButtons(i, joypads[i]);

 
}

unsigned int __stdcall ProcessInput (LPVOID lpParameter)
{

	/*extern void S9xWinScanJoypads ();	
#ifdef NETPLAY_SUPPORT
    if (!Settings.NetPlay)
#endif

	XSetThreadProcessor( GetCurrentThread(), 4 );
	HANDLE inputThread =(HANDLE)lpParameter;
	while(1) 
	{
		WaitForSingleObject(inputThread,40);

		if (!wasInXUI)
		{
			S9xWinScanJoypads ();

			extern uint32 joypads [8];
			for(int i = 0 ; i < 8 ; i++)
				ControlPadFlagsToS9xReportButtons(i, joypads[i]);

			if (GUI.ControllerOption==SNES_JUSTIFIER_2)
				ControlPadFlagsToS9xPseudoPointer(joypads[1]);

			if (exitInputThread)
			{
				_endthreadex(0);

			}
		}
 
	}*/

	return 0;
}

static void WinDisplayString (const char *string, int linesFromBottom, int pixelsFromLeft, bool allowWrap);


void XboxSetDefaultValues ()
{
	// TODO: delete the parts that are already covered by the default values in WinRegisterConfigItems

 
	GUI.ControllerOption = SNES_JOYPAD;
	GUI.ValidControllerOptions = 0xFFFF;
	GUI.IgnoreNextMouseMove	= false;

	GUI.HideMenu = false;
	GUI.window_size.left = 0;
	GUI.window_size.right =	524;
	GUI.window_size.top	= 0;
	GUI.window_size.bottom = 524;
	GUI.Width =	640;
	GUI.Height = 480;
	GUI.Depth =	16;
	GUI.Scale =	FILTER_HQ2X;
	GUI.NextScale =	FILTER_HQ2X;
	GUI.ScaleHiRes =	FILTER_HQ2X;
	GUI.NextScaleHiRes =	FILTER_HQ2X;
	GUI.DoubleBuffered = false;
	GUI.FullScreen = true;
	GUI.Stretch	= true;
	GUI.FlipCounter	= 0;
	GUI.NumFlipFrames =	1;
	GUI.VideoMemory	= true;
	GUI.ScreenCleared =	true;
	GUI.LockDirectories = false;
	GUI.window_maximized = false;
	GUI.AspectRatio = true;
	GUI.AspectWidth = 256;
	 
	// ROM Options
	XMemSet (&Settings, 0, sizeof (Settings));

	Settings.ForceLoROM	= false;
	Settings.ForceInterleaved =	false;

	Settings.ForceNotInterleaved = false;
	Settings.ForceInterleaved =	false;
	Settings.ForceInterleaved2 = false;

	Settings.ForcePAL =	false;
	Settings.ForceNTSC = false;
	Settings.ForceHeader = false;
	Settings.ForceNoHeader = false;

	// Sound options
	Settings.APUEnabled = TRUE;	 
	Settings.SoundSync = TRUE;
	Settings.InterpolatedSound = TRUE;
	Settings.SoundEnvelopeHeightReading	= TRUE;
	Settings.DisableSoundEcho =	FALSE;
	Settings.DisableMasterVolume = FALSE;
	Settings.Mute =	FALSE;
	Settings.SoundSkipMethod = 0;
	Settings.SoundPlaybackRate = 32000;
	Settings.SixteenBitSound = TRUE;
	Settings.Stereo	= TRUE;
	Settings.AltSampleDecode = FALSE;
	Settings.ReverseStereo = FALSE;
	Settings.SoundDriver = WIN_SNES9X_DIRECT_SOUND_DRIVER;
	Settings.SoundBufferSize = 4;
	Settings.SoundMixInterval =	10;
	Settings.DisableSampleCaching=TRUE;
	GUI.SoundChannelEnable=255;
	GUI.FAMute	= FALSE;

	// Tracing options
	Settings.TraceDMA =	false;
	Settings.TraceHDMA = false;
	Settings.TraceVRAM = false;
	Settings.TraceUnknownRegisters = false;
	Settings.TraceDSP =	false;

	// Joystick	options
//	Settings.SwapJoypads = false;
	Settings.JoystickEnabled = false;

	// ROM timing options (see also	H_Max above)
	Settings.PAL = false;
	Settings.FrameTimePAL =	20000;
	Settings.FrameTimeNTSC = 16667;
	Settings.FrameTime = 16667;
 
	// CPU options
	Settings.HDMATimingHack = 100;
	Settings.Shutdown =	false;
	Settings.ShutdownMaster	= false;
	Settings.BlockInvalidVRAMAccess = true;
	Settings.NextAPUEnabled	= Settings.APUEnabled =	TRUE;
	Settings.DisableIRQ	= false;
	Settings.Paused	= false;
	Timings.H_Max =	SNES_CYCLES_PER_SCANLINE;
	Timings.HBlankStart	= (256 * Timings.H_Max)	/ SNES_HCOUNTER_MAX;
	Settings.SkipFrames	= 1;

	// ROM image and peripheral	options
	Settings.ForceSuperFX =	false;
	Settings.ForceNoSuperFX	= false;
	Settings.MultiPlayer5Master	= false;
	Settings.SuperScopeMaster =	false;
	Settings.MouseMaster = false;
	Settings.SuperFX = false;

	// SNES	graphics options
	Settings.BGLayering	= false;
	Settings.DisableGraphicWindows = false;
	Settings.ForceTransparency = false;
	Settings.ForceNoTransparency = false;
	Settings.DisableHDMA = false;
	Settings.Mode7Interpolate =	false;
	GUI.HeightExtend = false;
	Settings.DisplayFrameRate =	true;
//	Settings.SixteenBit =	true;
	Settings.Transparency =	true;
	Settings.SupportHiRes =	true;
	Settings.AutoDisplayMessages = false; // this port supports	text display on	post-rendered surface

	Settings.DisplayPressedKeys	= 0;
	GUI.CurrentSaveSlot = 0;
	Settings.AutoSaveDelay = 15;
	Settings.ApplyCheats = true;

	Settings.TurboMode = false;
	Settings.TurboSkipFrames = 15;
	GUI.TurboModeToggle	= true;
	Settings.AutoMaxSkipFrames = 1;
	Settings.InitialInfoStringTimeout = 120;

#ifdef NETPLAY_SUPPORT
	Settings.Port =	1996;
	NetPlay.MaxFrameSkip = 10;
	NetPlay.MaxBehindFrameCount	= 10;
	NPServer.SyncByReset = true;
	NPServer.SendROMImageOnConnect = false;
#endif

	GUI.FreezeFileDir [0] =	0;
	Settings.SampleCatchup=false;
	Settings.TakeScreenshot=false;
	Settings.StretchScreenshots=1;

	GUI.EmulatedFullScreen = false;

	GUI.Language=0;
}

 
extern "C" void S9xMainLoop(void);
int Init3d (HWND);
extern "C" void DeinitS9x(void);
extern IDirect3DDevice9 *pDevice;


int RunEmulation(char *path, char *RomName)
{
	char FullPath[_MAX_PATH];

	//exitInputThread = false;
	Settings.StopEmulation = TRUE;
	exitFromXUI = false;
 
	//eventHandle = NULL;

 
	// Redirect stderr and stdout to file. It wouldn't go to any commandline anyway.
	FILE* fout = freopen("stdout.txt", "w", stdout);
	if(fout) setvbuf(fout, NULL, _IONBF, 0);
	FILE* ferr = freopen("stderr.txt", "w", stderr);
	if(ferr) setvbuf(ferr, NULL, _IONBF, 0);

	DWORD wSoundTimerRes;
 
	ConfigFile::SetAlphaSort(false);
	ConfigFile::SetTimeSort(false);

	strcpy(FullPath, path);
	strcat(FullPath, RomName);
	rom_filename = FullPath;
 
	bool maximized = GUI.window_maximized;
	GUI.outputMethod=DIRECT3D;
 
	extern void InitLUTs(); // init hq2x
	InitLUTs();

	S9xCustomDisplayString = WinDisplayString;
	Direct3D.initialize(pDevice); 
    GUI.ControlForced = 0xff;
	GUI.FullScreen = true; 
 
 
    void InitSnes9X (void);
    InitSnes9X ();
 

    QueryPerformanceFrequency((LARGE_INTEGER*)&PCBase);
    QueryPerformanceCounter((LARGE_INTEGER*)&PCStart);
	PCEnd = PCStart;
	PCEndTicks = GetTickCount()*1000;
	PCStartTicks = GetTickCount()*1000;
    PCFrameTime = PCFrameTimeNTSC = (__int64)((float)PCBase / 59.948743718592964824120603015098f);
    PCFrameTimePAL = PCBase / 50;

    Settings.StopEmulation = TRUE;

    GUI.FrameTimerSemaphore = CreateSemaphore (NULL, 0, 10, NULL);
    GUI.ServerTimerSemaphore = CreateSemaphore (NULL, 0, 10, NULL);

   

    Settings.StopEmulation = !LoadROM (rom_filename);

    if (!Settings.StopEmulation)
    {
		bool8 loaded = Memory.LoadSRAM (S9xGetFilename (".srm", SRAM_DIR));
		if(!loaded) // help migration from earlier Snes9x versions by checking ROM directory for savestates
			Memory.LoadSRAM (S9xGetFilename (".srm", ROMFILENAME_DIR));
        S9xLoadCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
        CheckDirectoryIsWritable (S9xGetFilename (".---", SNAPSHOT_DIR));
        CheckMenuStates ();
    }

    if (!Settings.StopEmulation)
    {
        
            GUI.CursorTimer = 60;
        
    }

	S9xUnmapAllControls();
	S9xSetupDefaultKeymap();
	ChangeInputDevice();

	DWORD lastTime = GetTickCount();

	//eventHandle = CreateEvent(NULL,0,0,NULL);	
	
	//InputThread = (HANDLE)_beginthreadex(NULL,0,&ProcessInput,(void *)eventHandle,0,NULL);

	run_loop = true;	



    while (TRUE)
    {
 
#ifdef NETPLAY_SUPPORT
        if (!Settings.NetPlay || !NetPlay.PendingWait4Sync ||
            WaitForSingleObject (GUI.ClientSemaphore, 100) != WAIT_TIMEOUT)
        {
            if (NetPlay.PendingWait4Sync)
            {
                NetPlay.PendingWait4Sync = FALSE;
                NetPlay.FrameCount++;
                S9xNPStepJoypadHistory ();
            }
#endif


			// the following is a hack to allow frametimes greater than 100ms,
			// without affecting the responsiveness of the GUI
			
			do_frame_adjust=false;
	
			BOOL once = false;
			
			while (Settings.StopEmulation || (Settings.Paused && !Settings.FrameAdvance) || Settings.ForcedPause )
			{
				if (!once)
				{
					XUIMessage xuiMsg;
					InGameMenuStruct msgData;
					InGameMenuFirstFunc( &xuiMsg, &msgData, 1 );
					XuiSendMessage( hScene, &xuiMsg );

					once = true;
					wasInXUI = true;
				}

				
				pDevice->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
 
				S9xSetSoundMute(Settings.ForcedPause || (Settings.Paused && (!Settings.FrameAdvance || GUI.FAMute)));
 
			 
				// Update XUI
				app.RunFrame();

				// Render XUI
				app.Render();

 

				// Update XUI Timers
				XuiTimersRun();

				// Present the frame.
				pDevice->Present( NULL, NULL, NULL, NULL );
 
			}
 

			if(run_loop)
			{
				ProcessInputs();
				S9xMainLoop();
 
				if (!run_loop)
				{
					goto loop_exit;
				}
 	 
			}

#ifdef NETPLAY_SUPPORT
        }
#endif
      
    }

loop_exit:
#ifdef USE_GLIDE
    S9xGlideEnable (FALSE);
#endif

	Settings.StopEmulation = TRUE;

	// stop sound playback
	ReInitSound(0);
 
    if (!Settings.StopEmulation)
    {
        Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
        S9xSaveCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
    }
    //if (!VOODOO_MODE && !GUI.FullScreen)
    //    GetWindowRect (GUI.hWnd, &GUI.window_size);

#ifdef USE_OPENGL
    if (OPENGL_MODE)
        S9xOpenGLDeinit ();
#endif

	// this goes here, because the avi
	// recording might have messed with
	// the auto frame skip setting
	// (it needs to come before WinSave)
	DoAVIClose(0);

	S9xMovieShutdown (); // must happen before saving config

	WinUnlockConfigFile ();
    WinSaveConfigFile ();
	WinCleanupConfigData();

 
	Memory.Deinit();
#ifdef USE_GLIDE
	if(Settings.GlideEnable)
		S9xGlideDeinit();
	else if (Settings.OpenGLEnable)
#else
	if (Settings.OpenGLEnable) {
#endif
#ifdef USE_OPENGL
			S9xOpenGLDeinit();
#endif
	}
		S9xGraphicsDeinit();
		S9xDeinitAPU();
		WinDeleteRecentGamesList ();
		DeinitS9x();

#ifdef CHECK_MEMORY_LEAKS
		_CrtDumpMemoryLeaks();
#endif


//		ExitInputThread();

		return 0;
}  

void RestoreGUIDisplay ()
{

} 

void RestoreSNESDisplay ()
{
    

//	S9xInitUpdate();
#ifdef USE_GLIDE
    if (VOODOO_MODE && Glide.voodoo_present)
    {
        //S9xGlideEnable (TRUE);
    }
#endif

    UpdateBackBuffer();
}

void FreezeUnfreeze (int slot, bool8 freeze)
{
    static TCHAR filename [_MAX_PATH + 1];
    TCHAR drive [_MAX_DRIVE + 1];
    TCHAR dir [_MAX_DIR + 1];
    TCHAR fname [_MAX_FNAME + 1];
    TCHAR ext [_MAX_EXT + 1];

#ifdef NETPLAY_SUPPORT
    if (!freeze && Settings.NetPlay && !Settings.NetPlayServer)
    {
        S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER,
			"Only the server is allowed to load freeze files.");
        return;
    }
#endif

    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);

	std::string fatxfname(fname);
    if (fatxfname.length() > 38)
    {
	   fatxfname = fatxfname.substr(0,37);
    }

    static char *digits = "t123456789";
	for(int oldDir = 0; oldDir <= 1; oldDir++)
	{
		for(int zmv = 0; zmv <= 1; zmv++)
		{
		    if((!oldDir && !zmv) || (!freeze && _access (filename, 0) != 0 && slot < 10))
			{
				sprintf (ext, TEXT(".%03d"), slot);

				if (GUI.FreezeFileDir [0])
				{
					strcpy (filename, oldDir ? S9xGetDirectory(ROMFILENAME_DIR) : S9xGetDirectory(SNAPSHOT_DIR));
					strcat (filename, TEXT("\\"));
					strcat (filename, fatxfname.c_str());
					strcat (filename, ext);
				}
				else
					_makepath (filename, drive, "", fatxfname.c_str(), ext);
			}
		}
	}

    S9xSetPause (PAUSE_FREEZE_FILE);

    if (freeze)
	{
 
        S9xFreezeGame (filename);
 
	}
    else
    {
		const int prevSavedAtOp = ICPU.SavedAtOp;

        if (S9xUnfreezeGame (filename))
        {
//	        S9xMessage (S9X_INFO, S9X_FREEZE_FILE_INFO, S9xBasename (filename));
#ifdef NETPLAY_SUPPORT
            S9xNPServerQueueSendingFreezeFile (filename);
#endif
//            UpdateBackBuffer();
        }

		// fix next frame advance after loading non-skipping state from a skipping state
		if(prevSavedAtOp && !ICPU.SavedAtOp)
			skipNextFrameStop = true;
    }

   S9xClearPause (PAUSE_FREEZE_FILE);
}

void CheckDirectoryIsWritable (const char *filename)
{
    FILE *fs = fopen (filename, "w+");

    if (fs == NULL)
	MessageBox (GUI.hWnd, TEXT("The folder where Snes9X saves emulated save RAM files and\ngame save positions (freeze files) is currently set to a\nread-only folder.\n\nIf you do not change the game save folder, Snes9X will be\nunable to save your progress in this game. Change the folder\nfrom the Settings Dialog available from the Options menu.\n\nThe default save folder is called Saves, if no value is set.\n"),
							 TEXT("Snes9X: Unable to save file warning"),
							 MB_OK | MB_ICONINFORMATION);
    else
    {
        fclose (fs);
        remove (filename);
    }
}

static void CheckMenuStates ()
{
   /* MENUITEMINFO mii;
    unsigned int i;

    ZeroMemory( &mii, sizeof( mii));
    mii.cbSize = sizeof( mii);
    mii.fMask = MIIM_STATE;

    mii.fState = GUI.FullScreen ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_WINDOW_FULLSCREEN, FALSE, &mii);

	mii.fState = GUI.Stretch ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_WINDOW_STRETCH, FALSE, &mii);

	mii.fState = GUI.Stretch ? (GUI.AspectRatio ? MFS_CHECKED : MFS_UNCHECKED) : MFS_CHECKED|MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_WINDOW_ASPECTRATIO, FALSE, &mii);

	mii.fState = GUI.VideoMemory ? MFS_CHECKED : MFS_UNCHECKED;
	if(!GUI.Stretch)
		mii.fState |= MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_WINDOW_VIDMEM, FALSE, &mii);

	mii.fState = (Settings.Paused && !Settings.StopEmulation) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_PAUSE, FALSE, &mii);

    mii.fState = MFS_UNCHECKED;
    if (Settings.StopEmulation)
        mii.fState |= MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE_SPC_DATA, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE_SRAM_DATA, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE2, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE3, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE4, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE5, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE6, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE7, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE8, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE9, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD2, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD3, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD4, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD5, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD6, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD7, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD8, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD9, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_RESET, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_CHEAT_ENTER, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_CHEAT_SEARCH_MODAL, FALSE, &mii);

	if (GUI.FullScreen)
        mii.fState |= MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_CHEAT_SEARCH, FALSE, &mii);

#ifdef NETPLAY_SUPPORT
    if (Settings.NetPlay && !Settings.NetPlayServer)
        mii.fState = MFS_DISABLED;
    else
        mii.fState = Settings.NetPlayServer ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SERVER, FALSE, &mii);

    mii.fState = Settings.NetPlay && !Settings.NetPlayServer ? 0 : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_DISCONNECT, FALSE, &mii);

    mii.fState = Settings.NetPlay || Settings.NetPlayServer ? MFS_DISABLED : 0;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_CONNECT, FALSE, &mii);

    mii.fState = NPServer.SendROMImageOnConnect ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SEND_ROM_ON_CONNECT, FALSE, &mii);

    mii.fState = NPServer.SyncByReset ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SYNC_BY_RESET, FALSE, &mii);

    mii.fState = Settings.NetPlayServer ? 0 : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SYNC, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_ROM, FALSE, &mii);
#endif

    mii.fState = Settings.ApplyCheats ? MFS_CHECKED : MFS_UNCHECKED;
    if (Settings.StopEmulation)
        mii.fState |= MFS_DISABLED;
    SetMenuItemInfo( GUI.hMenu, ID_CHEAT_APPLY, FALSE, &mii);

    mii.fState = MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_OPTIONS, FALSE, &mii);

    if (!Settings.APUEnabled)
        mii.fState |= MFS_DISABLED;

    SetMenuItemInfo (GUI.hMenu, ID_SOUND_NOSOUND, FALSE, &mii);
    for (i = 0; i < COUNT(SoundRates); i++)
        SetMenuItemInfo (GUI.hMenu, SoundRates[i].ident, FALSE, &mii);

    if (Settings.SoundPlaybackRate == 0 || Settings.Mute)
        mii.fState |= MFS_DISABLED;

    SetMenuItemInfo (GUI.hMenu, ID_SOUND_16BIT, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_STEREO, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_SYNC, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_INTERPOLATED, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_25MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_50MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_100MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_200MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_500MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_1S, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_2S, FALSE, &mii);

    if (!Settings.Stereo)
        mii.fState |= MFS_DISABLED;

    SetMenuItemInfo (GUI.hMenu, ID_SOUND_REVERSE_STEREO, FALSE, &mii);

    mii.fState = !Settings.APUEnabled ? MFS_CHECKED | MFS_DISABLED : MFS_CHECKED;

    if (/*!DirectSound.DSAvailable ||*/  /*Settings.SoundPlaybackRate == 0 || Settings.Mute || !Settings.APUEnabled)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_NOSOUND, FALSE, &mii);
    else
    {
        for (i = 0; i < COUNT(SoundRates); i++)
        {
            if (SoundRates [i].rate == Settings.SoundPlaybackRate)
            {
                SetMenuItemInfo (GUI.hMenu, SoundRates[i].ident, FALSE, &mii);
                break;
            }
        }
    }
    if (Settings.SoundPlaybackRate == 0 || Settings.Mute)
        mii.fState |= MFS_DISABLED;

    int id;
    switch (Settings.SoundBufferSize)
    {
	case 1:  id = ID_SOUND_25MS; break;
	case 2:  id = ID_SOUND_50MS; break;
	default:
	case 4:  id = ID_SOUND_100MS; break;
	case 8:  id = ID_SOUND_200MS; break;
	case 16: id = ID_SOUND_500MS; break;
	case 32: id = ID_SOUND_1S; break;
	case 64: id = ID_SOUND_2S; break;
    }
    SetMenuItemInfo (GUI.hMenu, id, FALSE, &mii);

    if (Settings.SixteenBitSound)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_16BIT, FALSE, &mii);
    if (Settings.Stereo)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_STEREO, FALSE, &mii);
    if (Settings.InterpolatedSound)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_INTERPOLATED, FALSE,&mii);
    if (Settings.SoundSync)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_SYNC, FALSE, &mii);

    if (!Settings.Stereo)
        mii.fState |= MFS_DISABLED;
    if (Settings.ReverseStereo)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_REVERSE_STEREO, FALSE, &mii);

#ifndef DEBUGGER
    mii.fState = MFS_DISABLED;
#else
    mii.fState = MFS_UNCHECKED;
#endif
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE_SPC, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE_SA1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE_DSP1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_FRAME_ADVANCE, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_SNES_STATUS, FALSE, &mii);

	mii.fState = (!Settings.StopEmulation) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_MOVIE_PLAY, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_MOVIE_RECORD, FALSE, &mii);

	mii.fState = (S9xMovieActive () && !Settings.StopEmulation) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_MOVIE_STOP, FALSE, &mii);

	mii.fState = (!Settings.StopEmulation) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_WRITE_AVI, FALSE, &mii);

	mii.fState = (!Settings.StopEmulation && (GUI.AVIOut)) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_STOP_AVI, FALSE, &mii);

	mii.fState = (GUI.SoundChannelEnable & (1 << 0)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL1, FALSE, &mii);
	mii.fState = (GUI.SoundChannelEnable & (1 << 1)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL2, FALSE, &mii);
	mii.fState = (GUI.SoundChannelEnable & (1 << 2)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL3, FALSE, &mii);
	mii.fState = (GUI.SoundChannelEnable & (1 << 3)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL4, FALSE, &mii);
	mii.fState = (GUI.SoundChannelEnable & (1 << 4)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL5, FALSE, &mii);
	mii.fState = (GUI.SoundChannelEnable & (1 << 5)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL6, FALSE, &mii);
	mii.fState = (GUI.SoundChannelEnable & (1 << 6)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL7, FALSE, &mii);
	mii.fState = (GUI.SoundChannelEnable & (1 << 7)) ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_CHANNELS_CHANNEL8, FALSE, &mii);

	UINT validFlag;
	enum controllers controller[2];
	int8 ids[4];
	S9xGetController(0, &controller[0], &ids[0],&ids[1],&ids[2],&ids[3]);
	S9xGetController(1, &controller[1], &ids[0],&ids[1],&ids[2],&ids[3]);

	validFlag = (((1<<SNES_JOYPAD) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_JOYPAD && controller[1] == CTL_JOYPAD) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_SNES_JOYPAD, FALSE, &mii);

	validFlag = (((1<<SNES_MULTIPLAYER5) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_JOYPAD && controller[1] == CTL_MP5) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_ENABLE_MULTITAP, FALSE, &mii);

	validFlag = (((1<<SNES_MULTIPLAYER8) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_MP5 && controller[1] == CTL_MP5) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_MULTITAP8, FALSE, &mii);

	validFlag = (((1<<SNES_MOUSE) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_MOUSE && controller[1] == CTL_JOYPAD) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_MOUSE_TOGGLE, FALSE, &mii);

	validFlag = (((1<<SNES_MOUSE_SWAPPED) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_JOYPAD && controller[1] == CTL_MOUSE) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_MOUSE_SWAPPED, FALSE, &mii);

	validFlag = (((1<<SNES_SUPERSCOPE) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_JOYPAD && controller[1] == CTL_SUPERSCOPE) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_SCOPE_TOGGLE, FALSE, &mii);

	validFlag = (((1<<SNES_JUSTIFIER) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_JOYPAD && controller[1] == CTL_JUSTIFIER && !ids[0]) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_JUSTIFIER, FALSE, &mii);

	validFlag = (((1<<SNES_JUSTIFIER_2) & GUI.ValidControllerOptions) && (!S9xMovieActive() || !S9xMovieGetFrameCounter())) ? MFS_ENABLED : MFS_DISABLED;
	mii.fState = validFlag|((controller[0] == CTL_JOYPAD && controller[1] == CTL_JUSTIFIER && ids[0]) ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo (GUI.hMenu, IDM_JUSTIFIERS, FALSE, &mii); */
}

static void ResetFrameTimer ()
{
    //QueryPerformanceCounter((LARGE_INTEGER*)&PCStart);
	//PCStartTicks = GetTickCount()*1000;
    //if (Settings.FrameTime == Settings.FrameTimeNTSC) PCFrameTime = PCFrameTimeNTSC;
    //else if (Settings.FrameTime == Settings.FrameTimePAL) PCFrameTime = PCFrameTimePAL;
    //else PCFrameTime = (__int64)((double)(PCBase * Settings.FrameTime) * .000001);


    //if (GUI.hFrameTimer)
    //    timeKillEvent (GUI.hFrameTimer);

    //GUI.hFrameTimer = timeSetEvent ((Settings.FrameTime+500)/1000, 0, FrameTimer, 0, TIME_PERIODIC);
}

bool8 LoadROM (const char *filename)
{
	if (!filename || !*filename)
		return (FALSE);
 
    if (Memory.LoadROM (filename))
    {
		S9xStartCheatSearch (&Cheat);
        //ReInitSound(1);
        ResetFrameTimer ();
        return (TRUE);
    }
    return (FALSE);
}

bool8 LoadMultiROM (const char *filename, const char *filename2)
{
	 
    if (Memory.LoadMultiCart (filename, filename2))
    {
		S9xStartCheatSearch (&Cheat);    
        ResetFrameTimer ();
        return (TRUE);
    }
    return (FALSE);
}

bool8 S9xLoadROMImage (const TCHAR *string)
{
    RestoreGUIDisplay ();
    TCHAR *buf = new TCHAR [200 + strlen (string)];
    sprintf (buf, TEXT("The NetPlay server is requesting you load the following game:\n '%s'"),
		string);

    MessageBox (GUI.hWnd, buf,
		TEXT(SNES9X_INFO),
		MB_OK | MB_ICONINFORMATION);

    delete buf;

    TCHAR FileName [_MAX_PATH];

	if(DoOpenRomDialog(FileName))
    {
        if (!Settings.StopEmulation)
        {
            Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
            S9xSaveCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
        }
        Settings.StopEmulation = !LoadROM (FileName);
        if (!Settings.StopEmulation)
        {
			bool8 loaded = Memory.LoadSRAM (S9xGetFilename (".srm", SRAM_DIR));
			if(!loaded) // help migration from earlier Snes9x versions by checking ROM directory for savestates
				Memory.LoadSRAM (S9xGetFilename (".srm", ROMFILENAME_DIR));
            S9xLoadCheatFile (S9xGetFilename (".cht", CHEAT_DIR));
            CheckDirectoryIsWritable (S9xGetFilename (".---", SNAPSHOT_DIR));
            CheckMenuStates ();
        }
        else
            return (FALSE);
    }
    else
        return (FALSE);

    return (TRUE);
}

/*****************************************************************************/
#ifdef NETPLAY_SUPPORT
void EnableServer (bool8 enable)
{
    if (enable != Settings.NetPlayServer)
    {
        if (Settings.NetPlay && !Settings.NetPlayServer)
        {
            Settings.NetPlay = FALSE;
            S9xNPDisconnect ();
        }

        if (enable)
        {
            S9xSetPause (PAUSE_NETPLAY_CONNECT);
            Settings.NetPlayServer = S9xNPStartServer (Settings.Port);
            Sleep (1000);

            if (!S9xNPConnectToServer ("127.0.0.1", Settings.Port,
				Memory.ROMName))
            {
                S9xClearPause (PAUSE_NETPLAY_CONNECT);
            }
        }
        else
        {
            Settings.NetPlayServer = FALSE;
            S9xNPStopServer ();
        }
    }
}
#endif
#ifdef USE_OPENGL
bool8 S9xOpenGLInit ()
{
    int PixelFormat;

    if (GUI.FullScreen)
    {
        DEVMODE dmScreenSettings;

        XMemSet (&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = GUI.Width;
        dmScreenSettings.dmPelsHeight = GUI.Height;
        dmScreenSettings.dmBitsPerPel = 16; //bits;
        dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            GUI.FullScreen = FALSE;
        }
    }
    if (GUI.FullScreen)
    {
        SetWindowLong (GUI.hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos (GUI.hWnd, HWND_TOP, 0, 0, GUI.Width,
			GUI.Height,
			SWP_DRAWFRAME|SWP_FRAMECHANGED);
    }
    else
    {
		bool maximized = GUI.window_maximized;
        SetWindowLong (GUI.hWnd, GWL_STYLE, WS_POPUPWINDOW|WS_CAPTION|
			WS_THICKFRAME|WS_VISIBLE|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
        SetWindowPos (GUI.hWnd, HWND_TOP,
			GUI.window_size.left,
			GUI.window_size.top,
			GUI.window_size.right - GUI.window_size.left,
			GUI.window_size.bottom - GUI.window_size.top,
			SWP_DRAWFRAME|SWP_FRAMECHANGED);
		if(maximized)
			ShowWindow(GUI.hWnd, SW_MAXIMIZE);
    }

	if(OpenGL.initialized)
		return (TRUE);

	static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			16, //bits,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
    };

    if (!(PixelFormat = ChoosePixelFormat (GUI.hDC, &pfd)))
    {
        MessageBox(NULL,TEXT("Can't Find A Suitable PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!SetPixelFormat (GUI.hDC, PixelFormat, &pfd))
    {
        MessageBox(NULL,TEXT("Can't Set The PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }

    if (!(GUI.hRC = wglCreateContext (GUI.hDC)))
    {
        MessageBox(NULL,TEXT("Can't Create A GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }

    if (!wglMakeCurrent (GUI.hDC, GUI.hRC))
    {
        MessageBox(NULL,TEXT("Can't Activate The GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }

    glGetIntegerv (GL_MAX_TEXTURE_SIZE, &OpenGL.max_texture_size);

    if (OpenGL.max_texture_size >= 512)
    {
        OpenGL.texture_size = 512;
        OpenGL.num_textures = 2;
    }
    else
    {
        OpenGL.texture_size = OpenGL.max_texture_size;
        OpenGL.num_textures = 1;
    }

    Settings.OpenGLEnable = TRUE;

    const char *ext = (const char *) glGetString (GL_EXTENSIONS);

    if (ext && strstr (ext, "EXT_packed_pixels") != NULL)
        OpenGL.packed_pixels_extension_present = TRUE;

    const char *version = (const char *) glGetString (GL_VERSION);

    if (version && strlen (version) < 100)
    {
		char ver [100];
		strcpy (ver,  version);

		// Strip dots from the version string
		char *ptr;
		while (ptr = strchr (ver, '.'))
			memmove (ptr, ptr + 1, strlen (ptr + 1) + 1);

		// Pad the string with zeros to 4 digits
		while (strlen (ver) < 4)
			strcat (ver, "0");

		OpenGL.version = atoi (ver);
    }
    else
		OpenGL.version = 1100;

#ifndef GL_UNSIGNED_SHORT_5_5_5_1_EXT
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT     0x8034
#endif

    if (OpenGL.version >= 1200)
    {
        OpenGL.internal_format = GL_RGB5_A1;
        OpenGL.format = GL_RGBA;
        OpenGL.type = GL_UNSIGNED_SHORT_5_5_5_1_EXT;
    }
    else
		if (OpenGL.packed_pixels_extension_present)
		{
			OpenGL.internal_format = GL_RGB5_A1;
			OpenGL.format = GL_RGBA;
			OpenGL.type = GL_UNSIGNED_SHORT_5_5_5_1_EXT;
		}
		else
		{
			OpenGL.internal_format = GL_RGB;
			OpenGL.format = GL_RGB;
			OpenGL.type = GL_UNSIGNED_BYTE;
		}

		glGenTextures (OpenGL.num_textures, OpenGL.textures);

		if (OpenGL.num_textures == 2)
		{
			glBindTexture (GL_TEXTURE_2D, OpenGL.textures [1]);
			glTexImage2D (GL_TEXTURE_2D, 0, OpenGL.internal_format, 256, 256, 0,
				OpenGL.format, OpenGL.type, NULL);

			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		glBindTexture (GL_TEXTURE_2D, OpenGL.textures [0]);
		glTexImage2D (GL_TEXTURE_2D, 0, OpenGL.internal_format,
			OpenGL.texture_size, OpenGL.texture_size, 0,
			OpenGL.format, OpenGL.type, NULL);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glPolygonMode (GL_FRONT, GL_FILL);
		glEnable (GL_CULL_FACE);
		glCullFace (GL_BACK);

		glEnable (GL_DITHER);
		glEnable (GL_LIGHTING);
		glEnable (GL_LIGHT0);

		glEnable (GL_POINT_SMOOTH);
		glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

		glEnable (GL_TEXTURE_2D);

		glShadeModel (GL_SMOOTH);
		glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth (1.0f);
		glDisable (GL_DEPTH_TEST);
		glDepthFunc (GL_LESS);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		GLfloat LightAmbient[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat LightDiffuse[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightSpecular[]   = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat LightPosition[]   = { 0.0f, 0.0f, 2.0f, 1.0f };

		glLightfv (GL_LIGHT0, GL_AMBIENT, LightAmbient);
		glLightfv (GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
		glLightfv (GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);

		//Set common material properties
		GLfloat MatSpecular[]    = { 1.0f, 1.0f, 1.0f, 1.1f };
		GLfloat WhMat[]          = { 1.0f, 1.0f, 1.0f, 1.0f };

		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
		glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
		glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, WhMat);

		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glMatrixMode (GL_MODELVIEW);

	    glDisable (GL_DEPTH_TEST);
	    glDisable (GL_LIGHTING);
		glEnable (GL_TEXTURE_2D);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glDisable (GL_BLEND);

		OpenGL.initialized = TRUE;

		return (TRUE);
}

void S9xOpenGLDeinit ()
{
    if (GUI.FullScreen)
        ChangeDisplaySettings (NULL, 0);

    if (GUI.hRC)
    {
        wglMakeCurrent (NULL, NULL);
        wglDeleteContext (GUI.hRC);
        GUI.hRC = NULL;
    }
}

void S9xOpenGLResize (int width, int height)
{
    if (height == 0)
        height = 1;

    glViewport (0, 0, width, height);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    gluPerspective (45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode (GL_MODELVIEW);
}
#endif

void S9xAddToRecentGames (const char *filename)
{
    // Make sure its not in the list already
    int i;
	for(i = 0; i < MAX_RECENT_GAMES_LIST_SIZE; i++)
        if (!*GUI.RecentGames[i] || strcmp (filename, GUI.RecentGames[i]) == 0)
            break;

	const bool underMax = (i < MAX_RECENT_GAMES_LIST_SIZE);
	if(underMax && *GUI.RecentGames[i])
	{
		// It is in the list, move it to the head of the list.
		char temp [MAX_PATH];
		strcpy(temp, GUI.RecentGames[i]);
		for(int j = i; j > 0; j--)
			strcpy(GUI.RecentGames[j], GUI.RecentGames[j-1]);

		strcpy(GUI.RecentGames[0], temp);
	}
	else
	{
		// Not in the list, add it.
		if(underMax)
			// Extend the recent game list length by 1.
			memmove(&GUI.RecentGames[1], &GUI.RecentGames[0], MAX_PATH*i);
		else
			// Throw the last item off the end of the list
			memmove(&GUI.RecentGames[1], &GUI.RecentGames[0], MAX_PATH*(i-1));

		strcpy(GUI.RecentGames[0], filename);

		WinSaveConfigFile();
	}

    S9xSetRecentGames();
}

void S9xRemoveFromRecentGames (int i)
{
	if (*GUI.RecentGames [i])
	{
		for (int j = i; j < MAX_RECENT_GAMES_LIST_SIZE-1; j++)
			strcpy(GUI.RecentGames [j], GUI.RecentGames [j + 1]);
		*GUI.RecentGames [MAX_RECENT_GAMES_LIST_SIZE-1] = '\0';

		S9xSetRecentGames ();
	}
}

void S9xSetRecentGames ()
{

	/*
    HMENU file = GetSubMenu (GUI.hMenu, 0);
    if (file)
    {
        HMENU recent = GetSubMenu (file, 1);
        if (recent)
        {
            MENUITEMINFO mii;
            TCHAR name [256 + 10];
            int i;

            // Clear out the menu first
            for (i = GetMenuItemCount (recent) - 1; i >= 0; i--)
                RemoveMenu (recent, i, MF_BYPOSITION);

            mii.cbSize = sizeof (mii);
            mii.fMask = MIIM_TYPE | MIIM_DATA | MIIM_STATE | MIIM_ID;
            mii.fType = MFT_STRING;
            mii.fState = MFS_UNCHECKED;

            for (i = 0; i < MAX_RECENT_GAMES_LIST_SIZE && i < GUI.MaxRecentGames && *GUI.RecentGames [i]; i++)
            {
                // Build up a menu item string in the form:
                // 1. <basename of ROM image name>

                sprintf (name, TEXT("&%c. "), i < 9 ? '1' + i : 'A' + i - 9);

				// append the game title to name, with formatting modifications as necessary
				{
					TCHAR baseName [256];
					strcpy (baseName, S9xBasename (GUI.RecentGames [i]));
					int pos = strlen (name), baseNameLen = strlen (baseName);
					for (int j = 0; j < baseNameLen ; j++)
					{
						char c = baseName [j];
						name [pos++] = c;

						// & is a special character in Windows menus,
						// so we have to change & to && when copying over the game title
						// otherwise "Pocky & Rocky (U).smc" will show up as "Pocky _Rocky (U).smc", for example
						if(c == '&')
							name [pos++] = '&';
					}
					name [pos] = '\0';
				}

				mii.dwTypeData = name;
                mii.cch = strlen (name) + 1;
                mii.wID = 0xFF00 + i;

                InsertMenuItem (recent, 0xFF00 + i, FALSE, &mii);
            }
        }
    } */
}

void WinDeleteRecentGamesList ()
{
	for(int i=0;i<MAX_RECENT_GAMES_LIST_SIZE;i++)
		GUI.RecentGames[i][0]='\0';
}

 
int CALLBACK DlgSP7PackConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

#ifdef RTC_DEBUGGER
int CALLBACK SPC7110rtc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int month;
	static int day;
	static int year;
	static int hour;
	static int minutes;
	static int seconds;
	static int dayinmonth;
	static struct SPC7110RTC* rtc;
	switch(msg)
	{
	case WM_INITDIALOG:
		if(DirectDraw.Clipped) S9xReRefresh();

		rtc= (struct SPC7110RTC *)lParam;

		seconds=rtc->reg[0]+rtc->reg[1]*10;
		minutes=rtc->reg[2]+rtc->reg[3]*10;
		hour=rtc->reg[4]+rtc->reg[5]*10;
		day=rtc->reg[6]+rtc->reg[7]*10;
		month=rtc->reg[8]+rtc->reg[9]*10;
		year=rtc->reg[10]+rtc->reg[11]*10;
		dayinmonth=S9xRTCDaysInMonth(month,year);


		SendDlgItemMessage(hDlg,IDC_MONTH, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,11));
		SendDlgItemMessage(hDlg,IDC_MONTH, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_MONTH, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)month-1);
		SetDlgItemInt(hDlg,IDC_LBLMONTH,month,0);

		SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,dayinmonth-1));
		SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)day-1);
		SetDlgItemInt(hDlg,IDC_LBLDAY,day,0);

		SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,99));
		SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETTICFREQ,(WPARAM)5,0);
		if(year<95)
		{
			SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)year+5);
			SetDlgItemInt(hDlg,IDC_LBLYEAR,year+2000,0);
		}
		else
		{
			SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)year-95);
			SetDlgItemInt(hDlg,IDC_LBLYEAR,year+1900,0);
		}

		SendDlgItemMessage(hDlg, IDC_HOUR, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,23));
		SendDlgItemMessage(hDlg,IDC_HOUR, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_HOUR, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)hour);
		SetDlgItemInt(hDlg,IDC_LBLHOUR,hour,0);

		SendDlgItemMessage(hDlg, IDC_MINUTE, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,59));
		SendDlgItemMessage(hDlg,IDC_MINUTE, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_MINUTE, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)minutes);
		SetDlgItemInt(hDlg,IDC_LBLMINUTE,minutes,0);

		SendDlgItemMessage(hDlg, IDC_SECOND, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,59));
		SendDlgItemMessage(hDlg,IDC_SECOND, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_SECOND, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)seconds);
		SetDlgItemInt(hDlg,IDC_LBLSECOND,seconds,0);



		if(rtc->reg[0x0D]&0x01)
			SendDlgItemMessage(hDlg,IDC_RTC_D1,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0D]&0x02)
			SendDlgItemMessage(hDlg,IDC_RTC_D2,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0D]&0x04)
			SendDlgItemMessage(hDlg,IDC_RTC_D4,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0D]&0x08)
			SendDlgItemMessage(hDlg,IDC_RTC_D8,BM_SETCHECK,BST_CHECKED,0);

		if(rtc->reg[0x0E]&0x01)
			SendDlgItemMessage(hDlg,IDC_RTC_E1,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0E]&0x02)
			SendDlgItemMessage(hDlg,IDC_RTC_E2,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0E]&0x04)
			SendDlgItemMessage(hDlg,IDC_RTC_E4,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0E]&0x08)
			SendDlgItemMessage(hDlg,IDC_RTC_E8,BM_SETCHECK,BST_CHECKED,0);

		if(rtc->reg[0x0F]&0x01)
			SendDlgItemMessage(hDlg,IDC_RTC_F1,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0F]&0x02)
			SendDlgItemMessage(hDlg,IDC_RTC_F2,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0F]&0x04)
			SendDlgItemMessage(hDlg,IDC_RTC_F4,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0F]&0x08)
			SendDlgItemMessage(hDlg,IDC_RTC_F8,BM_SETCHECK,BST_CHECKED,0);

		return true;
	case  WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			rtc->reg[0x0]=seconds%10;
			rtc->reg[0x1]=seconds/10;
			rtc->reg[0x2]=minutes%10;
			rtc->reg[0x3]=minutes/10;
			rtc->reg[0x4]=hour%10;
			rtc->reg[0x5]=hour/10;
			rtc->reg[0x6]=day%10;
			rtc->reg[0x7]=day/10;
			rtc->reg[0x8]=month%10;
			rtc->reg[0x9]=month/10;
			rtc->reg[0xA]=year%10;
			rtc->reg[0xB]=year/10;

			if(IsDlgButtonChecked(hDlg, IDC_RTC_D1))
				rtc->reg[0x0D]|=0x01;
			else
				rtc->reg[0x0D]&=0x0E;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_D2))
				rtc->reg[0x0D]|=0x02;
			else
				rtc->reg[0x0D]&=0x0D;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_D4))
				rtc->reg[0x0D]|=0x04;
			else
				rtc->reg[0x0D]&=0x0B;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_D8))
				rtc->reg[0x0D]|=0x08;
			else
				rtc->reg[0x0D]&=0x07;

			if(IsDlgButtonChecked(hDlg, IDC_RTC_E1))
				rtc->reg[0x0E]|=0x01;
			else
				rtc->reg[0x0E]&=0x0E;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_E2))
				rtc->reg[0x0E]|=0x02;
			else
				rtc->reg[0x0E]&=0x0D;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_E4))
				rtc->reg[0x0E]|=0x04;
			else
				rtc->reg[0x0E]&=0x0B;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_E8))
				rtc->reg[0x0E]|=0x08;
			else
				rtc->reg[0x0E]&=0x07;

			if(IsDlgButtonChecked(hDlg, IDC_RTC_F1))
				rtc->reg[0x0F]|=0x01;
			else
				rtc->reg[0x0F]&=0x0E;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_F2))
				rtc->reg[0x0F]|=0x02;
			else
				rtc->reg[0x0F]&=0x0D;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_F4))
				rtc->reg[0x0F]|=0x04;
			else
				rtc->reg[0x0F]&=0x0B;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_F8))
				rtc->reg[0x0F]|=0x08;
			else
				rtc->reg[0x0F]&=0x07;

			WinSaveConfigFile();

		case IDCANCEL:
			EndDialog(hDlg, 1);
			return true;
			/*case IDC_MONTH:
			int dinmonth;
			dinmonth = S9xRTCDaysInMonth(
			*/
		default: return false;
		}
		case WM_HSCROLL:
			month=1+SendDlgItemMessage(hDlg,IDC_MONTH,TBM_GETPOS,0,0);
			SetDlgItemInt(hDlg,IDC_LBLMONTH,month,0);
			day=1+SendDlgItemMessage(hDlg,IDC_DAY,TBM_GETPOS,0,0);
			year=SendDlgItemMessage(hDlg,IDC_YEAR,TBM_GETPOS,0,0);
			hour=SendDlgItemMessage(hDlg,IDC_HOUR,TBM_GETPOS,0,0);
			minutes=SendDlgItemMessage(hDlg,IDC_MINUTE,TBM_GETPOS,0,0);
			seconds=SendDlgItemMessage(hDlg,IDC_SECOND,TBM_GETPOS,0,0);
			SetDlgItemInt(hDlg,IDC_LBLHOUR,hour,0);
			SetDlgItemInt(hDlg,IDC_LBLMINUTE,minutes,0);
			SetDlgItemInt(hDlg,IDC_LBLSECOND,seconds,0);
			if(year<5)
			{
				year+=95;
				SetDlgItemInt(hDlg,IDC_LBLYEAR,year+1900,0);
			}
			else
			{
				year-=5;
				SetDlgItemInt(hDlg,IDC_LBLYEAR,year+2000,0);
			}
			dayinmonth=S9xRTCDaysInMonth(month,year);
			if(day>dayinmonth)
			{
				day=dayinmonth;
				SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)day-1);
			}
			SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,dayinmonth-1));
			SetDlgItemInt(hDlg,IDC_LBLDAY,day,0);
			return true;

		default: return false;
	}

}
#endif

 

const char *StaticRAMBitSize ()
{
    static char tmp [20];

    sprintf (tmp, " (%dKbit)", 8*(Memory.SRAMMask + 1) / 1024);
    return (tmp);
}
 
 

 

#define SKIP_FLOPPY

bool ExtensionIsValid(const TCHAR * filename)
{
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		if(curr->extension==NULL)
		{
			if(NULL==strstr(filename, TEXT(".")))
				return true;
		}
		else if(filename[(strlen(filename)-1)-strlen(curr->extension)]=='.')
		{
			if(0==_strnicmp(&filename[(strlen(filename))-strlen(curr->extension)],
				curr->extension, strlen(curr->extension)))
				return true;
		}
		curr=curr->next;
	}
	return false;
}

bool IsCompressed(const TCHAR* filename)
{
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		if(curr->extension==NULL)
		{
			if(NULL==strstr(filename, TEXT(".")))
				return curr->compressed;
		}
		else if(filename[(strlen(filename)-1)-strlen(curr->extension)]=='.')
		{
			if(0==_strnicmp(&filename[(strlen(filename))-strlen(curr->extension)],
				curr->extension, strlen(curr->extension)))
				return curr->compressed;
		}
		curr=curr->next;
	}
	return false;
}

inline bool AllASCII(char *b, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (b[i] < 32 || b[i] > 126)
		{
			return(false);
		}
	}
	return(true);
}

inline int InfoScore(char *Buffer)
{
	int score = 0;
	if (Buffer[28] + (Buffer[29] << 8) +
		Buffer[30] + (Buffer[31] << 8) == 0xFFFF)
	{	score += 3; }

	if (Buffer[26] == 0x33) { score += 2; }
	if ((Buffer[21] & 0xf) < 4) {	score += 2; }
	if (!(Buffer[61] & 0x80)) { score -= 4; }
	if ((1 << (Buffer[23] - 7)) > 48) { score -= 1; }
	if (Buffer[25] < 14) { score += 1; }
	if (!AllASCII(Buffer, 20)) { score -= 1; }

	return (score);
}

inline unsigned short sum(unsigned char *array, unsigned int size = HEADER_SIZE)
{
	register unsigned short theSum = 0;
	for (register unsigned int i = 0; i < size; i++)
	{
		theSum += array[i];
	}
	return(theSum);
}

void rominfo(const TCHAR *filename, TCHAR *namebuffer, TCHAR *sizebuffer)
{
	strcpy(namebuffer, ROM_ITEM_DESCNOTAVAILABLE);
	strcpy(sizebuffer, "? Mbits");

	if(IsCompressed(filename))
	{
		unzFile uf = unzOpen(filename);
		if(uf)
		{
			unz_file_info info;
			if(UNZ_OK == unzGetCurrentFileInfo(uf, &info, 0,0,0,0,0,0))
			{
				if (info.uncompressed_size < 0x8000) // Smaller than a block
					strcpy(namebuffer, ROM_ITEM_NOTAROM);
				else
					strcpy(namebuffer, ROM_ITEM_COMPRESSEDROMDESCRIPTION);

				// should subtract header size, so this may be slightly off, but it's better than "? MBits"
				double MBitD = (double)(info.uncompressed_size - 0) / 0x100000 * 8;
				int MBitI = (int)MBitD;
				int sizeIndex;
				if(0!=(MBitI / 10))
				{
					sizebuffer[0] = MBitI / 10 + '0';
					sizeIndex = 1;
				}
				else
					sizeIndex = 0;
				sizebuffer[sizeIndex+0] = MBitI % 10 + '0';
				sizebuffer[sizeIndex+1] = '.';
				sizebuffer[sizeIndex+2] = (char)((MBitD - MBitI) * 10) + '0';
				sizebuffer[sizeIndex+3] = (char)((int)((MBitD - MBitI) * 100) % 10) + '0';
				sizebuffer[sizeIndex+4] = ' ';
				sizebuffer[sizeIndex+5] = 'M';
				sizebuffer[sizeIndex+6] = 'b';
				sizebuffer[sizeIndex+7] = 'i';
				sizebuffer[sizeIndex+8] = 't';
				sizebuffer[sizeIndex+9] = '\0';
			}
			unzClose(uf);
		}
		return;
	}

	struct stat filestats;
	stat(filename, &filestats);

	int HeaderSize = 0;

	if (filestats.st_size >= 0x8000)
	{
		ifstream ROMFile(filename, ios::in | ios::binary);
		if (ROMFile)
		{
			int HasHeadScore = 0, NoHeadScore = 0,
				HeadRemain = filestats.st_size & 0x7FFF;

			switch(HeadRemain)
			{
			case 0:
				NoHeadScore += 3;
				break;

			case HEADER_SIZE:
				HasHeadScore += 2;
				break;
			}

			unsigned char HeaderBuffer[HEADER_SIZE];
			ROMFile.read((char *)HeaderBuffer, HEADER_SIZE);

			if (sum(HeaderBuffer) < 2500) { HasHeadScore += 2; }

			//SMC/SWC Header
			if (HeaderBuffer[8] == 0xAA &&
				HeaderBuffer[9] == 0xBB &&
				HeaderBuffer[10]== 4)
			{ HasHeadScore += 3; }
			//FIG Header
			else if ((HeaderBuffer[4] == 0x77 && HeaderBuffer[5] == 0x83) ||
				(HeaderBuffer[4] == 0xDD && HeaderBuffer[5] == 0x82) ||
				(HeaderBuffer[4] == 0xDD && HeaderBuffer[5] == 2) ||
				(HeaderBuffer[4] == 0xF7 && HeaderBuffer[5] == 0x83) ||
				(HeaderBuffer[4] == 0xFD && HeaderBuffer[5] == 0x82) ||
				(HeaderBuffer[4] == 0x00 && HeaderBuffer[5] == 0x80) ||
				(HeaderBuffer[4] == 0x47 && HeaderBuffer[5] == 0x83) ||
				(HeaderBuffer[4] == 0x11 && HeaderBuffer[5] == 2))
			{ HasHeadScore += 2; }
			else if (!strncmp("GAME DOCTOR SF 3", (char *)HeaderBuffer, 16))
			{ HasHeadScore += 5; }

			HeaderSize = HasHeadScore > NoHeadScore ? HEADER_SIZE : 0;

			bool EHi = false;
			if (filestats.st_size - HeaderSize >= 0x500000)
			{
				ROMFile.seekg(0x40FFC0 + HeaderSize, ios::beg);
				ROMFile.read((char *)HeaderBuffer, INFO_LEN);
				if (InfoScore((char *)HeaderBuffer) > 1)
				{
					EHi = true;
					strncpy(namebuffer, (char *)HeaderBuffer, 21);
				}
			}

			if (!EHi)
			{
				if (filestats.st_size - HeaderSize >= 0x10000)
				{
					char LoHead[INFO_LEN], HiHead[INFO_LEN];

					ROMFile.seekg(0x7FC0 + HeaderSize, ios::beg);
					ROMFile.read(LoHead, INFO_LEN);
					int LoScore = InfoScore(LoHead);

					ROMFile.seekg(0xFFC0 + HeaderSize, ios::beg);
					ROMFile.read(HiHead, INFO_LEN);
					int HiScore = InfoScore(HiHead);

					strncpy(namebuffer, LoScore > HiScore ? LoHead : HiHead, 21);

					if (filestats.st_size - HeaderSize >= 0x20000)
					{
						ROMFile.seekg((filestats.st_size - HeaderSize) / 2 + 0x7FC0 + HeaderSize, ios::beg);
						ROMFile.read(LoHead, INFO_LEN);
						int IntLScore = InfoScore(LoHead) / 2;

						if (IntLScore > LoScore && IntLScore > HiScore)
						{
							strncpy(namebuffer, LoHead, 21);
						}
					}
				}
				else //ROM only has one block
				{
					ROMFile.seekg(0x7FC0 + HeaderSize, ios::beg);
					ROMFile.read(namebuffer, 21);
				}
			}
			ROMFile.close();
		}
		else //Couldn't open file
		{
			strcpy(namebuffer, ROM_ITEM_CANTOPEN);
		}
	}
	else //Smaller than a block
	{
		strcpy(namebuffer, ROM_ITEM_NOTAROM);
	}

	double MBitD = (double)(filestats.st_size - HeaderSize) / 0x100000 * 8;
	int MBitI = (int)MBitD;
	int sizeIndex;
	if(0!=(MBitI / 10))
	{
		sizebuffer[0] = MBitI / 10 + '0';
		sizeIndex = 1;
	}
	else
		sizeIndex = 0;
	sizebuffer[sizeIndex+0] = MBitI % 10 + '0';
	sizebuffer[sizeIndex+1] = '.';
	sizebuffer[sizeIndex+2] = (char)((MBitD - MBitI) * 10) + '0';
	sizebuffer[sizeIndex+3] = (char)((int)((MBitD - MBitI) * 100) % 10) + '0';
	sizebuffer[sizeIndex+4] = ' ';
	sizebuffer[sizeIndex+5] = 'M';
	sizebuffer[sizeIndex+6] = 'b';
	sizebuffer[sizeIndex+7] = 'i';
	sizebuffer[sizeIndex+8] = 't';
	sizebuffer[sizeIndex+9] = '\0';
	namebuffer[21] = '\0';
}

 

typedef struct RomDataCacheNode
{
	char* fname;
	char* rname;
	char* rmbits;
	struct RomDataCacheNode* next;
} RomDataList;

void ClearCacheList(RomDataList* rdl)
{
	RomDataList* temp=rdl;
	RomDataList* temp2=NULL;
	if(rdl==NULL)
		return;
	do
	{
		temp2=temp->next;
		if(temp->fname)
			delete [] temp->fname;
		if(temp->rmbits)
			delete [] temp->rmbits;
		if(temp->rname)
			delete [] temp->rname;
		delete temp;
		temp=temp2;
	}
	while(temp!=NULL);
}



void ListFilesFromFolder(HWND hDlg, RomDataList** prdl)
{
 
}

extern "C"
{
	char*osd_GetPackDir()
	{
		static char filename[MAX_PATH];
		XMemSet(filename, 0, MAX_PATH);

		if(strlen(GUI.FreezeFileDir)!=0)
			strcpy (filename, GUI.FreezeFileDir);
		else
		{
			char dir [_MAX_DIR + 1];
			char drive [_MAX_DRIVE + 1];
			char name [_MAX_FNAME + 1];
			char ext [_MAX_EXT + 1];
			_splitpath(Memory.ROMFilename,drive, dir, name, ext);
			_makepath(filename,drive, dir, NULL, NULL);
		}

		if(!strncmp((char*)&Memory.ROM [0xffc0], "SUPER POWER LEAG 4   ", 21))
		{
			if(strlen(GUI.SPL4Pack))
				return GUI.SPL4Pack;
			else strcat(filename, "\\SPL4-SP7");
		}
		else if(!strncmp((char*)&Memory.ROM [0xffc0], "MOMOTETSU HAPPY      ",21))
		{
			if(strlen(GUI.MDHPack))
				return GUI.MDHPack;
			else strcat(filename, "\\SMHT-SP7");
		}
		else if(!strncmp((char*)&Memory.ROM [0xffc0], "HU TENGAI MAKYO ZERO ", 21))
		{
			if(strlen(GUI.FEOEZPack))
				return GUI.FEOEZPack;
			else strcat(filename, "\\FEOEZSP7");
		}
		else if(!strncmp((char*)&Memory.ROM [0xffc0], "JUMP TENGAIMAKYO ZERO",21))
		{
			if(strlen(GUI.SJNSPack))
				return GUI.SJNSPack;
			else strcat(filename, "\\SJUMPSP7");
		}
		else strcat(filename, "\\MISC-SP7");
		return filename;
	}
}
#ifdef NETPLAY_SUPPORT
int CALLBACK DlgNetConnect(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
//	HKEY hKey;
	char defPort[5];
	char portTemp[5];
	char temp[100];
//	char temp2[5];
	static char* hostname;
//	unsigned long cbData;
//	static int i;
	if(Settings.Port==0)
	{
		_itoa(1996,defPort,10);
	}
	else
	{
		_itoa(Settings.Port,defPort,10);
	}

	WORD chkLength;
//	if(RegCreateKeyEx(HKEY_CURRENT_USER,MY_REG_KEY "\\1.x\\NetPlayServerHistory",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS, NULL, &hKey,NULL) == ERROR_SUCCESS){}

	switch (msg)
	{
	case WM_INITDIALOG:
		if(DirectDraw.Clipped) S9xReRefresh();
		SetWindowText(hDlg,NPCON_TITLE);
		SetDlgItemText(hDlg,IDC_LABEL_SERVERADDY,NPCON_LABEL_SERVERADDY);
		SetDlgItemText(hDlg,IDC_LABEL_PORTNUM,NPCON_LABEL_PORTNUM);
		SetDlgItemText(hDlg,IDC_CLEARHISTORY, NPCON_CLEARHISTORY);
		SetDlgItemText(hDlg,IDOK,BUTTON_OK);
		SetDlgItemText(hDlg,IDCANCEL,BUTTON_CANCEL);
		hBmp=(HBITMAP)LoadImage(NULL, TEXT("Overload.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hostname = (char *)lParam;
		{
			for(int i=0; i<MAX_RECENT_HOSTS_LIST_SIZE && *GUI.RecentHostNames[i]; i++)
				SendDlgItemMessage(hDlg, IDC_HOSTNAME, CB_INSERTSTRING,i,(LPARAM)GUI.RecentHostNames[i]);
		}

		SendDlgItemMessage(hDlg, IDC_PORTNUMBER, WM_SETTEXT, 0, (LPARAM)defPort);

		SendDlgItemMessage(hDlg, IDC_HOSTNAME, WM_SETTEXT, 0, (LPARAM)NPCON_ENTERHOST);

		return TRUE;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint (hDlg, &ps);
			if(hBmp)
			{
				BITMAP bmp;
				ZeroMemory(&bmp, sizeof(BITMAP));
				RECT r;
				GetClientRect(hDlg, &r);
				HDC hdc=GetDC(hDlg);
				HDC hDCbmp=CreateCompatibleDC(hdc);
				GetObject(hBmp, sizeof(BITMAP), &bmp);
				HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
				StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
				SelectObject(hDCbmp, hOldBmp);
				DeleteDC(hDCbmp);
				ReleaseDC(hDlg, hdc);
			}

			EndPaint (hDlg, &ps);
		}
		return true;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CLEARHISTORY:
			{
				{
					SendDlgItemMessage(hDlg,IDC_HOSTNAME,CB_RESETCONTENT,0,0);
					SendDlgItemMessage(hDlg,IDC_HOSTNAME,CB_INSERTSTRING,0,(LPARAM)GUI.RecentHostNames[0]);
					for(int i=1; i<MAX_RECENT_HOSTS_LIST_SIZE; i++)
						*GUI.RecentHostNames[i] = '\0';
				}
				break;
			}
		case IDOK:
			{

				chkLength = (WORD) SendDlgItemMessage(hDlg,IDC_PORTNUMBER,EM_LINELENGTH,0,0);
				*((LPWORD)portTemp) = chkLength;
				SendDlgItemMessage(hDlg,IDC_PORTNUMBER,EM_GETLINE,0,(LPARAM)(LPCTSTR)portTemp);

				if(atoi(portTemp)>65535||atoi(portTemp)<1024)
				{
					MessageBox(hDlg,"Port Number needs to be between 1024 and 65535","Error",MB_OK);
					break;
				}
				else
				{
					Settings.Port = atoi(portTemp);
				}
				//chkLength = (WORD) SendDlgItemMessage(hDlg,IDC_HOSTNAME,EM_LINELENGTH,0,0);
				//if(chkLength > 0)
				//{
				//SendDlgItemMessage(hDlg,IDC_HOSTNAME,EM_GETLINE,0,(LPARAM)hostname);
				SendDlgItemMessage(hDlg,IDC_HOSTNAME,WM_GETTEXT,100,(LPARAM)temp);
				if(!strcmp(temp, NPCON_ENTERHOST))
				{
					MessageBox(hDlg,NPCON_PLEASE_ENTERHOST,"Error",MB_OK);
					break;
				}
				strcpy(hostname,temp);
				//MessageBox(hDlg,temp,"hola",MB_OK);

				// save hostname in recent list
				{
					int i;
					for(i=0; i<MAX_RECENT_HOSTS_LIST_SIZE; i++)
					{
						if(!*GUI.RecentHostNames[i])
						{
							strcpy(GUI.RecentHostNames[i], hostname);
							break;
						}
						else if(!stricmp(GUI.RecentHostNames[i], hostname))
							break;
					}
					if(i == MAX_RECENT_HOSTS_LIST_SIZE)
						strcpy(GUI.RecentHostNames[1+(rand()%(MAX_RECENT_HOSTS_LIST_SIZE-1))], hostname);
				}

				unsigned long len;
				len = strlen(temp);
				if(len > 0)
				{
					EndDialog(hDlg,1);
					if(hBmp)
					{
						DeleteObject(hBmp);
						hBmp=NULL;
					}
					return TRUE;
				}
				else
				{
					EndDialog(hDlg,0);
					if(hBmp)
					{
						DeleteObject(hBmp);
						hBmp=NULL;
					}
					return TRUE;
				}

				break;
				//}
			}
		case IDCANCEL:
			{
				EndDialog(hDlg, 0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return TRUE;
			}
		default:break;
		}
	}
	return FALSE;
}
#endif
void SetInfoDlgColor(unsigned char r, unsigned char g, unsigned char b)
{
	//GUI.InfoColor=RGB(r,g,b);
}

void ClearExts(void)
{
	ExtList* temp;
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		temp=curr->next;
		if(curr->extension)
			delete [] curr->extension;
		delete curr;
		curr=temp;
	}
	valid_ext=NULL;

}

void LoadExts(void)
{
	char buffer[MAX_PATH+2];
	if(valid_ext!=NULL)
	{
		ClearExts();
	}
	ExtList* curr;
	valid_ext=new ExtList;
	curr=valid_ext;
	ZeroMemory(curr, sizeof(ExtList));
	ifstream in;

#if (((defined(_MSC_VER) && _MSC_VER >= 1300)) || defined(__MINGW32__))
	in.open("Valid.Ext", ios::in);
#else
	in.open("Valid.Ext", ios::in|ios::nocreate);
#endif
	if (!in.is_open())
	{
		in.clear();
		MakeExtFile();
	#if (((defined(_MSC_VER) && _MSC_VER >= 1300)) || defined(__MINGW32__))
		in.open("Valid.Ext", ios::in);
	#else
		in.open("Valid.Ext", ios::in|ios::nocreate);
	#endif
		if(!in.is_open())
		{
 
		}
	}

	do
	{
		buffer[0]='\0';
		in.getline(buffer,MAX_PATH+2);
		if((*buffer)!='\0')
		{
			curr->next=new ExtList;
			curr=curr->next;
			ZeroMemory(curr, sizeof(ExtList));
			if(_strnicmp(buffer+strlen(buffer)-1, "Y", 1)==0)
				curr->compressed=true;
			if(strlen(buffer)>1)
			{
				curr->extension=new char[strlen(buffer)-1];
				strncpy(curr->extension, buffer, strlen(buffer)-1);
				curr->extension[strlen(buffer)-1]='\0';
			}
			else curr->extension=NULL;
		}
	}
	while(!in.eof());
	in.close();
	curr=valid_ext;
	valid_ext=valid_ext->next;
	delete curr;
}

void MakeExtFile(void)
{
	ofstream out;
	out.open("Valid.Ext");

	out<<"N"   <<endl<<"smcN"<<endl<<"zipY"<<endl<<"gzY" <<endl<<"swcN"<<endl<<"figN"<<endl;
	out<<"058N"<<endl<<"078N"<<endl<<"japN"<<endl<<"usaN"<<endl<<"048N"<<endl;
	out<<"eurN"<<endl<<"sfcN"<<endl<<"1N"  <<endl<<"mgdN"<<endl<<"ufoN"<<endl;
	out<<"binN"<<endl<<"gd3N"<<endl<<"mghN"<<endl<<"gd7N"<<endl<<"ausN"<<endl;
	out<<"dx2N"<<endl<<"aN"<<endl<<"jmaY";
	out.close();
	SetFileAttributes("Valid.Ext", FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_READONLY);
};
#ifdef NETPLAY_SUPPORT
int CALLBACK DlgNPOptions(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	char defPort[5];
	WORD chkLength;
	if(Settings.Port==0)
	{
		_itoa(1996,defPort,10);
	}
	else
	{
		_itoa(Settings.Port,defPort,10);
	}

	switch (msg)
	{
	case WM_INITDIALOG:
		if(DirectDraw.Clipped) S9xReRefresh();
		SetWindowText(hDlg,NPOPT_TITLE);
		SetDlgItemText(hDlg,IDC_LABEL_PORTNUM,NPOPT_LABEL_PORTNUM);
		SetDlgItemText(hDlg,IDC_LABEL_PAUSEINTERVAL,NPOPT_LABEL_PAUSEINTERVAL);
		SetDlgItemText(hDlg,IDC_LABEL_PAUSEINTERVAL_TEXT,NPOPT_LABEL_PAUSEINTERVAL_TEXT);
		SetDlgItemText(hDlg,IDC_LABEL_MAXSKIP,NPOPT_LABEL_MAXSKIP);
		SetDlgItemText(hDlg,IDC_SYNCBYRESET,NPOPT_SYNCBYRESET);
		SetDlgItemText(hDlg,IDC_SENDROM,NPOPT_SENDROM);
		SetDlgItemText(hDlg,IDC_ACTASSERVER,NPOPT_ACTASSERVER);
		SetDlgItemText(hDlg,IDC_PORTNUMBLOCK,NPOPT_PORTNUMBLOCK);
		SetDlgItemText(hDlg,IDC_CLIENTSETTINGSBLOCK,NPOPT_CLIENTSETTINGSBLOCK);
		SetDlgItemText(hDlg,IDC_SERVERSETTINGSBLOCK,NPOPT_SERVERSETTINGSBLOCK);
		SetDlgItemText(hDlg,IDOK,BUTTON_OK);
		SetDlgItemText(hDlg,IDCANCEL,BUTTON_CANCEL);

		hBmp=(HBITMAP)LoadImage(NULL, TEXT("TheDumper.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		SendDlgItemMessage(hDlg, IDC_PORTNUMBERA, WM_SETTEXT, 0, (LPARAM)defPort);
		if(Settings.NetPlayServer)
		{
			SendDlgItemMessage(hDlg, IDC_ACTASSERVER, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(NPServer.SendROMImageOnConnect)
		{
			SendDlgItemMessage(hDlg, IDC_SENDROM, BM_SETCHECK, BST_CHECKED,0);
		}

		if(NPServer.SyncByReset)
		{
			SendDlgItemMessage(hDlg, IDC_SYNCBYRESET, BM_SETCHECK, BST_CHECKED,0);
		}
		SendDlgItemMessage(hDlg, IDC_MAXSPIN, UDM_SETRANGE,0,MAKELPARAM((short)60,(short)0));
		SendDlgItemMessage(hDlg, IDC_MAXSPIN, UDM_SETPOS,0,MAKELONG(NetPlay.MaxFrameSkip,0));
		SendDlgItemMessage(hDlg, IDC_PAUSESPIN, UDM_SETRANGE,0,MAKELONG(30,0));
		SendDlgItemMessage(hDlg, IDC_PAUSESPIN, UDM_SETPOS,0,MAKELONG(NetPlay.MaxBehindFrameCount,0));
		return TRUE;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint (hDlg, &ps);
			if(hBmp)
			{
				BITMAP bmp;
				ZeroMemory(&bmp, sizeof(BITMAP));
				RECT r;
				GetClientRect(hDlg, &r);
				HDC hdc=GetDC(hDlg);
				HDC hDCbmp=CreateCompatibleDC(hdc);
				GetObject(hBmp, sizeof(BITMAP), &bmp);
				HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
				StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
				SelectObject(hDCbmp, hOldBmp);
				DeleteDC(hDCbmp);
				ReleaseDC(hDlg, hdc);
			}

			EndPaint (hDlg, &ps);
		}
		return true;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				NetPlay.MaxFrameSkip=(uint32)SendDlgItemMessage(hDlg, IDC_MAXSPIN, UDM_GETPOS,0,0);
				NetPlay.MaxBehindFrameCount=(uint32)SendDlgItemMessage(hDlg, IDC_PAUSESPIN, UDM_GETPOS,0,0);
				chkLength=(WORD)SendDlgItemMessage(hDlg,IDC_PORTNUMBERA,EM_LINELENGTH,0,0);
				*((LPWORD)defPort) = chkLength;
				SendDlgItemMessage(hDlg,IDC_PORTNUMBERA,EM_GETLINE,0,(LPARAM)defPort);
				if(atoi(defPort)<1024||atoi(defPort)>65535)
				{
					MessageBox(hDlg,"Port Number needs to be betweeb 1024 and 65535","Error",MB_OK);
					break;
				}
				else
				{
					Settings.Port = atoi(defPort);
				}
				//MessageBox(hDlg,defPort,defPort,MB_OK);
				Settings.NetPlayServer = IsDlgButtonChecked(hDlg,IDC_ACTASSERVER);
				NPServer.SendROMImageOnConnect = IsDlgButtonChecked(hDlg,IDC_SENDROM);
				NPServer.SyncByReset = IsDlgButtonChecked(hDlg,IDC_SYNCBYRESET);

				EndDialog(hDlg,0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				WinSaveConfigFile();
				return TRUE;
			}
		case IDCANCEL:
			{
				EndDialog(hDlg,0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}
#endif
 
void EnableDisableKeyFields (int index, HWND hDlg)
{
 
}

void UpdateModeListBox(HWND hListView)
{
	 
}

void TranslateKey(WORD keyz,char *out)
{

}


//HWND funky;
SJoyState JoystickF [16];


#ifdef NETPLAY_SUPPORT
int CALLBACK DlgNPProgress(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendDlgItemMessage(hDlg,IDC_NPPROGRESS,PBM_SETRANGE,0,(LPARAM)MAKELPARAM (0, 100));
	SendDlgItemMessage(hDlg,IDC_NPPROGRESS,PBM_SETPOS,(WPARAM)(int)NetPlay.PercentageComplete,0);

	return false;
}
#endif
 

 

template<typename IntType>
int ScanAddress(const char* str, IntType& value)
{
	int ret = 0;
	if(tolower(*str) == 's')
	{
		ret = sscanf(str+1, "%x", &value);
		value += 0x7E0000 + 0x20000;
	}
	else if(tolower(*str) == 'i')
	{
		ret = sscanf(str+1, "%x", &value);
		value += 0x7E0000 + 0x30000;
	}
	else
	{
		int plus = (*str == '0' && tolower(str[1]) == 'x') ? 2 : 0;
		ret = sscanf(str+plus, "%x", &value);
	}
	return ret;
}

enum CheatStatus{
	Untouched,
	Deleted,
	Modified
};
typedef struct{
int* index;
DWORD* state;
}CheatTracker;

#define ITEM_QUERY(a, b, c, d, e)  ZeroMemory(&a, sizeof(LV_ITEM)); \
						a.iItem= ListView_GetSelectionMark(GetDlgItem(hDlg, b)); \
						a.iSubItem=c; \
						a.mask=LVIF_TEXT; \
						a.pszText=d; \
						a.cchTextMax=e; \
						ListView_GetItem(GetDlgItem(hDlg, b), &a);

 

#define TEST_BIT(a,v) \
((a)[(v) >> 5] & (1 << ((v) & 31)))

static inline int CheatCount(int byteSub)
{
	int a, b=0;
//	for(a=0;a<0x32000-byteSub;a++)
	for(a=0;a<0x30000-byteSub;a++) // hide IRAM from cheat dialog (it seems not to be searhed correctly)
	{
		if(TEST_BIT(Cheat.ALL_BITS, a))
			b++;
	}
	return b;
}


struct ICheat
{
    uint32  address;
    uint32  new_val;
    uint32  saved_val;
	int		size;
    bool8   enabled;
    bool8   saved;
    char    name [22];
	int format;
};

bool TestRange(int val_type, S9xCheatDataSize bytes,  uint32 value)
{
	if(val_type!=2)
	{
		if(bytes==S9X_8_BITS)
		{
			if(value<256)
				return true;
			else return false;
		}
		if(bytes==S9X_16_BITS)
		{
			if(value<65536)
				return true;
			else return false;
		}
		if(bytes==S9X_24_BITS)
		{
			if(value<16777216)
				return true;
			else return false;
		}
		//if it reads in, it's a valid 32-bit unsigned!
		return true;
	}
	else
	{
		if(bytes==S9X_8_BITS)
		{
			if((int32)value<128 && (int32)value >= -128)
				return true;
			else return false;
		}
		if(bytes==S9X_16_BITS)
		{
			if((int32)value<32768 && (int32)value >= -32768)
				return true;
			else return false;
		}
		if(bytes==S9X_24_BITS)
		{
			if((int32)value<8388608 && (int32)value >= -8388608)
				return true;
			else return false;
		}
		//should be handled by sscanf
		return true;
	}
}

 
static void set_movieinfo(const char* path, HWND hDlg)
{
 
}
 
// checks if the currently loaded ROM has an SRAM file in the saves directory that we have write access to
static bool existsSRAM ()
{
  return(!access(S9xGetFilename(".srm", SRAM_DIR), R_OK|W_OK));
}
 

// MYO
void S9xHandlePortCommand(s9xcommand_t cmd, int16 data1, int16 data2)
{
	return;
}

//  NYI
const char *S9xChooseFilename (bool8 read_only)
{
	return NULL;
}

// NYI
const char *S9xChooseMovieFilename (bool8 read_only)
{
	return NULL;
}


const char * S9xStringInput(const char *msg)
{
	return NULL;
}

void S9xToggleSoundChannel (int c)
{
	if (c == 8)
		GUI.SoundChannelEnable = 255;
    else
		GUI.SoundChannelEnable ^= 1 << c;

	S9xSetSoundControl(GUI.SoundChannelEnable);
}

bool S9xPollButton(uint32 id, bool *pressed){
	if(S9xMoviePlaying())
		return false;

	*pressed = false;

#define CHECK_KEY(controller, button) (!S9xGetState(Joypad[controller].button) || (ToggleJoypadStorage[controller].button && !TurboToggleJoypadStorage[controller].button) || (IPPU.TotalEmulatedFrames%2 == ToggleJoypadStorage[controller].button && TurboToggleJoypadStorage[controller].button))

	extern bool S9xGetState (WORD KeyIdent);
	if (id & k_MO)	// mouse
	{
		switch (id & 0xFF)
		{
			case 0: *pressed = GUI.MouseButtons & 1 /* Left */ || ((id & k_C1) && (CHECK_KEY(0,A) || CHECK_KEY(0,L))) || ((id & k_C2) && (CHECK_KEY(1,A) || CHECK_KEY(1,L))); break;
			case 1: *pressed = GUI.MouseButtons & 2 /* Right */ || ((id & k_C1) && (CHECK_KEY(0,B) || CHECK_KEY(0,R))) || ((id & k_C2) && (CHECK_KEY(1,B) || CHECK_KEY(1,R))); break;
		}
	}
	else
	if (id & k_SS)	// superscope
	{
		switch (id & 0xFF)
		{
			case 0:	*pressed = GUI.MouseX <= 0 || GUI.MouseY <= 0 || GUI.MouseX >= IPPU.RenderedScreenWidth || GUI.MouseY >= ((IPPU.RenderedScreenHeight> 256) ? SNES_HEIGHT_EXTENDED<<1 : SNES_HEIGHT_EXTENDED) || CHECK_KEY(1,X); break;
			case 2:	*pressed = (GUI.MouseButtons & 2) /* Right */ || CHECK_KEY(1,B) || CHECK_KEY(1,R) ; break;
			case 3:	*pressed = (GUI.MouseButtons & 4) /* Middle */ || GUI.superscope_turbo || CHECK_KEY(1,Y);	GUI.superscope_turbo=0; GUI.MouseButtons &= ~4; break;
			case 4:	*pressed =                                        GUI.superscope_pause || CHECK_KEY(1,Start) || CHECK_KEY(1,Select);	break;
			case 1:	*pressed = (GUI.MouseButtons & 1) /* Left */ || CHECK_KEY(1,A) || CHECK_KEY(1,L); break;
		}
	}
	else
	if (id & k_LG)	// justifier
	{
		if (id & k_C1)
		{
			switch (id & 0xFF)
			{
				case 0:	*pressed = GUI.MouseX <= 0 || GUI.MouseY <= 0 || GUI.MouseX >= IPPU.RenderedScreenWidth || GUI.MouseY >= ((IPPU.RenderedScreenHeight> 256) ? SNES_HEIGHT_EXTENDED<<1 : SNES_HEIGHT_EXTENDED) || CHECK_KEY(0,X) || CHECK_KEY(0,Start); break;
				case 1:	*pressed = GUI.MouseButtons & 1 /* Left */  || CHECK_KEY(0,A) || CHECK_KEY(0,L); break;
				case 2: *pressed = GUI.MouseButtons & 2 /* Right */  || CHECK_KEY(1,B) || CHECK_KEY(1,R); break;
			}
		}
		else
		{
			switch (id & 0xFF)
			{
				case 0: *pressed = CHECK_KEY(1,Start) /* 2p Start */  || CHECK_KEY(1,X); break;
				case 1:	*pressed = CHECK_KEY(1,A) /* 2p A */ || CHECK_KEY(1,L); break;
				case 2: *pressed = CHECK_KEY(1,B) /* 2p B */ || CHECK_KEY(1,R); break;
			}
		}
	}

	return (true);
}

// ??? NYI
bool S9xPollAxis(uint32 id, int16 *value){
    return false;
}

bool S9xPollPointer(uint32 id, int16 *x, int16 *y){
	if(S9xMoviePlaying())
		return false;

	if (id & k_PT)
	{
		*x = GUI.MouseX;
		*y = GUI.MouseY;
	}
	else
		*x = *y = 0;
	return (true);
}

// adjusts settings based on ROM that was just loaded
void S9xPostRomInit()
{
	// "Cheats are on" message if cheats are on and active,
	// to make it less likely that someone will think there is some bug because of
	// a lingering cheat they don't realize is on
	if (Settings.ApplyCheats)
	{
		extern struct SCheatData Cheat;
	    for (uint32 i = 0; i < Cheat.num_cheats; i++)
		{
	        if (Cheat.c [i].enabled)
			{
				char String2 [1024];
				sprintf(String2, "(CHEATS ARE ON!) %s", String);
				strncpy(String, String2, 512);
				break;
			}
		}
	}

	if(!S9xMovieActive() && !startingMovie)
	{
		// revert previously forced control
		if(GUI.ControlForced!=0xff)
			GUI.ControllerOption = GUI.ControlForced;
		int prevController = GUI.ControllerOption;
		GUI.ValidControllerOptions = 0xFFFF;

		// NSRT controller settings
		if (!strncmp((const char *)Memory.NSRTHeader+24, "NSRT", 4))
		{
			switch(Memory.NSRTHeader[29])
			{
				default: // unknown or unsupported
					break;
				case 0x00: // Gamepad / Gamepad
					GUI.ControllerOption = SNES_JOYPAD;
					GUI.ValidControllerOptions = (1<<SNES_JOYPAD);
					break;
				case 0x10: // Mouse / Gamepad
					GUI.ControllerOption = SNES_MOUSE;
					GUI.ValidControllerOptions = (1<<SNES_MOUSE);
					break;
				case 0x20: // Mouse_or_Gamepad / Gamepad
					if(GUI.ControllerOption == SNES_MOUSE_SWAPPED)
						GUI.ControllerOption = SNES_MOUSE;
					if(GUI.ControllerOption != SNES_MOUSE)
						GUI.ControllerOption = SNES_JOYPAD;
					GUI.ValidControllerOptions = (1<<SNES_JOYPAD) | (1<<SNES_MOUSE);
					break;
				case 0x01: // Gamepad / Mouse
					GUI.ControllerOption = SNES_MOUSE_SWAPPED;
					GUI.ValidControllerOptions = (1<<SNES_MOUSE_SWAPPED);
					break;
				case 0x22: // Mouse_or_Gamepad / Mouse_or_Gamepad
					if(GUI.ControllerOption != SNES_MOUSE && GUI.ControllerOption != SNES_MOUSE_SWAPPED)
						GUI.ControllerOption = SNES_JOYPAD;
					GUI.ValidControllerOptions = (1<<SNES_JOYPAD) | (1<<SNES_MOUSE) | (1<<SNES_MOUSE_SWAPPED);
					break;
				case 0x03: // Gamepad / Superscope
					GUI.ControllerOption = SNES_SUPERSCOPE;
					GUI.ValidControllerOptions = (1<<SNES_SUPERSCOPE);
					break;
				case 0x04: // Gamepad / Gamepad_or_Superscope
					if(GUI.ControllerOption == SNES_JUSTIFIER || GUI.ControllerOption == SNES_JUSTIFIER_2)
						GUI.ControllerOption = SNES_SUPERSCOPE;
					if(GUI.ControllerOption != SNES_SUPERSCOPE)
						GUI.ControllerOption = SNES_JOYPAD;
					GUI.ValidControllerOptions = (1<<SNES_JOYPAD) | (1<<SNES_SUPERSCOPE);
					break;
				case 0x05: // Gamepad / Justifier
					if(GUI.ControllerOption != SNES_JUSTIFIER_2)
						GUI.ControllerOption = SNES_JUSTIFIER;
					GUI.ValidControllerOptions = (1<<SNES_JUSTIFIER) | (1<<SNES_JUSTIFIER_2);
					break;
				case 0x06: // Gamepad / Multitap_or_Gamepad
					GUI.ControllerOption = SNES_MULTIPLAYER5;
					GUI.ValidControllerOptions = (1<<SNES_MULTIPLAYER5) | (1<<SNES_JOYPAD);
					break;
				case 0x66: // Multitap_or_Gamepad / Multitap_or_Gamepad
					GUI.ControllerOption = SNES_MULTIPLAYER8;
					GUI.ValidControllerOptions = (1<<SNES_MULTIPLAYER8) | (1<<SNES_MULTIPLAYER5) | (1<<SNES_JOYPAD);
					break;
				case 0x24: // Gamepad_or_Mouse / Gamepad_or_Superscope
					if(GUI.ControllerOption == SNES_JUSTIFIER || GUI.ControllerOption == SNES_JUSTIFIER_2)
						GUI.ControllerOption = SNES_SUPERSCOPE;
					if(GUI.ControllerOption != SNES_SUPERSCOPE && GUI.ControllerOption != SNES_MOUSE)
						GUI.ControllerOption = SNES_JOYPAD;
					GUI.ValidControllerOptions = (1<<SNES_JOYPAD) | (1<<SNES_MOUSE) | (1<<SNES_SUPERSCOPE);
					break;
				case 0x27: // Gamepad_or_Mouse / Gamepad_or_Mouse_or_Superscope
					if(GUI.ControllerOption == SNES_JUSTIFIER || GUI.ControllerOption == SNES_JUSTIFIER_2)
						GUI.ControllerOption = SNES_SUPERSCOPE;
					if(GUI.ControllerOption != SNES_SUPERSCOPE && GUI.ControllerOption != SNES_MOUSE && GUI.ControllerOption != SNES_MOUSE_SWAPPED)
						GUI.ControllerOption = SNES_JOYPAD;
					GUI.ValidControllerOptions = (1<<SNES_JOYPAD) | (1<<SNES_MOUSE) | (1<<SNES_MOUSE_SWAPPED) | (1<<SNES_SUPERSCOPE);
					break;
				case 0x08: // Gamepad / Mouse_or_Multitap_or_Gamepad
					if(GUI.ControllerOption == SNES_MOUSE)
						GUI.ControllerOption = SNES_MOUSE_SWAPPED;
					if(GUI.ControllerOption == SNES_MULTIPLAYER8)
						GUI.ControllerOption = SNES_MULTIPLAYER5;
					if(GUI.ControllerOption != SNES_MULTIPLAYER5 && GUI.ControllerOption != SNES_MOUSE_SWAPPED)
						GUI.ControllerOption = SNES_JOYPAD;
					GUI.ValidControllerOptions = (1<<SNES_MOUSE_SWAPPED) | (1<<SNES_MULTIPLAYER5) | (1<<SNES_JOYPAD);
					break;
			}
		}

		// update menu and remember what (if anything) the control was forced from
		ChangeInputDevice();
		GUI.ControlForced = prevController;
	}

	// reset fast-forward and other input-related GUI state
	Settings.TurboMode = FALSE;
	GUI.superscope_turbo = 0;
	GUI.superscope_pause = 0;
	GUI.MouseButtons = 0;
	GUI.MouseX = 0;
	GUI.MouseY = 0;
	GUI.TurboMask = 0;
	GUI.FrameAdvanceJustPressed = 0;

	// black out the screen
 	for (uint32 y = 0; y < (uint32)IPPU.RenderedScreenHeight; y++)
		XMemSet(GFX.Screen + y * GFX.RealPPL, 0, GFX.RealPPL*2);
}



#include "../font.h"
extern uint16* display_screen;
extern int display_ppl, display_width, display_height;
extern int display_fontwidth, display_fontheight, display_hfontaccessscale, display_vfontaccessscale;
extern bool8 display_paramsinited;

template<typename screenPtrType>
static inline void FontPixToScreen(char p, screenPtrType *s)
{
	if(p == '#')
	{
		*s = Settings.DisplayColor;
	}
	else if(p == '.')
	{
		static const screenPtrType black = BUILD_PIXEL(0,0,0);
		*s = black;
	}
}
template<>
static inline void FontPixToScreen(char p, uint32 *s)
{
#define CONVERT_16_TO_32(pixel) \
    (((((pixel) >> 11)        ) << /*RedShift+3*/  19) | \
     ((((pixel) >> 6)   & 0x1f) << /*GreenShift+3*/11) | \
      (((pixel)         & 0x1f) << /*BlueShift+3*/ 3))

	if(p == '#')
	{
		*s = CONVERT_16_TO_32(Settings.DisplayColor);
	}
	else if(p == '.')
	{
		static const uint32 black = CONVERT_16_TO_32(BUILD_PIXEL(0,0,0));
		*s = black;
	}
}

#define CHOOSE(c1) ((c1=='#'||X=='#') ? '#' : ((c1=='.'||X=='.') ? '.' : c1))

template<typename screenPtrType>
static inline void FontPixToScreenEPX(int x, int y, screenPtrType *s)
{
	const char X = font[y][x];                // E D H
	const char A = x>0  ?font[y][x-1]:' ';    // A X C
	const char C = x<143?font[y][x+1]:' ';    // F B G
	if (A != C)
	{
		const char D = y>0  ?font[y-1][x]:' ';
		const char B = y<125?font[y+1][x]:' ';
		if (B != D)
		{
			FontPixToScreen((D == A) ? CHOOSE(D) : X, s);
			FontPixToScreen((C == D) ? CHOOSE(C) : X, s+1);
			FontPixToScreen((A == B) ? CHOOSE(A) : X, s+display_ppl);
			FontPixToScreen((B == C) ? CHOOSE(B) : X, s+display_ppl+1);
			return;
		}
	}
	FontPixToScreen(X, s);
	FontPixToScreen(X, s+1);
	FontPixToScreen(X, s+display_ppl);
	FontPixToScreen(X, s+display_ppl+1);
}
#undef CHOOSE

#define CHOOSE(c1) ((X=='#') ? '#' : c1)
template<typename screenPtrType>
inline void FontPixToScreenEPXSimple3(int x, int y, screenPtrType *s)
{
	const char X = font[y][x];                // E D H
	const char A = x>0  ?font[y][x-1]:' ';    // A X C
	const char C = x<143?font[y][x+1]:' ';    // F B G
	const char D = y>0  ?font[y-1][x]:' ';
	const char B = y<125?font[y+1][x]:' ';
	const bool XnE = y>0  &&x>0  ?(X != font[y-1][x-1]):X!=' ';
	const bool XnF = y<125&&x<143?(X != font[y+1][x-1]):X!=' ';
	const bool XnG = y<125&&x>0  ?(X != font[y+1][x+1]):X!=' ';
	const bool XnH = y>0  &&x<143?(X != font[y-1][x+1]):X!=' ';
	const bool DA = D == A && (XnE || CHOOSE(D)!=X);
	const bool AB = A == B && (XnF || CHOOSE(A)!=X);
	const bool BC = B == C && (XnG || CHOOSE(B)!=X);
	const bool CD = C == D && (XnH || CHOOSE(C)!=X);
	FontPixToScreen(DA ? A : X, s);
	FontPixToScreen(X, s+1);
	FontPixToScreen(CD ? C : X, s+2);
	FontPixToScreen(X, s+display_ppl);
	FontPixToScreen(X, s+display_ppl+1);
	FontPixToScreen(X, s+display_ppl+2);
	FontPixToScreen(AB ? A : X, s+display_ppl+display_ppl);
	FontPixToScreen(X, s+display_ppl+display_ppl+1);
	FontPixToScreen(BC ? C : X, s+display_ppl+display_ppl+2);
}
#undef CHOOSE

template<typename screenPtrType>
void WinDisplayChar(screenPtrType *s, uint8 c) {
	if(c <= 32)
		return;
    int line = ((c - 32) >> 4) * display_fontheight;
    int offset = ((c - 32) & 15) * display_fontwidth;
    int h, w;
	if(!display_paramsinited) display_ppl = Settings.OpenGLEnable ? IPPU.RenderedScreenWidth : GFX.RealPPL;
	if(display_hfontaccessscale == 1 && display_vfontaccessscale == 1) {
		for(h=0; h<display_fontheight; h++, line++, s+=display_ppl-display_fontwidth)
			for(w=0; w<display_fontwidth; w++, s++)
				FontPixToScreen(font [(line)] [(offset + w)], s);
	} else if(display_hfontaccessscale == 2 && display_vfontaccessscale == 2) {
		for(h=0; h<display_fontheight; h+=2, line+=2, s+=2*display_ppl-display_fontwidth)
			for(w=0; w<display_fontwidth; w+=2, s+=2)
				FontPixToScreenEPX((offset + w)/display_hfontaccessscale, line/display_vfontaccessscale, s);
	} else if(display_hfontaccessscale == 3 && display_vfontaccessscale == 3) {
		for(h=0; h<display_fontheight; h+=3, line+=3, s+=3*display_ppl-display_fontwidth)
			for(w=0; w<display_fontwidth; w+=3, s+=3)
				FontPixToScreenEPXSimple3((offset + w)/display_hfontaccessscale, line/display_vfontaccessscale, s);
	} else {
		for(h=0; h<display_fontheight; h++, line++, s+=display_ppl-display_fontwidth)
			for(w=0; w<display_fontwidth; w++, s++)
				FontPixToScreen(font [(line)/display_vfontaccessscale] [(offset + w)/display_hfontaccessscale], s);
	}
}

template<typename screenPtrType>
static void WinDisplayStringI (const char *string, int linesFromBottom, int pixelsFromLeft, bool allowWrap)
{
	if(linesFromBottom <= 0)
		linesFromBottom = 1;

	display_ppl /= (sizeof(screenPtrType)>>1);

	screenPtrType *Screen = (screenPtrType*)display_screen // text draw position, starting on the screen
                  + pixelsFromLeft // with this much horizontal offset
//				    * (Settings.SixteenBit ? 2 : 1)
                  + (display_height - display_fontheight * linesFromBottom) // and this much vertical offset
				    * display_ppl;

    int len = strlen(string);
    int max_chars = display_width / (display_fontwidth-display_hfontaccessscale);
    int char_count = 0;
	int prev_hfont_access_scale = display_hfontaccessscale;

	// squash if it won't fit on 1 line and we're drawing greater than 1x scale and we're not allowing wrapping
	while(len > max_chars && !allowWrap && display_hfontaccessscale > 1)
	{
		display_fontwidth /= display_hfontaccessscale;
		display_hfontaccessscale--;
		display_fontwidth *= display_hfontaccessscale;

		max_chars = display_width / (display_fontwidth-display_hfontaccessscale);
	}

	// loop through and draw the characters
	for(int i = 0 ; i < len ; i++, char_count++)
	{
		if(char_count >= max_chars || (unsigned char)string[i] < 32)
		{
			if(!allowWrap)
				break;

			Screen -= /*Settings.SixteenBit ? (display_fontwidth-display_hfontaccessscale)*sizeof(uint16)*max_chars :*/ (display_fontwidth-display_hfontaccessscale)*max_chars;
			Screen += display_fontheight * display_ppl;
			if(Screen >= (screenPtrType*)display_screen + display_ppl * display_height)
				break;

			char_count -= max_chars;
		}
		if((unsigned char) string[i]<32) continue;

		WinDisplayChar(Screen, string[i]);
		Screen += /*Settings.SixteenBit ? (display_fontwidth-display_hfontaccessscale)*sizeof(uint16) :*/ (display_fontwidth-display_hfontaccessscale);
	}

	// revert temporary change to font scale, if any
	if(display_hfontaccessscale != prev_hfont_access_scale)
	{
		display_hfontaccessscale = prev_hfont_access_scale;
		display_fontwidth *= display_hfontaccessscale;
	}

	display_ppl *= (sizeof(screenPtrType)>>1);
}

static void WinDisplayString (const char *string, int linesFromBottom, int pixelsFromLeft, bool allowWrap)
{
	if(GUI.ScreenDepth == 32 && GUI.DepthConverted && !Settings.AutoDisplayMessages)
	{
		WinDisplayStringI<uint32>(string, linesFromBottom, pixelsFromLeft, allowWrap);
	}
	else
	{
		WinDisplayStringI<uint16>(string, linesFromBottom, pixelsFromLeft, allowWrap);
	}
}
