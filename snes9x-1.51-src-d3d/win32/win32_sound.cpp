#include "IS9xSoundOutput.h"
#include "../soundux.h"
#include "../snes9x.h"
#include "../apu.h"
#include "wsnes9x.h"
#include "CXAudio2.h"
// FMOD and FMOD Ex cannot be used at the same time
#ifdef FMOD_SUPPORT
#include "CFMOD.h"
#pragma comment(linker,"/DEFAULTLIB:fmodvc.lib")
#elif defined FMODEX_SUPPORT
#include "CFMODEx.h"
#pragma comment(linker,"/DEFAULTLIB:fmodexp_vc.lib")
#endif

// available sound output methods
CXAudio2 S9xXAudio2;
// FMOD and FMOD Ex cannot be used at the same time
#ifdef FMOD_SUPPORT
CFMOD S9xFMOD;
#elif defined FMODEX_SUPPORT
CFMODEx S9xFMODEx;	// FMOD Ex is currently unusable
#endif

// Interface used to access the sound output
IS9xSoundOutput *S9xSoundOutput;

// vars for S9XGenerateSound
int _samplecount;
uint8 *syncSoundBuffer;


// Wrapper function for S9xMixSamples that takes care of its upper limit
void S9xMixSamplesNoLimit(uint8 *buffer, int sample_count)
{
	int max_mixSampleCount = (Settings.SixteenBitSound ? SOUND_BUFFER_SIZE / 2 : SOUND_BUFFER_SIZE);
	while(sample_count > max_mixSampleCount) {
		S9xMixSamples(buffer,max_mixSampleCount);
		buffer += MAX_BUFFER_SIZE;
		sample_count-=max_mixSampleCount;
	}
	S9xMixSamples(buffer,sample_count);
}

/*  ReInitSound
reinitializes the sound core with current settings
IN:
mode		-	0 disables sound output, 1 enables
-----
returns true if successful, false otherwise
*/
bool ReInitSound(int mode)
{
	S9xSoundOutput->DeInitSoundOutput();
	if(mode)
		return S9xInitSound(mode,Settings.Stereo,0);
	else
		return true;
}

/*  SetupSound
applies current sound settings that do not require a reinit
these are currently only buffersize and playback rate changes
-----
returns true if successful, false otherwise
*/
bool SetupSound (void)
{
	S9xSetPlaybackRate(Settings.SoundPlaybackRate);
	// we get the temp buffer for sound synchronization from the output object
	return S9xSoundOutput->SetupSound(&syncSoundBuffer,&_samplecount);
}

/*  S9xOpenSoundDevice
called by S9xInitSound - initializes the currently selected sound output and
applies the current sound settings
IN:
mode		-	unused
pStereo		-	unused
BufferSize	-	unused
-----
returns true if successful, false otherwise
*/
bool8 S9xOpenSoundDevice (int mode, bool8 pStereo, int BufferSize)
{
	// point the interface to the correct output object
    S9xSoundOutput = &S9xXAudio2;
	 
	if(!S9xSoundOutput->InitSoundOutput())
		return false;
	if(Settings.Mute || !Settings.APUEnabled)
		return true;
	return SetupSound();
}

#define FIXED_POINT 0x10000
#define FIXED_POINT_SHIFT 16
#define FIXED_POINT_REMAINDER 0xffff

/*  S9xGenerateSound
called by the sound core if Settings.SoundSync is enabled - synchronizes access
to the temp buffer with a critical section
if the sound output does not offer sound sync then syncSoundBuffer will be NULL and
S9xGenerateSound returns immediately
*/
extern "C" void S9xGenerateSound(void)
{
    if (!syncSoundBuffer || so.samples_mixed_so_far >= _samplecount ||
        so.mute_sound || GUI.AVIOut
		)
		return;
	
	if(!TryEnterCriticalSection(&GUI.SoundCritSect))
		return;
	so.err_counter += so.err_rate;
    if (so.err_counter >= FIXED_POINT)
    {
		int sample_count = so.err_counter >> FIXED_POINT_SHIFT;
		int byte_offset;

        so.err_counter &= FIXED_POINT_REMAINDER;
		if (so.stereo)
			sample_count <<= 1;
        if (so.samples_mixed_so_far + sample_count > _samplecount)
            sample_count = _samplecount - so.samples_mixed_so_far;

		byte_offset = so.sixteen_bit?so.samples_mixed_so_far << 1:so.samples_mixed_so_far;

		S9xMixSamplesNoLimit(syncSoundBuffer + byte_offset,sample_count);
		so.samples_mixed_so_far += sample_count;
    }
	LeaveCriticalSection(&GUI.SoundCritSect);
}