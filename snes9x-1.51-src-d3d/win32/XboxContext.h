////////////////////////////////////////////////////////////////////
//
// XboxContext.h
//
// Minimal Xbox 360 context and property definitions
// (Achievement-related code excluded)
//
////////////////////////////////////////////////////////////////////

#ifndef __XBOX_CONTEXT_H__
#define __XBOX_CONTEXT_H__

#ifdef __cplusplus
extern "C" {
#endif

//
// Title info
//

#define TITLEID_SNES360                             0xFFED0707

//
// Context ids
//
// These values are passed as the dwContextId to XUserSetContext.
//

#define CONTEXT_GAMESTATE                           0

//
// Context values
//
// These values are passed as the dwContextValue to XUserSetContext.
//

// Values for CONTEXT_GAMESTATE

#define CONTEXT_GAMESTATE_IDLE                      0
#define CONTEXT_GAMESTATE_INGAME                    1
#define CONTEXT_GAMESTATE_PAUSED                    2
#define CONTEXT_GAMESTATE_BROWSING_ROMS             3

// Values for X_CONTEXT_PRESENCE

#define CONTEXT_PRESENCE_HOMEBREW_PRESENCE          0
#define CONTEXT_PRESENCE_PRESENCE                   1

// Values for X_CONTEXT_GAME_MODE

#define CONTEXT_GAME_MODE_HOMEBREW_MODE             0

//
// Property ids
//
// These values are passed as the dwPropertyId value to XUserSetProperty
// and as the dwPropertyId value in the XUSER_PROPERTY structure.
//

#define PROPERTY_NUM_ROMS                           0x10000002

#ifdef __cplusplus
}
#endif

#endif /* __XBOX_CONTEXT_H__ */

