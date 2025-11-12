#include <xmp.h>
#include "RomList.h"
#include "Storage.h"
#include "Main.h"
#include "RomSettings.h"
#include <set>
//#include "Snes 360.spa.h"  // Achievements disabled
#include "XboxContext.h"  // Minimal context/property definitions (achievements excluded)
 

extern int RunEmulation(char *path, char *RomName);
extern BOOL IsCurrentlyInGame;
extern HXUIOBJ phObj;
extern HXUIOBJ hScene; 
extern HXUIOBJ hMainScene;
extern HXUIOBJ hRomListScene;
char szRoms[MAX_PATH]; 
char szRomPath[MAX_PATH];
extern GameStorage snesStoreage;
extern CRomPathSettings romPaths;

wchar_t DeviceText[60];
std::vector<std::string> m_ListData;
map<string,string>::iterator romPath;
std::string ReplaceCharInString(const std::string & source, char charToReplace, const std::string replaceString);
bool g_bShowFavoritesOnly = false;  // Flag to show only favorites

// Static instance pointer for per-frame updates
static CRomListScene* g_RomListSceneInstance = NULL;


void SaveConfig(void)
{

	HANDLE hEventComplete = NULL;
	XOVERLAPPED xov = {0};

	strcpy(Settings.romPath, szRomPath);
	hEventComplete = CreateEvent( NULL, FALSE, FALSE, NULL );

	xov.hEvent = hEventComplete;
	
	HANDLE stream = snesStoreage.OpenStream("Snes360Config.dat", L"Snes 360 Configuration", &xov, &hEventComplete);
	
	if (stream)
	{
		snesStoreage.WriteStream(stream, (char *)&Settings, sizeof(Settings));
		snesStoreage.CloseStream(stream, &xov);
		CloseHandle(hEventComplete);
		snesStoreage.WriteThumbnail("Snes360Config.dat", "game:\\media\\saveicon.png" );
	}

}

void InRescanRomsFirstFunc(XUIMessage *pMsg, InRescanRomsStruct* pData, char *szPath)
{
    pData->szPath = szPath;
    XuiMessage(pMsg,XM_MESSAGE_ON_RESCAN_ROMS);
    _XuiMessageExtra(pMsg,(XUIMessageData*) pData, sizeof(*pData));

	
}


// Handler for the XM_NOTIFY message
HRESULT CRomListScene::OnNotifyPress( HXUIOBJ hObjPressed, 
       BOOL& bHandled )
    {
		int nIndex;

        if ( hObjPressed == m_RomList )
        {
			XMPPause( NULL );
			nIndex = m_RomList.GetCurSel();
						
			XuiSceneCreate( L"file://game:/media/Snes360.xzp#..\\Xbox\\Skin\\", L"InGameOptions.xur", NULL, &hScene );
			this->NavigateForward(hScene);			
			
			RunEmulation((char *)romPath->second.c_str(), (char *)m_ListData[nIndex].c_str()); 	

			SaveConfig();

			XuiDestroyObject( hScene );
			XMPContinue( NULL ); 
			bHandled = TRUE;
			return S_OK;
			
        }
		else if (hObjPressed == m_NextDevice)
		{
			XuiImageElementSetImagePath(m_PreviewImage.m_hObj, L"");

			romPath++;

			if (romPath == romPaths.GetDeviceMapEnd())
			{
				romPath = romPaths.GetDeviceMapBegin();
			}


			XUIMessage xuiMsg;
			InRescanRomsStruct msgData;
			InRescanRomsFirstFunc( &xuiMsg, &msgData, (char *)romPath->second.c_str() );
			XuiSendMessage( m_RomList.m_hObj, &xuiMsg );

			strcpy((char *)szRomPath, romPath->first.c_str());
			swprintf_s(DeviceText, L"Current Device : %S", szRomPath);
			m_DeviceText.SetText(DeviceText);			
			m_RomList.SetCurSel(0);			 
			
			SaveConfig();

			return S_OK;

		}
		else if (hObjPressed == m_AddToFavorites)
		{
			nIndex = m_RomList.GetCurSel();
			if (nIndex >= 0 && nIndex < (int)m_ListData.size())
			{
				string romName = m_ListData[nIndex];
				
				// Toggle favorite status
				if (romPaths.IsFavorite(romName))
				{
					romPaths.RemoveFavorite(romName);
					const WCHAR * button_text = L"OK";
					ShowMessageBoxEx(NULL, NULL, L"Favorites", L"Removed from favorites", 1, (LPCWSTR*)&button_text, NULL, XUI_MB_CENTER_ON_PARENT, NULL);
				}
				else
				{
					romPaths.AddFavorite(romName);
					const WCHAR * button_text = L"OK";
					ShowMessageBoxEx(NULL, NULL, L"Favorites", L"Added to favorites", 1, (LPCWSTR*)&button_text, NULL, XUI_MB_CENTER_ON_PARENT, NULL);
				}
			}
			
			bHandled = TRUE;
			return S_OK;
		}

		if( XuiControlIsBackButton( hObjPressed ) )
		{
			// Reset favorites flag when going back
			g_bShowFavoritesOnly = false;
			this->NavigateBack();

		}
		
 
		 
        bHandled = TRUE;
        return S_OK;
    }


HRESULT CRomListScene::OnRender( XUIMessageRender* pRenderData, BOOL& bHandled )
{
	// Check for LB button press to toggle favorite
	XINPUT_STATE state;
	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		bool lbPressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
		bool yPressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
		
		// Detect LB button press (wasn't pressed before, is pressed now)
		if (lbPressed && !m_bLBWasPressed)
		{
			// Get currently selected ROM
			int nIndex = m_RomList.GetCurSel();
			if (nIndex >= 0 && nIndex < (int)m_ListData.size())
			{
				string romName = m_ListData[nIndex];
				
				// Toggle favorite status
				if (romPaths.IsFavorite(romName))
				{
					romPaths.RemoveFavorite(romName);
					const WCHAR * button_text = L"OK";
					ShowMessageBoxEx(NULL, NULL, L"Favorites", L"Removed from favorites", 1, (LPCWSTR*)&button_text, NULL, XUI_MB_CENTER_ON_PARENT, NULL);
				}
				else
				{
					romPaths.AddFavorite(romName);
					const WCHAR * button_text = L"OK";
					ShowMessageBoxEx(NULL, NULL, L"Favorites", L"Added to favorites", 1, (LPCWSTR*)&button_text, NULL, XUI_MB_CENTER_ON_PARENT, NULL);
				}
			}
		}
		
		m_bLBWasPressed = lbPressed;
		m_bYWasPressed = yPressed;
	}
	
	bHandled = FALSE;  // Let other handlers process render too
	return S_OK;
}

VOID   CRomListScene::SetEffectValue( INT nValue )
{
        CEffectScene* pEffectScene;

        // Since we implement CEffectScene in this sample, we can just retrieve
        // a CEffectScene instance pointer from the handle contained in m_EffectScene
        XuiObjectFromHandle( m_EffectScene, ( VOID** )&pEffectScene );

        assert( pEffectScene != NULL );

        pEffectScene->SetDisplacementFactor( nValue / 50.0f );
}
    //----------------------------------------------------------------------------------
    // Performs initialization tasks - retreives controls.
    //----------------------------------------------------------------------------------
HRESULT CRomListScene::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
    {
        // Retrieve controls for later use.
        GetChildById( L"XuiAddToFavorite", &m_AddToFavorites );
        GetChildById( L"XuiPlay", &m_PlayRom );
        GetChildById( L"XuiMainMenu", &m_Back );		 
		GetChildById( L"XuiRomList", &m_RomList );
		GetChildById( L"XuiRomPreview", &m_PreviewImage );	 		 
	 
		GetChildById( L"XuiCurrentDeviceText", &m_DeviceText);
		GetChildById( L"XuiNextDeviceButton", &m_NextDevice);

		GetChildById( L"EffectScene", &m_EffectScene );
 
		phObj = this->m_hObj;
		hRomListScene = this->m_hObj;  // Store scene handle for OnRescanRoms callback
		g_RomListSceneInstance = this;  // Expose instance for per-frame updates

		m_RomList.DiscardResources(XUI_DISCARD_ALL);
		m_RomList.SetFocus();
		m_RomList.SetCurSel(0);
 
		// Update device text - show "Favorites" if in favorites mode, otherwise show current device
		if (g_bShowFavoritesOnly)
		{
			swprintf_s(DeviceText, L"Favorites");
		}
		else
		{
			swprintf_s(DeviceText, L"Current Device : %S", szRomPath);
		}
		m_DeviceText.SetText(DeviceText);
		
		// Initialize LB button state tracking
		m_bLBWasPressed = false;
		// Initialize Y button state tracking
		m_bYWasPressed = false;
		// Clear search filter on init
		m_SearchFilter.clear();
		m_OriginalListData.clear();
		// Initialize keyboard state
		m_searchLatch = false;
		m_keyboardPending = false;
		m_keyboardEvent = NULL;
		memset(&m_keyboardOverlapped, 0, sizeof(XOVERLAPPED));
		m_keyboardResult[0] = L'\0';

		bHandled = TRUE;

		SetEffectValue( 20 );
        return S_OK;
    }

CRomList::CRomList()
{
	romPath = romPaths.GetDeviceMapBegin();
 
	HANDLE hEventComplete = NULL;
	XOVERLAPPED xov = {0};

	hEventComplete = CreateEvent( NULL, FALSE, FALSE, NULL );

	xov.hEvent = hEventComplete;

	HANDLE stream = snesStoreage.OpenStreamForRead("Snes360Config.dat", L"Snes 360 Configuration", &xov, &hEventComplete);
		
	if (stream)
	{
		snesStoreage.ReadStream(stream, (char *)&Settings, sizeof(Settings));
		snesStoreage.CloseStream(stream, &xov);
		CloseHandle(hEventComplete);

		romPath = romPaths.FindDevice(std::string(Settings.romPath));
	 
	}

    strcpy((char *)szRomPath, romPath->first.c_str());
	swprintf_s(DeviceText, L"Current Device : %S", szRomPath);

}
 

HRESULT CRomList::OnNotify( XUINotify *hObj, BOOL& bHandled )
{
	wchar_t previewPath[MAX_PATH];
	 

	HXUIOBJ hPreviewImage;

	int nIndex = 0;
	switch(hObj->dwNotify)
	{
		case XN_SELCHANGED:
			 						 
			nIndex = XuiListGetCurSel( this->m_hObj, NULL );
			
			const char *fname = GetFilename((char *)romPaths.m_PreviewPath.c_str(), (char *)m_ListData[nIndex].c_str(), "png");
			XuiElementGetChildById( phObj, 
                L"XuiRomPreview", &hPreviewImage );

			string previewName(fname);

		  
			

			swprintf_s(previewPath,L"file://%S", ReplaceCharInString(romPaths.m_PreviewPath, '\\', "/").c_str());
			XuiElementDiscardResources(hPreviewImage, XUI_DISCARD_ALL);
			XuiElementSetBasePath(hPreviewImage, previewPath);

			previewName = ReplaceCharInString(previewName, '\\',"/");
			swprintf_s(previewPath,L"file://%S", previewName.c_str());
			XuiImageElementSetImagePath(hPreviewImage, previewPath);
						 
			break;

	}

	UpdatePresence(CONTEXT_GAMESTATE_BROWSING_ROMS);

	return S_OK;

}
 

HRESULT CRomList::OnRescanRoms( char *szPath,  BOOL& bHandled )
{ 
	
	DeleteItems(0, m_ListData.size());
	m_ListData.clear();

	if (g_bShowFavoritesOnly)
	{
		// Load favorites mode - get favorites from settings
		romPaths.LoadFavorites();
		set<string> favorites = romPaths.GetFavorites();
		
		if (!favorites.empty())
		{
			// Search through all device paths to find the ROM files
			for (map<string, string>::iterator pathIt = romPaths.GetDeviceMapBegin(); 
				 pathIt != romPaths.GetDeviceMapEnd(); 
				 ++pathIt)
			{
				string devicePath = pathIt->second;
				
				// First, get all files in this directory to match case-insensitively
				string searchPath = devicePath + "*.*";
				HANDLE hFind;
				WIN32_FIND_DATAA oFindData;
				
				hFind = FindFirstFile(searchPath.c_str(), &oFindData);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					do
					{
						// Skip directories
						if (!(oFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
						{
							// Get found filename in lowercase for comparison
							char foundLower[MAX_PATH];
							strcpy(foundLower, oFindData.cFileName);
							_strlwr(foundLower);
							string foundFileNameLower(foundLower);
							
							// Check if this file matches any favorite (case-insensitive)
							for (set<string>::iterator favIt = favorites.begin(); 
								 favIt != favorites.end(); 
								 ++favIt)
							{
								string favoriteName = *favIt;
								// Convert favorite name to lowercase for comparison
								char favLower[MAX_PATH];
								strcpy(favLower, favoriteName.c_str());
								_strlwr(favLower);
								string favLowerStr(favLower);
								
								// Match case-insensitively
								if (foundFileNameLower == favLowerStr)
								{
									// Check if we haven't already added this ROM
									bool alreadyAdded = false;
									for (size_t i = 0; i < m_ListData.size(); i++)
									{
										// Case-insensitive comparison
										char existingLower[MAX_PATH];
										strcpy(existingLower, m_ListData[i].c_str());
										_strlwr(existingLower);
										string existingLowerStr(existingLower);
										
										if (existingLowerStr == favLowerStr)
										{
											alreadyAdded = true;
											break;
										}
									}
									
									if (!alreadyAdded)
									{
										// Use the original favorite name (as stored)
										m_ListData.push_back(favoriteName);
									}
									break;  // Found match, move to next favorite
								}
							}
						}
					} while (FindNextFile(hFind, &oFindData));
					
					FindClose(hFind);
				}
			}
		}
		
		// Reset flag after loading
		g_bShowFavoritesOnly = false;
	}
	else
	{
		// Normal mode - load all ROMs from current path
		strcpy((char *)szRoms, romPath->second.c_str());
		strcat(szRoms, "*.*");
	 
		HANDLE hFind;	
		WIN32_FIND_DATAA oFindData;

		hFind = FindFirstFile(szRoms, &oFindData);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{		
				// Skip directories and special entries
				if (!(oFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					m_ListData.push_back(_strlwr(oFindData.cFileName));
				}

			} while (FindNextFile(hFind, &oFindData));

			FindClose(hFind);
		}
	}

	// Sort alphabetically (favorites will still show [Favorite] tag but won't be sorted first)
	std::sort(m_ListData.begin(), m_ListData.end());
	
	// If there's a scene instance, notify it that rescanning is complete
	// so it can store the original list and re-apply any search filter
	if (hRomListScene)
	{
		CRomListScene* pScene = NULL;
		if (SUCCEEDED(XuiObjectFromHandle(hRomListScene, (VOID**)&pScene)))
		{
			// Store the new list as original and re-apply filter if one exists
			pScene->OnRomsRescanned();
		}
	}
	
	InsertItems( 0, m_ListData.size() );

	bHandled = TRUE;	
    return( S_OK );
}

HRESULT CRomList::OnInit(XUIMessageInit *pInitData, BOOL& bHandled)
{
	// If in favorites mode, use empty path (we'll search all paths in OnRescanRoms)
	// Otherwise use the current device path and ensure flag is false
	char *pathToUse = (char *)romPath->second.c_str();
	if (g_bShowFavoritesOnly)
	{
		pathToUse = (char *)"";  // Empty path - OnRescanRoms will handle favorites mode
	}
	else
	{
		// Explicitly ensure flag is false for normal mode (in case scene is reused)
		g_bShowFavoritesOnly = false;
	}
 
	XUIMessage xuiMsg;
	InRescanRomsStruct msgData;
	InRescanRomsFirstFunc( &xuiMsg, &msgData, pathToUse );
	XuiSendMessage( m_hObj, &xuiMsg );

	bHandled = TRUE;
    return S_OK;
}

HRESULT CRomList::OnGetItemCountAll(
        XUIMessageGetItemCount *pGetItemCountData, 
        BOOL& bHandled)
    {
        pGetItemCountData->cItems = m_ListData.size();
        bHandled = TRUE;
        return S_OK;
    }


// Gets called every frame
HRESULT CRomList::OnGetSourceDataText(
    XUIMessageGetSourceText *pGetSourceTextData, 
    BOOL& bHandled)
{
    if( ( 0 == pGetSourceTextData->iData ) && ( ( pGetSourceTextData->bItemData ) ) ) {
		if (pGetSourceTextData->iItem >= 0 && pGetSourceTextData->iItem < (int)m_ListData.size())
		{
			string displayName = m_ListData[pGetSourceTextData->iItem];
			
			// Check if this ROM is a favorite and add prefix
			romPaths.LoadFavorites();  // Reload to ensure we have latest favorites
			if (romPaths.IsFavorite(displayName))
			{
				displayName = "[Favorite] " + displayName;
			}
			
			LPCWSTR lpszwBuffer = MultiCharToUniChar((char *)displayName.c_str());
            pGetSourceTextData->szText = lpszwBuffer;
            bHandled = TRUE;
		}
    }
    return S_OK;

}


std::string ReplaceCharInString(  
    const std::string & source, 
    char charToReplace, 
    const std::string replaceString 
    ) 
{ 
    std::string result; 
 
    // For each character in source string: 
    const char * pch = source.c_str(); 
    while ( *pch != '\0' ) 
    { 
        // Found character to be replaced? 
        if ( *pch == charToReplace ) 
        { 
            result += replaceString; 
        } 
        else 
        { 
            // Just copy original character 
            result += (*pch); 
        } 
 
        // Move to next character 
        ++pch; 
    } 
 
    return result; 
}

// Show Xbox 360 keyboard dialog for search (async, like NES emu)
HRESULT CRomListScene::ShowSearchKeyboard()
{
	// Prevent multiple keyboard opens
	if (m_searchLatch || m_keyboardPending) return S_OK;
	m_searchLatch = true;
	
	// Close any existing event handle
	if (m_keyboardEvent)
	{
		CloseHandle(m_keyboardEvent);
		m_keyboardEvent = NULL;
	}
	
	// Create event for async operation
	m_keyboardEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_keyboardEvent)
	{
		m_searchLatch = false;
		return E_FAIL;
	}
	
	// Initialize overlapped structure
	memset(&m_keyboardOverlapped, 0, sizeof(XOVERLAPPED));
	m_keyboardOverlapped.hEvent = m_keyboardEvent;
	
	// Initialize result buffer and pre-populate with current filter
	memset(m_keyboardResult, 0, sizeof(m_keyboardResult));
	// Copy current filter to result buffer (for default text)
	if (!m_SearchFilter.empty())
	{
		wcsncpy(m_keyboardResult, m_SearchFilter.c_str(), sizeof(m_keyboardResult) / sizeof(WCHAR) - 1);
		m_keyboardResult[(sizeof(m_keyboardResult) / sizeof(WCHAR)) - 1] = L'\0';  // Ensure null termination
	}
	
	DWORD dwResultLength = sizeof(m_keyboardResult) / sizeof(WCHAR);
	
	// Show the Xbox 360 on-screen keyboard (non-blocking async call)
	DWORD dwResult = XShowKeyboardUI(
		0,                              // dwUserIndex (controller 1 = 0)
		0,                              // dwFlags (0 = default keyboard)
		m_SearchFilter.c_str(),          // pwszDefaultText - pre-populate with current filter
		L"Search ROMs",                   // pwszTitleText
		L"Enter game name to search",     // pwszDescriptionText
		m_keyboardResult,                // pwszResultText - receives the entered text
		dwResultLength,                  // cchResultText - size of result buffer
		&m_keyboardOverlapped            // pOverlapped - for async operation
	);
	
	if (dwResult == ERROR_IO_PENDING)
	{
		// Keyboard is now pending - will check for completion in UpdatePerFrame (non-blocking)
		m_keyboardPending = true;
		m_searchLatch = false;  // Allow Y button to work again after keyboard closes
	}
	else if (dwResult == ERROR_SUCCESS)
	{
		// Keyboard returned immediately (unlikely but handle it)
		m_SearchFilter = m_keyboardResult;
		ApplySearchFilter(m_SearchFilter);
		CloseHandle(m_keyboardEvent);
		m_keyboardEvent = NULL;
		m_searchLatch = false;
	}
	else
	{
		// Error occurred
		CloseHandle(m_keyboardEvent);
		m_keyboardEvent = NULL;
		m_searchLatch = false;
	}
	
	return S_OK;
}

// UpdatePerFrame - called from main loop for per-frame updates (like NES emu)
VOID CRomListScene::UpdatePerFrame()
{
	// Only act while we're in the ROM list scene (not during emulation)
	if (IsCurrentlyInGame) return;
	
	// Check for keyboard completion if one is pending (non-blocking check)
	if (m_keyboardPending && m_keyboardEvent)
	{
		DWORD dwWaitResult = WaitForSingleObject(m_keyboardEvent, 0);  // Non-blocking check
		if (dwWaitResult == WAIT_OBJECT_0)
		{
			// Keyboard input completed - always process the result
			m_keyboardPending = false;
			
			// Get the result text (should already be populated in m_keyboardResult by XShowKeyboardUI)
			std::wstring newFilter = m_keyboardResult;
			
			// Apply the filter (even if empty - empty means show all ROMs)
			m_SearchFilter = newFilter;
			ApplySearchFilter(m_SearchFilter);
			
			CloseHandle(m_keyboardEvent);
			m_keyboardEvent = NULL;
		}
	}
	
	// Check for search trigger (Y button) - only in ROM browser (not during emulation)
	XINPUT_STATE state;
	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		bool yJustPressed = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
		static bool yWasPressed = false;
		
		if (yJustPressed && !yWasPressed && !m_keyboardPending)
		{
			// Y button just pressed - show keyboard
			ShowSearchKeyboard();
		}
		
		yWasPressed = yJustPressed;
	}
}

// Called when ROMs are rescanned - store original list and re-apply filter
VOID CRomListScene::OnRomsRescanned()
{
	// Store the current (unfiltered) list as original
	m_OriginalListData = m_ListData;
	
	// Re-apply search filter if one exists
	if (!m_SearchFilter.empty())
	{
		ApplySearchFilter(m_SearchFilter);
	}
}

// Apply search filter to ROM list
HRESULT CRomListScene::ApplySearchFilter(const std::wstring& filter)
{
	// If filter is empty, restore original list
	if (filter.empty())
	{
		if (!m_OriginalListData.empty())
		{
			m_ListData = m_OriginalListData;
		}
	}
	else
	{
		// Store original list if not already stored
		if (m_OriginalListData.empty() && !m_ListData.empty())
		{
			m_OriginalListData = m_ListData;
		}
		
		// Convert filter to lowercase for case-insensitive search
		std::wstring filterLower = filter;
		std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::towlower);
		
		// Convert to multibyte for comparison with ROM names
		char filterMB[256];
		WideCharToMultiByte(CP_ACP, 0, filterLower.c_str(), -1, filterMB, 256, NULL, NULL);
		
		// Filter the list
		std::vector<std::string> filteredList;
		for (size_t i = 0; i < m_OriginalListData.size(); i++)
		{
			// Convert ROM name to lowercase for comparison
			char romLower[MAX_PATH];
			strcpy(romLower, m_OriginalListData[i].c_str());
			_strlwr(romLower);
			
			// Check if ROM name contains the filter text
			if (strstr(romLower, filterMB) != NULL)
			{
				filteredList.push_back(m_OriginalListData[i]);
			}
		}
		
		m_ListData = filteredList;
	}
	
	// Update the list display
	m_RomList.DeleteItems(0, m_RomList.GetItemCount());
	m_RomList.InsertItems(0, m_ListData.size());
	m_RomList.SetCurSel(0);
	
	return S_OK;
} 
 