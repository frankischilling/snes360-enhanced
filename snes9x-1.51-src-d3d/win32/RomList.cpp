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
		
		// Detect button press (wasn't pressed before, is pressed now)
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
 