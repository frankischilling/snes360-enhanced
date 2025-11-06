#include<xtl.h>
#include<d3d9.h>
#include<xam.h>

#pragma once
#ifndef STORAGE_C
#define STORAGE_C

static const DWORD  MAX_DATA_COUNT = 10;



//--------------------------------------------------------------------------------------
// Save root name to use. This can be any valid directory name.
//--------------------------------------------------------------------------------------
static CHAR g_szSaveRoot[] = "save";

//--------------------------------------------------------------------------------------
// Dummy file name to use for the save game.
//--------------------------------------------------------------------------------------
static CHAR g_szSaveGame[] = "save:\\savegame.txt";

static CHAR g_szThumbnailImage[] = "game:\\saveicon.png";

static DWORD  m_dwMinUsers                   = 1;     
static DWORD  m_dwMaxUsers                   = 1;     
static BOOL   m_bRequireOnlineUsers          = FALSE; 
static DWORD  m_dwSignInPanes                = 1;     
static HANDLE m_hNotification                = NULL;  
static DWORD  m_dwSignedInUserMask           = 0;     
static DWORD  m_dwNumSignedInUsers           = 0;     
static DWORD  m_dwOnlineUserMask             = 0;     

static BOOL   m_bSystemUIShowing             = FALSE; 
static BOOL   m_bNeedToShowSignInUI          = FALSE; 
static BOOL   m_bMessageBoxShowing           = FALSE;
static BOOL   m_bSigninUIWasShown            = FALSE;
static XOVERLAPPED m_Overlapped              = {0};
static LPCWSTR m_pwstrButtons[2]             = { L"Exit", L"Snes360 Sign In" };
static MESSAGEBOX_RESULT m_MessageBoxResult  = {0};

// Flags that can be returned from Update()
static enum SIGNIN_UPDATE_FLAGS
{
    SIGNIN_USERS_CHANGED    = 0x00000001,
    SYSTEM_UI_CHANGED       = 0x00000002,
    CONNECTION_CHANGED      = 0x00000004
};

class GameStorage
{

public:
	HRESULT Initialise();
	void ShowDeviceUI();

    BOOL m_bDrawHelp;
    XCONTENT_DATA       m_ContentData[MAX_DATA_COUNT];  // Data containing display names
    LPDIRECT3DTEXTURE9  m_ThumbnailTextures[MAX_DATA_COUNT];
    DWORD m_dwContentDataCount;           // Number of data elements
    DWORD m_dwSelectedIndex;              // Selected element in UI
    WCHAR               m_szMessage[128];               // Message to be displayed in UI
    XOVERLAPPED m_Overlapped;                   // Overlapped object for device selector UI
    XCONTENTDEVICEID m_DeviceID;                   // Device identifier returned by device selector UI
    BOOL m_bFirstSignin;                 // Flag indicating first signin
    BOOL m_bDeviceUIActive;              // Is the device UI showing
    HANDLE m_hNotification;
    DWORD  m_dwFirstSignedInUser;
    VOID                EnumerateContentNames();
    DWORD WriteSaveGame( CONST CHAR* szFileName, CONST WCHAR* szDisplayName, CHAR *szBuffer, DWORD bytesToWrite );	
    DWORD               ReadSaveGame( XCONTENT_DATA* ContentData, char *szBuffer, DWORD bytesToRead, DWORD *bytesRead );
    BOOL                ReadThumbnail( XCONTENT_DATA* pContentData, LPDIRECT3DTEXTURE9* pThumbnailTexture );     
    BOOL                TransferContent( XCONTENT_DATA* pContentData );
    BOOL                DeleteContent( XCONTENT_DATA* pContentData );
	VOID QuerySigninStatus();
	BOOL WriteThumbnail( CONST CHAR* szFileName, CHAR* szThumbnailFile );
	DWORD SignInUpdate();
	HRESULT Update();

	HANDLE OpenStream(CONST CHAR* szFileName, CONST WCHAR* szDisplayName, XOVERLAPPED *xov, HANDLE *hEventComplete);
	HANDLE OpenStreamForRead(CONST CHAR* szFileName, CONST WCHAR* szDisplayName, XOVERLAPPED *xov, HANDLE *hEventComplete);
	DWORD WriteStream(HANDLE hFile, CHAR *szBuffer, DWORD bytesToWrite);
	DWORD ReadStream(HANDLE hFile, CHAR *szBuffer, DWORD bytesToRead);
	VOID CloseStream(HANDLE hFile, XOVERLAPPED *xov);

	VOID SetDefaultDevice();

    // Check users that are signed in
    static DWORD    GetSignedInUserCount()
    {
        return m_dwNumSignedInUsers;
    }
    static DWORD    GetSignedInUserMask()
    {
        return m_dwSignedInUserMask;
    }
    static BOOL     IsUserSignedIn( DWORD dwController )
    {
        return ( m_dwSignedInUserMask & ( 1 << dwController ) ) != 0;
    }

    static BOOL     AreUsersSignedIn()
    {
        return ( m_dwNumSignedInUsers >= m_dwMinUsers ) &&
            ( m_dwNumSignedInUsers <= m_dwMaxUsers );
    }

    // Get the first signed-in user
    DWORD    GetSignedInUser()
    {
        return m_dwFirstSignedInUser;
    }

    // Check users that are signed into live
    static DWORD    GetOnlineUserMask()
    {
        return m_dwOnlineUserMask;
    }
    static BOOL     IsUserOnline( DWORD dwController )
    {
        return ( m_dwOnlineUserMask & ( 1 << dwController ) ) != 0;
    }

    // Check the presence of system UI
    static BOOL     IsSystemUIShowing()
    {
        return m_bSystemUIShowing || m_bNeedToShowSignInUI;
    }

    // Function to reinvoke signin UI
    static VOID     ShowSignInUI()
    {
        m_bNeedToShowSignInUI = TRUE;
    }




		
};


#endif