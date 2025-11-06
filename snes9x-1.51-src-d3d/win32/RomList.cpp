#include <xmp.h>
#include "RomList.h"
#include "Storage.h"
#include "Main.h"
#include "RomSettings.h"
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

		if( XuiControlIsBackButton( hObjPressed ) )
		{
			this->NavigateBack();

		}
		
 
		 
        bHandled = TRUE;
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
 
		swprintf_s(DeviceText, L"Current Device : %S", szRomPath);
		m_DeviceText.SetText(DeviceText);

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
	strcpy((char *)szRoms, romPath->second.c_str());
	strcat(szRoms, "*.*");
 
	m_ListData.clear();

	HANDLE hFind;	
	WIN32_FIND_DATAA oFindData;

	hFind = FindFirstFile(szRoms, &oFindData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{		
			
			m_ListData.push_back(_strlwr(oFindData.cFileName));
			 

		} while (FindNextFile(hFind, &oFindData));

	
	}

	std::sort(m_ListData.begin(), m_ListData.end());
	 
	InsertItems( 0, m_ListData.size() );

	bHandled = TRUE;	
    return( S_OK );
}

HRESULT CRomList::OnInit(XUIMessageInit *pInitData, BOOL& bHandled)
{
 
	XUIMessage xuiMsg;
	InRescanRomsStruct msgData;
	InRescanRomsFirstFunc( &xuiMsg, &msgData, (char *)romPath->second.c_str() );
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

			LPCWSTR lpszwBuffer = MultiCharToUniChar((char *)m_ListData[pGetSourceTextData->iItem].c_str());

            pGetSourceTextData->szText = lpszwBuffer;

            bHandled = TRUE;
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
 