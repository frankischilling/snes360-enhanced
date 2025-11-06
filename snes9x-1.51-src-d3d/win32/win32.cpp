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

#include <xtl.h>
#include <xui.h>

#include "Main.h"

#include "../snes9x.h"
#include "../memmap.h"
#include "../debug.h"
#include "../cpuexec.h"
#include "../ppu.h"
#include "../snapshot.h"
#include "../apu.h"
#include "../display.h"
#include "../gfx.h"
#include "../soundux.h"
#include "../movie.h"
#include "../netplay.h"

#include "wsnes9x.h"
#include "Storage.h"

#include "render.h"
#include "wlanguage.h"

#include <direct.h>

#include <io.h>
//#define DEBUGGER

#include "direct3d.h"
#include "Storage.h"
#include "RomSettings.h"

extern CDirect3D Direct3D;
extern CSnes360App app;
extern GameStorage snesStoreage;
extern CRomPathSettings romPaths;

//#define GENERATE_OFFSETS_H

//#ifdef GENERATE_OFFSETS_H
//FILE *offsets_h = NULL;
//#define main generate_offsets_h
//#define S9xSTREAM offsets_h
//#include "offsets.cpp"
//#endif

struct SJoyState Joystick [16];
uint32 joypads [8];
bool8 do_frame_adjust=false;

// avi variables
static uint8* avi_buffer = NULL;
static uint8* avi_sound_buffer = NULL;
static int avi_sound_bytes_per_sample = 0;
static int avi_sound_samples_per_update = 0;
static int avi_width = 0;
static int avi_height = 0;
static uint32 avi_skip_frames = 0;
//void Convert8To24 (SSurface *src, SSurface *dst, RECT *srect);
void Convert16To24 (SSurface *src, SSurface *dst, RECT *srect);
void DoAVIOpen(const char* filename);
void DoAVIClose(int reason);

bool ReInitSound(int mode);


void S9xWinScanJoypads ();
bool in_display_dlg = false;
extern BOOL run_loop;
extern HXUIOBJ phObj;

typedef struct
{
    uint8 red;
    uint8 green;
    uint8 blue;
} Colour;

 
void ConvertDepth (SSurface *src, SSurface *dst, RECT *);
static Colour FixedColours [256];
static uint8 palette [0x10000];

FILE *trace_fs = NULL;

int __fastcall Normalize (int cur, int min, int max)
{
    int Result = 0;

    if ((max - min) == 0)
        return (Result);

    Result = cur - min;
    Result = (Result * 200) / (max - min);
    Result -= 100;

    return (Result);
}

void S9xTextMode( void)
{
}

void S9xGraphicsMode ()
{
}

void S9xExit( void)
{
	//if(Settings.SPC7110)
	//	(*CleanUp7110)();
}

const char *S9xGetFilename (const char *ex, enum s9x_getdirtype dirtype)
{
	
    static char filename [PATH_MAX + 1];
    char dir [_MAX_DIR + 1];
    char drive [_MAX_DRIVE + 1];
    char fname [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];
   _splitpath (Memory.ROMFilename, drive, dir, fname, ext);

   string fatxfname(fname);
   if (fatxfname.length() > 38)
   {
	   fatxfname = fatxfname.substr(0,37);
   }
   _snprintf(filename, sizeof(filename),  "%s%s%s",
             S9xGetDirectory(dirtype), fatxfname.c_str(), ex);

    
    return (filename);
}




const void S9xGetLastDirectory (char* buffer, int buf_len)
{
	if(buf_len <= 0)
		return;

}

#define IS_SLASH(x) ((x) == '\\' || (x) == '/')
static char startDirectory [PATH_MAX];
static bool startDirectoryValid = false;

const char *S9xGetDirectory (enum s9x_getdirtype dirtype)
{
//	_fullpath
	if(!startDirectoryValid)
	{
		// directory from which the executable was launched
//		GetCurrentDirectory(PATH_MAX, startDirectory);

		// directory of the executable's location:
		//GetModuleFileName(NULL, startDirectory, PATH_MAX);
        for(int i=strlen(startDirectory); i>=0; i--){
            if(IS_SLASH(startDirectory[i])){
                startDirectory[i]='\0';
                break;
            }
        }

		startDirectoryValid = true;
	}

	const char* rv = startDirectory;

    switch(dirtype){
	  default:
      case DEFAULT_DIR:
	  case HOME_DIR:
		  break;

	  case SCREENSHOT_DIR:
		  rv = GUI.ScreensDir;
		  break;

      case ROM_DIR:
		  rv = GUI.RomDir;
		  break;

      case SRAM_DIR:
		  rv = GUI.SRAMFileDir;
		  break;

	  case BIOS_DIR:
		  rv = GUI.BiosDir;
		  break;

      case SPC_DIR:
		  rv = GUI.SPCDir;
		  break;

	  case PATCH_DIR:
	  case CHEAT_DIR:
		  rv = GUI.PatchDir;
		  break;

	  case SNAPSHOT_DIR:
		  rv = GUI.FreezeFileDir;
		  break;

	  case ROMFILENAME_DIR: {
			static char filename [PATH_MAX];
			strcpy(filename, Memory.ROMFilename);
			if(!filename[0])
				rv = GUI.RomDir;
			for(int i=strlen(filename); i>=0; i--){
				if(IS_SLASH(filename[i])){
					filename[i]='\0';
					break;
				}
			}
			rv = filename;
		}
		break;
    }

	mkdir(rv);

	return rv;
}

///*extern "C"*/ const char *S9xGetFilename (const char *e)
//{
//    static char filename [_MAX_PATH + 1];
//    char drive [_MAX_DRIVE + 1];
//    char dir [_MAX_DIR + 1];
//    char fname [_MAX_FNAME + 1];
//    char ext [_MAX_EXT + 1];
//
//    if (strlen (GUI.FreezeFileDir))
//    {
//        _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
//        strcpy (filename, GUI.FreezeFileDir);
//        strcat (filename, TEXT("\\"));
//        strcat (filename, fname);
//        strcat (filename, e);
//    }
//    else
//    {
//        _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
//        _makepath (filename, drive, dir, fname, e);
//    }
//
//    return (filename);
//}

const char *S9xGetFilenameInc (const char *e, enum s9x_getdirtype dirtype)
{
    static char filename [PATH_MAX + 1];
    char dir [_MAX_DIR + 1];
    char drive [_MAX_DRIVE + 1];
    char fname [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];
    unsigned int i=0;
    const char *d;

    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);

    string fatxfname(fname);
    if (fatxfname.length() > 38)
    {
	   fatxfname = fatxfname.substr(0,37);
    }
    
	_snprintf(filename, sizeof(filename), "%s%s%s",romPaths.m_PreviewPath.c_str(), fatxfname.c_str(), e);
    
    return (filename);
}

bool8 S9xOpenSnapshotFile( const char *fname, bool8 read_only, STREAM *file)
{
    char filename [_MAX_PATH + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char fn [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];

    _splitpath( fname, drive, dir, fn, ext);
    _makepath( filename, drive, dir, fn, ext[0] == '\0' ? ".000" : ext);

    if (read_only)
    {
		 
	if ((*file = OPEN_STREAM (filename, "rb")))
	    return (TRUE);
    }
    else
    {
	if ((*file = OPEN_STREAM (filename, "wb")))
	    return (TRUE);
        FILE *fs = fopen (filename, "rb");
        if (fs)
        {
            sprintf (String, "Freeze file \"%s\" exists but is read only",
                     filename);
            fclose (fs);
            S9xMessage (S9X_ERROR, S9X_FREEZE_FILE_NOT_FOUND, String);
        }
        else
        {
            sprintf (String, "Cannot create freeze file \"%s\". Directory is read-only or does not exist.", filename);

            S9xMessage (S9X_ERROR, S9X_FREEZE_FILE_NOT_FOUND, String);
        }
    }
    return (FALSE);
}

void S9xCloseSnapshotFile( STREAM file)
{
     
}

void S9xMessage (int type, int, const char *str)
{

}

/*unsigned long _interval = 10;
long _buffernos = 4;
long _blocksize = 4400;
long _samplecount = 440;
long _maxsamplecount = 0;
long _buffersize = 0;

bool StartPlaying = false;
DWORD _lastblock = 0;
bool8 pending_setup = false;
long pending_rate = 0;
bool8 pending_16bit = false;
bool8 pending_stereo = false;*/
extern uint8 *syncSoundBuffer;

//static bool8 block_signal = FALSE;
//static volatile bool8 pending_signal = FALSE;

static RECT dstRect = { 0, 512, 0, 448 };

extern unsigned long START;

void S9xSyncSpeed( void)
{
#ifdef NETPLAY_SUPPORT
    if (Settings.NetPlay)
    {
#if defined (NP_DEBUG) && NP_DEBUG == 2
        printf ("CLIENT: SyncSpeed @%d\n", timeGetTime () - START);
#endif
        S9xWinScanJoypads ();

		LONG prev;
        BOOL success;

	// Wait for heart beat from server
        if ((success = ReleaseSemaphore (GUI.ClientSemaphore, 1, &prev)) &&
            prev == 0)
        {
            // No heartbeats already arrived, have to wait for one.
            // Mop up the ReleaseSemaphore test above...
            WaitForSingleObject (GUI.ClientSemaphore, 0);

            // ... and then wait for the real sync-signal from the
            // client loop thread.
            NetPlay.PendingWait4Sync = WaitForSingleObject (GUI.ClientSemaphore, 100) != WAIT_OBJECT_0;
#if defined (NP_DEBUG) && NP_DEBUG == 2
            if (NetPlay.PendingWait4Sync)
                printf ("CLIENT: PendingWait4Sync1 @%d\n", timeGetTime () - START);
#endif
            IPPU.RenderThisFrame = TRUE;
            IPPU.SkippedFrames = 0;
        }
        else
        {
            if (success)
            {
                // Once for the ReleaseSemaphore above...
                WaitForSingleObject (GUI.ClientSemaphore, 0);
                if (prev == 4 && NetPlay.Waiting4EmulationThread)
                {
                    // Reached the lower behind count threshold - tell the
                    // server its safe to start sending sync pulses again.
                    NetPlay.Waiting4EmulationThread = FALSE;
                    S9xNPSendPause (FALSE);
                }

#if defined (NP_DEBUG) && NP_DEBUG == 2
                if (prev > 1)
                {
                    printf ("CLIENT: SyncSpeed prev: %d @%d\n", prev, timeGetTime () - START);
                }
#endif
            }
            else
            {
#ifdef NP_DEBUG
                printf ("*** CLIENT: SyncSpeed: Release failed @ %d\n", timeGetTime () - START);
#endif
            }

            // ... and again to mop up the already-waiting sync-signal
            NetPlay.PendingWait4Sync = WaitForSingleObject (GUI.ClientSemaphore, 200) != WAIT_OBJECT_0;
#if defined (NP_DEBUG) && NP_DEBUG == 2
            if (NetPlay.PendingWait4Sync)
                printf ("CLIENT: PendingWait4Sync2 @%d\n", timeGetTime () - START);
#endif

	    if (IPPU.SkippedFrames < NetPlay.MaxFrameSkip)
	    {
		IPPU.SkippedFrames++;
		IPPU.RenderThisFrame = FALSE;
	    }
	    else
	    {
		IPPU.RenderThisFrame = TRUE;
		IPPU.SkippedFrames = 0;
	    }
        }
        // Give up remainder of time-slice to any other waiting threads,
        // if they need any time, that is.
        Sleep (0);
        if (!NetPlay.PendingWait4Sync)
        {
            NetPlay.FrameCount++;
            S9xNPStepJoypadHistory ();
        }
    }
    else
#endif

    if (!Settings.TurboMode && Settings.SkipFrames == AUTO_FRAMERATE &&
		!GUI.AVIOut)
    {
		if (!do_frame_adjust)
		{
			IPPU.RenderThisFrame = TRUE;
			IPPU.SkippedFrames = 0;
		}
		else
		{
			if (IPPU.SkippedFrames < Settings.AutoMaxSkipFrames)
			{
				IPPU.SkippedFrames++;
				IPPU.RenderThisFrame = FALSE;
			}
			else
			{
				IPPU.RenderThisFrame = TRUE;
				IPPU.SkippedFrames = 0;
			}
		}
	}
    else
    {
	uint32 SkipFrames;
	if(Settings.TurboMode && !GUI.AVIOut)
		SkipFrames = Settings.TurboSkipFrames;
	else
		SkipFrames = (Settings.SkipFrames == AUTO_FRAMERATE) ? 0 : Settings.SkipFrames;
	if (++IPPU.FrameSkip >= SkipFrames)
	{
	    IPPU.FrameSkip = 0;
	    IPPU.SkippedFrames = 0;
	    IPPU.RenderThisFrame = TRUE;
	}
	else
	{
	    IPPU.SkippedFrames++;
		IPPU.RenderThisFrame = GUI.AVIOut!=0;
	}
    }
}

const char *S9xBasename (const char *f)
{
    const char *p;
    if ((p = strrchr (f, '/')) != NULL || (p = strrchr (f, '\\')) != NULL)
	return (p + 1);

#ifdef __DJGPP
    if (p = strrchr (f, SLASH_CHAR))
	return (p + 1);
#endif

    return (f);
}

bool8 S9xReadMousePosition (int which, int &x, int &y, uint32 &buttons)
{
    if (which == 0)
    {
        x = GUI.MouseX;
        y = GUI.MouseY;
        buttons = GUI.MouseButtons;
        return (TRUE);
    }

    return (FALSE);
}

bool S9xGetState (WORD KeyIdent)
{
	return true;
}

void CheckAxis (int val, int min, int max, bool &first, bool &second)
{
    if (Normalize (val, min, max) < -S9X_JOY_NEUTRAL)
    {
        second = false;
        first = true;
    }
    else
        first = false;

    if (Normalize (val, min, max) > S9X_JOY_NEUTRAL)
    {
        first = false;
        second = true;
    }
    else
        second = false;
}

void S9xWinScanJoypads ()
{
	uint8 PadState[2];
	DWORD dwPlayerIndex = 0;
	DWORD dwResult;
	XINPUT_STATE state;


	for (DWORD i=0; i< 4; i++ )
	{
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &state );

        if( dwResult == ERROR_SUCCESS )
		{ 
				PadState[0]  = 0;
				PadState[0] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER  ?  16 : 0;
				PadState[0] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER      ?  32 : 0;
				PadState[0] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_Y      ?  64 : 0;
				PadState[0] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_B      ? 128 : 0;

	            PadState[1]  = 0;
				PadState[1] |= (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT || state.Gamepad.sThumbLX > 12000 ) ?   1 : 0;
				PadState[1] |= (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT   || state.Gamepad.sThumbLX < -12000)  ?   2 : 0;
				PadState[1] |= (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN   || state.Gamepad.sThumbLY < -12000) ?   4 : 0;
				PadState[1] |= (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP     || state.Gamepad.sThumbLY > 12000)?   8 : 0;
				PadState[1] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_START  ?  16 : 0;
				PadState[1] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ?  32 : 0;
				PadState[1] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_X      ?  64 : 0;
				PadState[1] |= state.Gamepad.wButtons & XINPUT_GAMEPAD_A      ? 128 : 0;

				joypads [i] = PadState [0] | (PadState [1] << 8) | 0x80000000;

				if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB && state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB))
				{					 
					if(CPU.SRAMModified) 
					{
						S9xAutoSaveSRAM();
						CPU.SRAMModified=FALSE;
					}
					Settings.Paused = true;	
					Settings.ForcedPause = true;
					Settings.FrameAdvance = false;
				}

				if (state.Gamepad.bRightTrigger == 0xFF)
				{
					Settings.TurboMode = true;					 
				}
				else
				{					 
					Settings.TurboMode = false;					 
				}
				
		}
        else
		{
            joypads [i] = 0;
		}
	}

 
   
#ifdef NETPLAY_SUPPORT
    if (Settings.NetPlay)
	{
		// Send joypad position update to server
		S9xNPSendJoypadUpdate (joypads [GUI.NetplayUseJoypad1 ? 0 : NetPlay.Player-1]);

		// set input from network
		for (int J = 0; J < NP_MAX_CLIENTS; J++)
			joypads[J] = S9xNPGetJoypad (J);
	}
#endif */
}

PALETTEENTRY S9xPaletteEntry[256];
void S9xSetPalette( void)
{
	 
}

 
BYTE *ScreenBuf1 = NULL;
BYTE *ScreenBuf2 = NULL;
BYTE *ScreenBuffer = NULL;

static bool8 locked_surface = FALSE;
static SSurface Dst;

bool8 S9xInitUpdate (void)
{
#ifdef WIN_SKIPRENDER_OPTIMIZATION 
	// under these specific conditions, blit the game screen directly without calling any render function
	// but, this causes problems when GUI.Scale changes during a frame (which happens when previewing filters)
    if (GUI.outputMethod==DIRECTDRAW && GUI.Scale == FILTER_NONE &&
        (GUI.Stretch || !GUI.FullScreen) &&
        !GUI.NeedDepthConvert &&
        ((DirectDraw.lpDDSOffScreen2 &&
          LockSurface2 (DirectX.lpDDSOffScreen2, &Dst))))
    {
		GFX.Pitch = Dst.Pitch;
        GFX.Screen = (uint16*)Dst.Surface;
        DirectDraw.lpDDSOffScreen2->Unlock (Dst.Surface);
        locked_surface = TRUE;
    }
    else
#endif
    {
        locked_surface = FALSE;
#ifdef USE_OPENGL
        // For OpenGL lock the screen buffer size width to 512 (might be
        // overriden in S9xStartScreenRefresh) so the buffer can be uploaded
        // into texture memory with a single OpenGL call.
//        if (Settings.OpenGLEnable)
        if (OPENGL_MODE)
            GFX.RealPPL = 512 * sizeof (uint16);
        else
#endif
        GFX.RealPPL = EXT_PITCH;
        GFX.Screen = (uint16*)ScreenBuffer;
    }

        GFX.PPL = GFX.Pitch >> 1;

	return (TRUE);
}

#define RenderMethod ((Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512) ? RenderMethodHiRes : RenderMethod)

bool8 S9xContinueUpdate(int Width, int Height)
{
	// called every other frame during interlace

    SSurface Src;

    Src.Width = Width;
	if(Height%SNES_HEIGHT)
	    Src.Height = Height;
	else
	{
		if(Height==SNES_HEIGHT)
			Src.Height=SNES_HEIGHT_EXTENDED;
		else Src.Height=SNES_HEIGHT_EXTENDED<<1;
	}
    Src.Pitch = GFX.Pitch;
    Src.Surface = (BYTE*)GFX.Screen;

	Height = Src.Height;

	 
	return true;
}

extern bool in_display_dlg;

bool8 S9xDeinitUpdate (int Width, int Height/*, bool8 sixteen_bit*/)
{
    SSurface Src;
    bool PrimarySurfaceLockFailed = false;
    RECT srcRect;

    Src.Width = Width;
	if(Height%SNES_HEIGHT)
	    Src.Height = Height;
	else
	{
		if(Height==SNES_HEIGHT)
			Src.Height=SNES_HEIGHT_EXTENDED;
		else Src.Height=SNES_HEIGHT_EXTENDED<<1;
	}
    Src.Pitch = GFX.Pitch;
    Src.Surface = (BYTE*)GFX.Screen;

	const int OrigHeight = Height;
	Height = Src.Height;

 	GUI.ScreenCleared = true;

    SelectRenderMethod ();

    if (!VOODOO_MODE && !OPENGL_MODE)
    {
		// Clear some of the old SNES rendered image
		// when the resolution becomes lower in x or y,
		// otherwise the image processors (filters) might access
		// some of the old rendered data at the edges.
        {
            static int LastWidth = 0;
            static int LastHeight = 0;

            if (Width < LastWidth)
            {
                const int hh = max(LastHeight, OrigHeight);
                for (int i = 0; i < hh; i++)
                    XMemSet (GFX.Screen + i * (GFX.Pitch>>1) + Width*1, 0, 4);
            }
            if (OrigHeight < LastHeight)
			{
                const int ww = max(LastWidth, Width);
                for (int i = OrigHeight; i < LastHeight ; i++)
                    XMemSet (GFX.Screen + i * (GFX.Pitch>>1), 0, ww * 2);

				// also old clear extended height stuff from drawing surface
				if((int)Src.Height > OrigHeight)
					for (int i = OrigHeight; i < (int)Src.Height ; i++)
						XMemSet (Src.Surface + i * Src.Pitch, 0, Src.Pitch);
			}
            LastWidth = Width;
            LastHeight = OrigHeight;
        }

		Direct3D.render(Src);
    }
    else
    {
        srcRect.top    = 0;
        srcRect.bottom = Height;
        srcRect.left   = 0;
        srcRect.right  = Width;
        dstRect = srcRect;
		RenderMethod (Src, Dst, &srcRect);
		if(!Settings.AutoDisplayMessages)
			S9xDisplayMessages ((uint16*)Dst.Surface, Dst.Pitch/2, srcRect.right-srcRect.left, srcRect.bottom-srcRect.top - ((in_display_dlg && GUI.HeightExtend) ? GetFilterScale(GUI.Scale) : 0), GetFilterScale(GUI.Scale));
    }

    GUI.SizeHistory [GUI.FlipCounter % GUI.NumFlipFrames] = dstRect;
    GUI.FlipCounter++;

    return (true);
}

void InitSnes9X( void)
{
#ifdef DEBUGGER
//    extern FILE *trace;

//    trace = fopen( "SNES9X.TRC", "wt");
    freopen( "SNES9X.OUT", "wt", stdout);
    freopen( "SNES9X.ERR", "wt", stderr);

//    CPU.Flags |= TRACE_FLAG;
//    APU.Flags |= TRACE_FLAG;
#endif

//#ifdef GENERATE_OFFSETS_H
//    offsets_h = fopen ("offsets.h", "wt");
//    generate_offsets_h (0, NULL);
//    fclose (offsets_h);
//#endif

    Memory.Init();

	extern void S9xPostRomInit();
	Memory.PostRomInitFunc = S9xPostRomInit;

    ScreenBuf1 = new BYTE [EXT_PITCH * EXT_HEIGHT];
    ScreenBuf2 = new BYTE [EXT_PITCH * EXT_HEIGHT];

    ScreenBuffer = ScreenBuf1 + EXT_OFFSET;
    XMemSet (ScreenBuf1, 0, EXT_PITCH * EXT_HEIGHT);
    XMemSet (ScreenBuf2, 0, EXT_PITCH * EXT_HEIGHT);

    GFX.Pitch = EXT_PITCH;
    GFX.RealPPL = EXT_PITCH;
    GFX.Screen = (uint16*)(ScreenBuf1 + EXT_OFFSET);

    S9xSetWinPixelFormat ();
    S9xGraphicsInit();

	InitializeCriticalSection(&GUI.SoundCritSect);
 
    S9xInitAPU();
	S9xInitSound (7, Settings.Stereo, 0);

#ifdef USE_GLIDE
    Settings.GlideEnable = FALSE;
#endif

	S9xMovieInit ();

  /*  for (int C = 0; C != 16; C ++)
        Joystick[C].Attached = joyGetDevCaps (JOYSTICKID1+C, &Joystick[C].Caps,
                                              sizeof( JOYCAPS)) == JOYERR_NOERROR;*/
}
extern "C"{
void DeinitS9x()
{
	if(ScreenBuf1)
		delete [] ScreenBuf1;
	if(ScreenBuf2)
		delete [] ScreenBuf2;
	DeleteCriticalSection(&GUI.SoundCritSect);
	 
}
}
int ffs (uint32 mask)
{
    int m = 0;
    if (mask)
    {
        while (!(mask & (1 << m)))
            m++;

        return (m);
    }

    return (0);
}

void S9xSetWinPixelFormat ()
{
    extern int Init_2xSaI (uint32 BitFormat);

    S9xSetRenderPixelFormat (0);
    Init_2xSaI (565);
    GUI.NeedDepthConvert = FALSE;
	GUI.DepthConverted = !GUI.NeedDepthConvert;

    if (VOODOO_MODE)
    {
        GUI.ScreenDepth = 16;
        GUI.RedShift = 11;
        GUI.GreenShift = 5;
        GUI.BlueShift = 0;
//        Settings.SixteenBit = TRUE;
    }
    else
    if (OPENGL_MODE)
    {
        GUI.ScreenDepth = 16;
        GUI.RedShift = 11;
        GUI.GreenShift = 6;
        GUI.BlueShift = 1;
//        Settings.SixteenBit = TRUE;
	S9xSetRenderPixelFormat (0);
	Init_2xSaI (565);
    }
    else
	if (GUI.outputMethod==DIRECT3D)
    {
		Direct3D.setSnes9xColorFormat();
	}
	else
	{

      
    }


    int l = 0;
    int i;

    for (i = 0; i < 6; i++)
    {
	int r = (i * 31) / (6 - 1);
	for (int j = 0; j < 6; j++)
	{
	    int g = (j * 31) / (6 - 1);
	    for (int k = 0; k < 6; k++)
	    {
		int b = (k * 31) / (6 - 1);

		FixedColours [l].red = r << 3;
		FixedColours [l].green = g << 3;
		FixedColours [l++].blue = b << 3;
	    }
	}
    }

    int *color_diff = new int [0x10000];
    int diffr, diffg, diffb, maxdiff = 0, won = 0, lost;
    int r, d = 8;
    for (r = 0; r <= (int) MAX_RED; r++)
    {
	int cr, g, q;

	int k = 6 - 1;
	cr = (r * k) / MAX_RED;
	q  = (r * k) % MAX_RED;
	if (q > d && cr < k)
	    cr++;
	diffr = abs (cr * k - r);
	for (g = 0; g <= (int) MAX_GREEN; g++)
	{
	    int cg, b;

	    k  = 6 - 1;
	    cg = (g * k) / MAX_GREEN;
	    q  = (g * k) % MAX_GREEN;
	    if(q > d && cg < k)
		cg++;
	    diffg = abs (cg * k - g);
	    for (b = 0; b <= (int) MAX_BLUE; b++)
	    {
		int cb;
		int rgb = BUILD_PIXEL2(r, g, b);

		k  = 6 - 1;
		cb = (b * k) / MAX_BLUE;
		q  = (b * k) % MAX_BLUE;
		if (q > d && cb < k)
		    cb++;
		diffb = abs (cb * k - b);
		palette[rgb] = (cr * 6 + cg) * 6 + cb;
		color_diff[rgb] = diffr + diffg + diffb;
		if (color_diff[rgb] > maxdiff)
		    maxdiff = color_diff[rgb];
	    }
	}
    }

    while (maxdiff > 0 && l < 256)
    {
	int newmaxdiff = 0;
	lost = 0; won++;
	for (r = MAX_RED; r >= 0; r--)
	{
	    int g;

	    for (g = MAX_GREEN; g >= 0; g--)
	    {
		int b;

		for (b = MAX_BLUE; b >= 0; b--)
		{
		    int rgb = BUILD_PIXEL2(r, g, b);

		    if (color_diff[rgb] == maxdiff)
		    {
			if (l >= 256)
			    lost++;
			else
			{
			    FixedColours [l].red = r << 3;
			    FixedColours [l].green = g << 3;
			    FixedColours [l].blue = b << 3;
			    palette [rgb] = l++;
			}
			color_diff[rgb] = 0;
		    }
		    else
			if (color_diff[rgb] > newmaxdiff)
			    newmaxdiff = color_diff[rgb];

		}
	    }
	}
	maxdiff = newmaxdiff;
    }
    delete [] color_diff;
}

void Convert8To32 (SSurface *src, SSurface *dst, RECT *srect)
{
    uint32 brightness = IPPU.MaxBrightness >> 1;
    uint32 conv [256];
    int height = srect->bottom - srect->top;
    int width = srect->right - srect->left;
    int offset1 = srect->top * src->Pitch + srect->left;
    int offset2 = 0;//((dst->Height - height) >> 1) * dst->Pitch +
//        ((dst->Width - width) >> 1) * sizeof (uint32);

    for (int p = 0; p < 256; p++)
    {
        uint32 pixel = PPU.CGDATA [p];
        conv [p] = (((pixel & 0x1f) * brightness) << GUI.RedShift) |
                   ((((pixel >> 5) & 0x1f) * brightness) << GUI.GreenShift) |
                   ((((pixel >> 10) & 0x1f) * brightness) << GUI.BlueShift);
    }
    for (register int y = 0; y < height; y++)
    {
        register uint8 *s = ((uint8 *) src->Surface + y * src->Pitch + offset1);
        register uint32 *d = (uint32 *) ((uint8 *) dst->Surface +
                                         y * dst->Pitch + offset2);
        for (register int x = 0; x < width; x++)
            *d++ = conv [PPU.CGDATA [*s++]];
    }
}

void Convert16To32 (SSurface *src, SSurface *dst, RECT *srect)
{
    int height = srect->bottom - srect->top;
    int width = srect->right - srect->left;
    int offset1 = srect->top * src->Pitch + srect->left * 2;
    int offset2 = 0;//((dst->Height - height) >> 1) * dst->Pitch +
        //((dst->Width - width) >> 1) * sizeof (uint32);

    for (register int y = 0; y < height; y++)
    {
        register uint16 *s = (uint16 *) ((uint8 *) src->Surface + y * src->Pitch + offset1);
        register uint32 *d = (uint32 *) ((uint8 *) dst->Surface +
                                         y * dst->Pitch + offset2);
        for (register int x = 0; x < width; x++)
        {
            uint32 pixel = *s++;
            *d++ = (((pixel >> 11) & 0x1f) << GUI.RedShift) |
                   (((pixel >> 6) & 0x1f) << GUI.GreenShift) |
                   ((pixel & 0x1f) << GUI.BlueShift);
        }
    }
}

//void Convert8To24 (SSurface *src, SSurface *dst, RECT *srect)
//{
//    uint32 brightness = IPPU.MaxBrightness >> 1;
//    uint8 levels [32];
//    int height = srect->bottom - srect->top;
//    int width = srect->right - srect->left;
//    int offset1 = srect->top * src->Pitch + srect->left;
//    int offset2 = ((dst->Height - height) >> 1) * dst->Pitch +
//        ((dst->Width - width) >> 1) * 3;
//
//    for (int l = 0; l < 32; l++)
//	levels [l] = l * brightness;
//
//    for (register int y = 0; y < height; y++)
//    {
//        register uint8 *s = ((uint8 *) src->Surface + y * src->Pitch + offset1);
//        register uint8 *d = ((uint8 *) dst->Surface + y * dst->Pitch + offset2);
//
//#ifdef LSB_FIRST
//        if (GUI.RedShift < GUI.BlueShift)
//#else
//	if (GUI.RedShift > GUI.BlueShift)
//#endif
//        {
//            // Order is RGB
//            for (register int x = 0; x < width; x++)
//            {
//                uint16 pixel = PPU.CGDATA [*s++];
//                *(d + 0) = levels [(pixel & 0x1f)];
//                *(d + 1) = levels [((pixel >> 5) & 0x1f)];
//                *(d + 2) = levels [((pixel >> 10) & 0x1f)];
//                d += 3;
//            }
//        }
//        else
//        {
//            // Order is BGR
//            for (register int x = 0; x < width; x++)
//            {
//                uint16 pixel = PPU.CGDATA [*s++];
//                *(d + 0) = levels [((pixel >> 10) & 0x1f)];
//                *(d + 1) = levels [((pixel >> 5) & 0x1f)];
//                *(d + 2) = levels [(pixel & 0x1f)];
//                d += 3;
//            }
//        }
//    }
//}

void Convert16To24 (SSurface *src, SSurface *dst, RECT *srect)
{
    const int height = srect->bottom - srect->top;
    const int width = srect->right - srect->left;
    const int offset1 = srect->top * src->Pitch + srect->left * 2;
    const int offset2 = ((dst->Height - height) >> 1) * dst->Pitch + ((dst->Width - width) >> 1) * 3;
	const int snesWidth = src->Width;
	const int snesHeight = src->Height;
	const bool doubledX = (snesWidth >= width*2) ? true : false;
	const bool doubledY = (snesHeight >= height*2) ? true : false;

    for (int y = 0; y < height; y++)
    {
		register uint16 *s = (uint16 *) ((uint8 *) src->Surface + (doubledY ? y*2 : y) * src->Pitch + offset1);
        register uint8 *d = ((uint8 *) dst->Surface + y * dst->Pitch + offset2);

		#define Interp(c1, c2) \
			(c1 == c2) ? c1 : \
			(((((c1 & 0x07E0)      + (c2 & 0x07E0)) >> 1) & 0x07E0) + \
			((((c1 & 0xF81F)      + (c2 & 0xF81F)) >> 1) & 0xF81F))

		if(y >= snesHeight)
		{
			// beyond SNES image - make the row black
			XMemSet(d, 0, width*3);
		}
		else

#ifdef LSB_FIRST
		if (GUI.RedShift < GUI.BlueShift)
#else
		if (GUI.RedShift > GUI.BlueShift)
#endif
        {
			// Order is RGB
			if(!doubledX)
			{
				for (int x = 0; x < width; x++)
				{
					uint32 pixel = *s++;
					*(d + 0) = (pixel >> (11 - 3)) & 0xf8;
					*(d + 1) = (pixel >> (6 - 3)) & 0xf8;
					*(d + 2) = (pixel & 0x1f) << 3;
					d += 3;
				}
			}
			else // high-res x, blend:
			{
				for (int x = 0; x < width; x++)
				{
					uint32 pixel = Interp(s[0],s[1]);
					s += 2;
					*(d + 0) = (pixel >> (11 - 3)) & 0xf8;
					*(d + 1) = (pixel >> (6 - 3)) & 0xf8;
					*(d + 2) = (pixel & 0x1f) << 3;
					d += 3;
				}
			}
        }
        else
        {
			// Order is BGR
			if(!doubledX)
			{
				for (int x = 0; x < width; x++)
				{
					uint32 pixel = *s++;
					*(d + 0) = (pixel & 0x1f) << 3;
					*(d + 1) = (pixel >> (6 - 3)) & 0xf8;
					*(d + 2) = (pixel >> (11 - 3)) & 0xf8;
					d += 3;
				}
			}
			else // high-res x, blend:
			{
				for (int x = 0; x < width; x++)
				{
					uint32 pixel = Interp(s[0],s[1]);
					s += 2;
					*(d + 0) = (pixel & 0x1f) << 3;
					*(d + 1) = (pixel >> (6 - 3)) & 0xf8;
					*(d + 2) = (pixel >> (11 - 3)) & 0xf8;
					d += 3;
				}
			}
        }
    }
}

void Convert16To8 (SSurface *src, SSurface *dst, RECT *srect)
{
    int height = srect->bottom - srect->top;
    int width = srect->right - srect->left;
    int offset1 = srect->top * src->Pitch + srect->left * 2;
    int offset2 = 0;//((dst->Height - height) >> 1) * dst->Pitch +
        //((dst->Width - width) >> 1);

    for (register int y = 0; y < height; y++)
    {
        register uint16 *s = (uint16 *) ((uint8 *) src->Surface + y * src->Pitch + offset1);
        register uint8 *d = ((uint8 *) dst->Surface + y * dst->Pitch + offset2);

        for (register int x = 0; x < width; x++)
            *d++ = palette [*s++];
    }
}

void Convert8To16 (SSurface *src, SSurface *dst, RECT *srect)
{
    uint32 levels [32];
    uint32 conv [256];
    int height = srect->bottom - srect->top;
    int width = srect->right - srect->left;
    int offset1 = srect->top * src->Pitch + srect->left;
    int offset2 = 0;//((dst->Height - height) >> 1) * dst->Pitch +
        //((dst->Width - width) >> 1) * sizeof (uint16);

    for (int l = 0; l < 32; l++)
	levels [l] = (l * IPPU.MaxBrightness) >> 4;

    for (int p = 0; p < 256; p++)
    {
        uint32 pixel = PPU.CGDATA [p];

        conv [p] = (levels [pixel & 0x1f] << GUI.RedShift) |
                   (levels [(pixel >> 5) & 0x1f] << GUI.GreenShift) |
                   (levels [(pixel >> 10) & 0x1f] << GUI.BlueShift);
    }
    for (register int y = 0; y < height; y++)
    {
        register uint8 *s = ((uint8 *) src->Surface + y * src->Pitch + offset1);
        register uint16 *d = (uint16 *) ((uint8 *) dst->Surface +
                                         y * dst->Pitch + offset2);
        for (register int x = 0; x < width; x += 2)
        {
            *(uint32 *) d = conv [*s] | (conv [*(s + 1)] << 16);
            s += 2;
            d += 2;
        }
    }
}

void ConvertDepth (SSurface *src, SSurface *dst, RECT *srect)
{
    // SNES image has been rendered in 16-bit, RGB565 format
    switch (GUI.ScreenDepth)
    {
        case 8:
            Convert16To8 (src, dst, srect);
            break;
        case 15: // is this right?
        case 16:
            break;
        case 24:
            Convert16To24 (src, dst, srect);
            break;
        case 32:
            Convert16To32 (src, dst, srect);
            break;
    }

	//srect->left = (dst->Width - src->Width) >> 1;
 //   srect->right = srect->left + src->Width;
 //   srect->top = (dst->Height - src->Height) >> 1;
 //   srect->bottom = srect->top + src->Height;
}

void S9xAutoSaveSRAM ()
{
    Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
}

void S9xSetPause (uint32 mask)
{
    Settings.ForcedPause |= mask;
	S9xSetSoundMute(TRUE);
}

void S9xClearPause (uint32 mask)
{
    Settings.ForcedPause &= ~mask;
    if (!Settings.ForcedPause)
    {
        // Wake up the main loop thread just if its blocked in a GetMessage call.
        
    }
}

static int S9xCompareSDD1IndexEntries (const void *p1, const void *p2)
{
    return (*(uint32 *) p1 - *(uint32 *) p2);
}

void S9xLoadSDD1Data ()
{
    char filename [_MAX_PATH + 1];
    char index [_MAX_PATH + 1];
    char data [_MAX_PATH + 1];

	Settings.SDD1Pack=FALSE;
    Memory.FreeSDD1Data ();

    if (strncmp (Memory.ROMName, TEXT("Star Ocean"), 10) == 0)
	{
		if(strlen(GUI.StarOceanPack)!=0)
			strcpy(filename, GUI.StarOceanPack);
		else Settings.SDD1Pack=TRUE;
	}
    else if(strncmp(Memory.ROMName, TEXT("STREET FIGHTER ALPHA2"), 21)==0)
	{
		if(Memory.ROMRegion==1)
		{
			if(strlen(GUI.SFA2NTSCPack)!=0)
				strcpy(filename, GUI.SFA2NTSCPack);
			else Settings.SDD1Pack=TRUE;
		}
		else
		{
			if(strlen(GUI.SFA2PALPack)!=0)
				strcpy(filename, GUI.SFA2PALPack);
			else Settings.SDD1Pack=TRUE;
		}
	}
	else
	{
		if(strlen(GUI.SFZ2Pack)!=0)
			strcpy(filename, GUI.SFZ2Pack);
		else Settings.SDD1Pack=TRUE;
	}

	if(Settings.SDD1Pack==TRUE)
		return;

    strcpy (index, filename);
    strcat (index, "\\SDD1GFX.IDX");
    strcpy (data, filename);
    strcat (data, "\\SDD1GFX.DAT");

    FILE *fs = fopen (index, "rb");
    int len = 0;

    if (fs)
    {
        // Index is stored as a sequence of entries, each entry being
        // 12 bytes consisting of:
        // 4 byte key: (24bit address & 0xfffff * 16) | translated block
        // 4 byte ROM offset
        // 4 byte length
        fseek (fs, 0, SEEK_END);
        len = ftell (fs);
        rewind (fs);
        Memory.SDD1Index = (uint8 *) XMemAlloc (len, g_dwPhysicalMemoryAttributes);
        fread (Memory.SDD1Index, 1, len, fs);
        fclose (fs);
        Memory.SDD1Entries = len / 12;

        if (!(fs = fopen (data, "rb")))
        {
            free ((char *) Memory.SDD1Index);
            Memory.SDD1Index = NULL;
            Memory.SDD1Entries = 0;
        }
        else
        {
            fseek (fs, 0, SEEK_END);
            len = ftell (fs);
            rewind (fs);
            Memory.SDD1Data = (uint8 *) XMemAlloc (len, g_dwPhysicalMemoryAttributes);
            fread (Memory.SDD1Data, 1, len, fs);
            fclose (fs);

            qsort (Memory.SDD1Index, Memory.SDD1Entries, 12,
                   S9xCompareSDD1IndexEntries);
        }
    }
}

bool JustifierOffscreen()
{
	return (bool)((GUI.MouseButtons&2)!=0);
}

//void JustifierButtons(uint32& justifiers)
//{
//	if(IPPU.Controller==SNES_JUSTIFIER_2)
//	{
//		if((GUI.MouseButtons&1)||(GUI.MouseButtons&2))
//		{
//			justifiers|=0x00200;
//		}
//		if(GUI.MouseButtons&4)
//		{
//			justifiers|=0x00800;
//		}
//	}
//	else
//	{
//		if((GUI.MouseButtons&1)||(GUI.MouseButtons&2))
//		{
//			justifiers|=0x00100;
//		}
//		if(GUI.MouseButtons&4)
//		{
//			justifiers|=0x00400;
//		}
//	}
//}

#ifdef MK_APU_RESAMPLE
void ResampleTo16000HzM16(uint16* input, uint16*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i++)
	{
		output[i]=(input[i*2]+input[(2*i)+1])>>1;
	}
}

void ResampleTo16000HzS16(uint16* input, uint16*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i+=2)
	{
		output[i]=(input[i*2]+input[(2*(i+1))])>>1;
		output[i+1]=(input[(i*2)+1]+input[(2*(i+1))+1])>>1;
	}
}
void ResampleTo8000HzM16(uint16* input, uint16*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i++)
	{
		output[i]=(input[i*4]+input[(4*i)+1]+input[(4*i)+2]+input[(4*i)+3])>>2;
	}
}

void ResampleTo8000HzS16(uint16* input, uint16*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i+=2)
	{
		output[i]=(input[i*4]+input[(4*i)+2]+input[(4*(i+1))]+input[(4*(i+1))+2])>>2;
		output[i+1]=(input[(i*4)+1]+input[(4*i)+3]+input[(4*(i+1))+1]+input[(4*(i+1))+3])>>2;
	}
}

void ResampleTo16000HzM8(uint8* input, uint8*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i++)
	{
		output[i]=(input[i*2]+input[(2*i)+1])>>1;
	}
}

void ResampleTo16000HzS8(uint8* input, uint8*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i+=2)
	{
		output[i]=(input[i*2]+input[(2*(i+1))])>>1;
		output[i+1]=(input[(i*2)+1]+input[(2*(i+1))+1])>>1;
	}
}
void ResampleTo8000HzM8(uint8* input, uint8*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i++)
	{
		output[i]=(input[i*4]+input[(4*i)+1]+input[(4*i)+2]+input[(4*i)+3])>>2;
	}
}

void ResampleTo8000HzS8(uint8* input, uint8*output,int output_samples)
{
	int i=0;
	for(i=0;i<output_samples;i+=2)
	{
		output[i]=(input[i*4]+input[(4*i)+2]+input[(4*(i+1))]+input[(4*(i+1))+2])>>2;
		output[i+1]=(input[(i*4)+1]+input[(4*i)+3]+input[(4*(i+1))+1]+input[(4*(i+1))+3])>>2;
	}
}

#endif

void DoAVIOpen(const char* filename)
{
	 
}

void DoAVIClose(int reason)
{
 
}