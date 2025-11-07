#include <xmp.h>
#include <xtl.h>
#include <set>
#include <map>
#include <algorithm>
#include "FavoritesList.h"
#include "Storage.h"
#include "Main.h"
#include "RomSettings.h"
#include "XboxContext.h"
#include "GeneralFunctions.h"

extern int RunEmulation(char *path, char *RomName);
extern BOOL IsCurrentlyInGame;
extern HXUIOBJ phObj;
extern HXUIOBJ hScene; 
extern HXUIOBJ hMainScene;
extern HXUIOBJ hRomListScene;
extern GameStorage snesStoreage;
extern CRomPathSettings romPaths;

std::string ReplaceCharInString(const std::string & source, char charToReplace, const std::string replaceString);

std::vector<std::string> m_FavoritesListData;
std::vector<std::string> m_FavoritesListPaths;  // Full paths to ROM files

extern void SaveConfig(void);

CFavoritesList::CFavoritesList()
{
	LoadFavorites();
}

void CFavoritesList::LoadFavorites()
{
	m_FavoritesListData.clear();
	m_FavoritesListPaths.clear();
	
	// Reload favorites from file
	romPaths.LoadFavorites();
	
	// Get favorites from settings
	set<string> favorites = romPaths.GetFavorites();
	
	if (favorites.empty())
		return;
	
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
							for (size_t i = 0; i < m_FavoritesListData.size(); i++)
							{
								// Case-insensitive comparison
								char existingLower[MAX_PATH];
								strcpy(existingLower, m_FavoritesListData[i].c_str());
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
								// Use the original favorite name (as stored) and store full path
								m_FavoritesListData.push_back(favoriteName);
								string fullPath = devicePath + string(oFindData.cFileName);  // Use actual filename from filesystem
								m_FavoritesListPaths.push_back(fullPath);
							}
							break;  // Found match, move to next favorite
						}
					}
				}
			} while (FindNextFile(hFind, &oFindData));
			
			FindClose(hFind);
		}
	}
	
	// Sort the list while keeping names and paths in sync
	// Create pairs of (name, path) for sorting
	std::vector<std::pair<std::string, std::string> > rows;
	rows.reserve(m_FavoritesListData.size());
	for (size_t i = 0; i < m_FavoritesListData.size(); ++i)
	{
		rows.push_back(std::make_pair(m_FavoritesListData[i], m_FavoritesListPaths[i]));
	}
	
	// Case-insensitive sort by display name using a function object (VS2008 doesn't support lambdas)
	struct ComparePair
	{
		bool operator()(const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) const
		{
			return _stricmp(a.first.c_str(), b.first.c_str()) < 0;
		}
	};
	
	std::sort(rows.begin(), rows.end(), ComparePair());
	
	// Rebuild the vectors in sorted order
	m_FavoritesListData.clear();
	m_FavoritesListPaths.clear();
	for (size_t i = 0; i < rows.size(); ++i)
	{
		m_FavoritesListData.push_back(rows[i].first);
		m_FavoritesListPaths.push_back(rows[i].second);
	}
}

HRESULT CFavoritesList::OnNotify( XUINotify *hObj, BOOL& bHandled )
{
	wchar_t previewPath[MAX_PATH];
	HXUIOBJ hPreviewImage;

	int nIndex = 0;
	switch(hObj->dwNotify)
	{
		case XN_SELCHANGED:
			nIndex = XuiListGetCurSel( this->m_hObj, NULL );
			
			// Bounds check before accessing array
			if (nIndex >= 0 && nIndex < (int)m_FavoritesListData.size() && phObj != NULL)
			{
				const char *fname = GetFilename((char *)romPaths.m_PreviewPath.c_str(), (char *)m_FavoritesListData[nIndex].c_str(), "png");
				if (fname != NULL)
				{
					HRESULT hr = XuiElementGetChildById( phObj, L"XuiRomPreview", &hPreviewImage );
					if (SUCCEEDED(hr) && hPreviewImage != NULL)
					{
						string previewName(fname);

						swprintf_s(previewPath, MAX_PATH, L"file://%S", ReplaceCharInString(romPaths.m_PreviewPath, '\\', "/").c_str());
						XuiElementDiscardResources(hPreviewImage, XUI_DISCARD_ALL);
						XuiElementSetBasePath(hPreviewImage, previewPath);

						previewName = ReplaceCharInString(previewName, '\\',"/");
						swprintf_s(previewPath, MAX_PATH, L"file://%S", previewName.c_str());
						XuiImageElementSetImagePath(hPreviewImage, previewPath);
					}
				}
			}
			break;
	}

	UpdatePresence(CONTEXT_GAMESTATE_BROWSING_ROMS);

	return S_OK;
}

HRESULT CFavoritesList::OnInit(XUIMessageInit *pInitData, BOOL& bHandled)
{
	// Load favorites first
	LoadFavorites();
	
	// Clear existing items safely
	int itemCount = GetItemCount();
	if (itemCount > 0)
	{
		DeleteItems(0, itemCount);
	}
	
	// Only insert items if we have favorites
	if (m_FavoritesListData.size() > 0)
	{
		InsertItems( 0, m_FavoritesListData.size() );
	}
	
	bHandled = TRUE;
    return S_OK;
}

HRESULT CFavoritesList::OnGetItemCountAll(
        XUIMessageGetItemCount *pGetItemCountData, 
        BOOL& bHandled)
{
    pGetItemCountData->cItems = m_FavoritesListData.size();
    bHandled = TRUE;
    return S_OK;
}

HRESULT CFavoritesList::OnGetSourceDataText(
    XUIMessageGetSourceText *pGetSourceTextData, 
    BOOL& bHandled)
{
    if( ( 0 == pGetSourceTextData->iData ) && ( ( pGetSourceTextData->bItemData ) ) ) {
		// Bounds check to prevent crash
		if (pGetSourceTextData->iItem >= 0 && pGetSourceTextData->iItem < (int)m_FavoritesListData.size())
		{
			LPCWSTR lpszwBuffer = MultiCharToUniChar((char *)m_FavoritesListData[pGetSourceTextData->iItem].c_str());
			pGetSourceTextData->szText = lpszwBuffer;
			bHandled = TRUE;
		}
		else
		{
			bHandled = FALSE;
		}
    }
    return S_OK;
}

// Handler for the XM_NOTIFY message
HRESULT CFavoritesListScene::OnNotifyPress( HXUIOBJ hObjPressed, 
       BOOL& bHandled )
{
	int nIndex;

    if ( hObjPressed == m_FavoritesList || hObjPressed == m_PlayRom )
    {
		XMPPause( NULL );
		nIndex = m_FavoritesList.GetCurSel();
		
		// Bounds check for both arrays
		if (nIndex >= 0 && nIndex < (int)m_FavoritesListData.size() && nIndex < (int)m_FavoritesListPaths.size())
		{
			// Split the full path into directory and filename
			// RunEmulation expects (directory, filename) and concatenates them internally
			std::string fullPath = m_FavoritesListPaths[nIndex];
			std::string dir, file;
			
			// Find the last path separator
			size_t lastSep = fullPath.find_last_of("\\/");
			if (lastSep == std::string::npos)
			{
				// No separator found, treat entire string as filename
				dir = "";
				file = fullPath;
			}
			else
			{
				// Split at the separator (include separator in directory)
				dir = fullPath.substr(0, lastSep + 1);
				file = fullPath.substr(lastSep + 1);
			}
			
			// Create the InGameOptions scene and navigate to it (required for RunEmulation to work)
			// The scene will be hidden until user presses both sticks in-game
			HRESULT hr = XuiSceneCreate( L"file://game:/media/Snes360.xzp#..\\Xbox\\Skin\\", L"InGameOptions.xur", NULL, &hScene );
			if (SUCCEEDED(hr))
			{
				// Navigate forward to the scene (needed for RunEmulation context)
				// The InGameOptions scene should be designed to not show UI until both sticks are pressed
				this->NavigateForward(hScene);
				
				// Pass directory and filename separately (like the ROM list does)
				RunEmulation((char *)dir.c_str(), (char *)file.c_str()); 	
				
				SaveConfig();
				
				// Destroy the scene after starting (it will be recreated when both sticks are pressed)
				XuiDestroyObject( hScene );
			}
			XMPContinue( NULL );
		}
		
		bHandled = TRUE;
		return S_OK;
    }

	if( XuiControlIsBackButton( hObjPressed ) )
	{
		this->NavigateBack();
	}
	 
    bHandled = TRUE;
    return S_OK;
}

HRESULT CFavoritesListScene::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
{
    // Retrieve controls for later use.
    GetChildById( L"XuiPlay", &m_PlayRom );
    GetChildById( L"XuiMainMenu", &m_Back );		 
	GetChildById( L"XuiRomList", &m_FavoritesList );
	GetChildById( L"XuiRomPreview", &m_PreviewImage );	 		 
	GetChildById( L"EffectScene", &m_EffectScene );

	phObj = this->m_hObj;

	// Only set focus and selection if list has items
	m_FavoritesList.DiscardResources(XUI_DISCARD_ALL);
	
	// Check if list has items before setting focus/selection
	if (m_FavoritesList.GetItemCount() > 0)
	{
		m_FavoritesList.SetFocus();
		m_FavoritesList.SetCurSel(0);
	}

	bHandled = TRUE;

	SetEffectValue( 20 );
    return S_OK;
}

VOID CFavoritesListScene::SetEffectValue( INT nValue )
{
    CEffectScene* pEffectScene;

    XuiObjectFromHandle( m_EffectScene, ( VOID** )&pEffectScene );

    assert( pEffectScene != NULL );

    pEffectScene->SetDisplacementFactor( nValue / 50.0f );
}

