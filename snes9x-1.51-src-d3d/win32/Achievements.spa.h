////////////////////////////////////////////////////////////////////
//
// .\Achievements.spa.h
//
// Auto-generated on Thursday, 06 November 2025 at 15:00:35
// XLAST project version 1.0.148.0
// SPA Compiler version 2.0.9328.0
//
////////////////////////////////////////////////////////////////////

#ifndef __SNES360_SPA_H__
#define __SNES360_SPA_H__

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

//
// Achievement ids
//
// These values are used in the dwAchievementId member of the
// XUSER_ACHIEVEMENT structure that is used with
// XUserWriteAchievements and XUserCreateAchievementEnumerator.
//

#define ACHIEVEMENT_HOMEBREW_UNLOCKED               1
#define ACHIEVEMENT_SUPER_METROID                   2
#define ACHIEVEMENT_SUPER_MARIO_KART                3
#define ACHIEVEMENT_SUPER_PUNCH_OUT                 4
#define ACHIEVEMENT_FREEXEX_DISS                    5
#define ACHIEVEMENT_LAUNCH_TITLE                    6
#define ACHIEVEMENT_CHEAT                           7
#define ACHIEVEMENT_BIGONE                          8
#define ACHIEVEMENT_TURBO                           9
#define ACHIEVEMENT_50_CLUB                         10
#define ACHIEVEMENT_100_CLUB                        11
#define ACHIEVEMENT_LEAKER                          12

//
// AvatarAssetAward ids
//


//
// Stats view ids
//
// These are used in the dwViewId member of the XUSER_STATS_SPEC structure
// passed to the XUserReadStats* and XUserCreateStatsEnumerator* functions.
//

// Skill leaderboards for ranked game modes

#define STATS_VIEW_SKILL_RANKED_HOMEBREW_MODE       0xFFFF0000

// Skill leaderboards for unranked (standard) game modes

#define STATS_VIEW_SKILL_STANDARD_HOMEBREW_MODE     0xFFFE0000

// Title defined leaderboards


//
// Stats view column ids
//
// These ids are used to read columns of stats views.  They are specified in
// the rgwColumnIds array of the XUSER_STATS_SPEC structure.  Rank, rating
// and gamertag are not retrieved as custom columns and so are not included
// in the following definitions.  They can be retrieved from each row's
// header (e.g., pStatsResults->pViews[x].pRows[y].dwRank, etc.).
//

//
// Matchmaking queries
//
// These values are passed as the dwProcedureIndex parameter to
// XSessionSearch to indicate which matchmaking query to run.
//


//
// Gamer pictures
//
// These ids are passed as the dwPictureId parameter to XUserAwardGamerTile.
//



#ifdef __cplusplus
}
#endif

#endif // __SNES360_SPA_H__


