
#include "Snes9x.h"
#include "wsnes9x.h"
#include "main.h"
#include "InGameOptions.h"
#include "Storage.h"
#include "screenshot.h"


extern BOOL IsCurrentlyInGame;
extern struct SSettings Settings;
extern struct sGUI GUI;
extern IDirect3DDevice9 *pDevice;
extern void FreezeUnfreeze (int slot, bool8 freeze);
extern BOOL run_loop;
extern bool exitFromXUI;
extern HXUIOBJ hRomListScene;
extern HXUIOBJ phObj;
extern bool8 S9xSetSoundMute (bool8 mute);
extern GameStorage snesStoreage;
extern int bAdjustScreen;
extern void S9xAutoSaveSRAM (void);

// Handler for the XM_NOTIFY message
HRESULT CInGameOptions::OnNotifyPress( HXUIOBJ hObjPressed, 
       BOOL& bHandled )
    {
		
	 
		if ( hObjPressed == m_SaveState)
        {
			const WCHAR * button_text = L"OK";
			 
			FreezeUnfreeze(0,true);
			ShowMessageBoxEx(NULL,NULL,L"Snes360 - Save State", L"Saved state successfully", 1, (LPCWSTR*)&button_text,NULL,  XUI_MB_CENTER_ON_PARENT, NULL); 
			 
			bHandled = TRUE;
			return S_OK;
			
        }
		else if (hObjPressed == m_LoadState)
		{
			const WCHAR * button_text = L"OK";
			FreezeUnfreeze(0,false);
			ShowMessageBoxEx(NULL,NULL,L"Snes360 - Load State", L"Loaded state successfully", 1, (LPCWSTR*)&button_text,NULL,  XUI_MB_CENTER_ON_PARENT, NULL); 			 
			//DoAchievo(ACHIEVEMENT_CHEAT);  // Achievements disabled
			bHandled = TRUE;
			return S_OK;			


		}
 
		else if ( hObjPressed == m_TVMode )
		{

			GUI.Scale =	FILTER_TVMODE;
			GUI.NextScale =	FILTER_TVMODE;
			GUI.ScaleHiRes = FILTER_TVMODE;
			GUI.NextScaleHiRes = FILTER_TVMODE;

			return S_OK;

		}
		else if (hObjPressed == m_BackToGame)
		{
			Settings.Paused = false;	
			Settings.ForcedPause = false;	
			//ReInitSound((int)(!Settings.Mute));
			S9xSetSoundMute(FALSE);
			bHandled = TRUE;
			return S_OK;

		}
		else if (hObjPressed == m_AdjustScreen)
		{
			Settings.Paused = false;	
			Settings.ForcedPause = false;	
			//ReInitSound((int)(!Settings.Mute));
			//S9xSetSoundMute(FALSE);
			bHandled = TRUE;
			bAdjustScreen = 1;
			return S_OK;
		}
		else if (hObjPressed == m_ExitGame)
		{

			if(CPU.SRAMModified) 
			{
				S9xAutoSaveSRAM();
				CPU.SRAMModified=FALSE;
			}

			Settings.Paused = false;
			Settings.ForcedPause = false;	
			exitFromXUI = true;
			bHandled = TRUE;			 			     
			this->NavigateBack();
 			return S_OK;
		}
		else if (hObjPressed == m_AspectRatio)
		{

			if (m_AspectRatio.IsChecked())
			{
				GUI.AspectRatio = true;	
				Settings.Aspect = 1;
			}
			else
			{
				GUI.AspectRatio = false;
				Settings.Aspect = 0;
			}

			bHandled = TRUE;
			return S_OK;

		}
		else if (hObjPressed == m_PointFiltering)
		{

			if (m_PointFiltering.IsChecked())
			{
				GUI.VideoMemory = false;	
			}
			else
			{
				GUI.VideoMemory = true;
			}

			bHandled = TRUE;
			return S_OK;

		}
		else if (hObjPressed == m_MuteAudio)
		{

			if (m_MuteAudio.IsChecked())
			{
				Settings.Mute = true;	
			}
			else
			{
				Settings.Mute = false;
			}

			bHandled = TRUE;
			return S_OK;

		}
		else if (hObjPressed == m_TakePreview)
		{
			
			if (S9xDoScreenshot(256, 224))
			{

				const WCHAR * button_text = L"OK";				
				ShowMessageBoxEx(NULL,NULL,L"Snes360 - Take Preview", L"Preview Saved.", 1, (LPCWSTR*)&button_text,NULL,  XUI_MB_CENTER_ON_PARENT, NULL);
 
				bHandled = TRUE;
				return S_OK;
			}

		}


		switch(m_FilterGroup.GetCurSel())
		{
		case 0:			
			GUI.Scale =	FILTER_SIMPLE2X;
			GUI.NextScale =	FILTER_SIMPLE2X;
			GUI.ScaleHiRes = FILTER_SIMPLE2X;
			GUI.NextScaleHiRes = FILTER_SIMPLE2X;
			break;
		case 1:
			GUI.Scale =	FILTER_SCANLINES;
			GUI.NextScale =	FILTER_SCANLINES;
			GUI.ScaleHiRes = FILTER_SCANLINES;
			GUI.NextScaleHiRes = FILTER_SCANLINES;
			break;
		case 2:
			GUI.Scale =	FILTER_TVMODE;
			GUI.NextScale =	FILTER_TVMODE;
			GUI.ScaleHiRes = FILTER_TVMODE;
			GUI.NextScaleHiRes = FILTER_TVMODE;
			break;
		case 3:
			GUI.Scale =	FILTER_SUPEREAGLE;
			GUI.NextScale =	FILTER_SUPEREAGLE;
			GUI.ScaleHiRes = FILTER_SUPEREAGLE;
			GUI.NextScaleHiRes = FILTER_SUPEREAGLE;
			break;
		case 4:
			GUI.Scale =	FILTER_SUPER2XSAI;
			GUI.NextScale =	FILTER_SUPER2XSAI;
			GUI.ScaleHiRes = FILTER_SUPER2XSAI;
			GUI.NextScaleHiRes = FILTER_SUPER2XSAI;
			break;
		case 5:
			GUI.Scale =	FILTER_EPXA;
			GUI.NextScale =	FILTER_EPXA;
			GUI.ScaleHiRes = FILTER_EPXA;
			GUI.NextScaleHiRes = FILTER_EPXA;
			break;
		case 6:
			GUI.Scale =	FILTER_HQ2X;
			GUI.NextScale =	FILTER_HQ2X;
			GUI.ScaleHiRes = FILTER_HQ2X;
			GUI.NextScaleHiRes = FILTER_HQ2X;
			break;
		}

		Settings.Filter = m_FilterGroup.GetCurSel();
 
        bHandled = TRUE;
        return S_OK;
    }


    //----------------------------------------------------------------------------------
    // Performs initialization tasks - retreives controls.
    //----------------------------------------------------------------------------------
HRESULT CInGameOptions::OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
    {
        // Retrieve controls for later use.
        GetChildById( L"XuiSaveStateButton", &m_SaveState );
        GetChildById( L"XuiLoadStateButton", &m_LoadState );
        GetChildById( L"XuiFilterSimple2x", &m_Simple2x );		 
		GetChildById( L"XuiFilterScanlines", &m_Scanlines );
		GetChildById( L"XuiFilterTVMode", &m_TVMode );	 		 
		GetChildById( L"XuiFilterSuperEagle", &m_SuperEagle );
		GetChildById( L"XuiFilter2xSAI", &m_Super2xSAI );
		GetChildById( L"XuiFilterHQ2x", &m_HQ2x );
		GetChildById( L"XuiPreviewImage", &m_PreviewImage );
		GetChildById( L"XuiPreviewSmall", &m_PreviewSmallImage );
		GetChildById( L"XuiButtonBackToGame", &m_BackToGame);
		GetChildById( L"XuiButtonExitGame", &m_ExitGame);
		GetChildById( L"XuiAspectRatio", &m_AspectRatio);
		GetChildById( L"XuiPointFiltering", &m_PointFiltering);
		GetChildById( L"XuiMuteAudio", &m_MuteAudio);
		GetChildById( L"XuiButtonTakePreview", &m_TakePreview);
		GetChildById( L"XuiRadioGroup1", &m_FilterGroup);
		GetChildById( L"XuiFilterEPX", &m_EPX);
		GetChildById( L"XuiAdjustScreen", &m_AdjustScreen);

		GetChildById( L"EffectScene", &m_EffectScene );
	

		if (Settings.Aspect == 1)
		{
			GUI.AspectRatio = true;
			m_AspectRatio.SetCheck(true);
		}
		else
		{
			GUI.AspectRatio = false;
			m_AspectRatio.SetCheck(false);

		}

		if (!GUI.VideoMemory)
		{
			m_PointFiltering.SetCheck(true);
		}

		if (Settings.Mute)
		{
			m_MuteAudio.SetCheck(true);
		}

		switch(Settings.Filter)
		{
		case 0:			
			GUI.Scale =	FILTER_SIMPLE2X;
			GUI.NextScale =	FILTER_SIMPLE2X;
			GUI.ScaleHiRes = FILTER_SIMPLE2X;
			GUI.NextScaleHiRes = FILTER_SIMPLE2X;
			break;
		case 1:
			GUI.Scale =	FILTER_SCANLINES;
			GUI.NextScale =	FILTER_SCANLINES;
			GUI.ScaleHiRes = FILTER_SCANLINES;
			GUI.NextScaleHiRes = FILTER_SCANLINES;
			break;
		case 2:
			GUI.Scale =	FILTER_TVMODE;
			GUI.NextScale =	FILTER_TVMODE;
			GUI.ScaleHiRes = FILTER_TVMODE;
			GUI.NextScaleHiRes = FILTER_TVMODE;
			break;
		case 3:
			GUI.Scale =	FILTER_SUPEREAGLE;
			GUI.NextScale =	FILTER_SUPEREAGLE;
			GUI.ScaleHiRes = FILTER_SUPEREAGLE;
			GUI.NextScaleHiRes = FILTER_SUPEREAGLE;
			break;
		case 4:
			GUI.Scale =	FILTER_SUPER2XSAI;
			GUI.NextScale =	FILTER_SUPER2XSAI;
			GUI.ScaleHiRes = FILTER_SUPER2XSAI;
			GUI.NextScaleHiRes = FILTER_SUPER2XSAI;
			break;
		case 5:
			GUI.Scale =	FILTER_EPXA;
			GUI.NextScale =	FILTER_EPXA;
			GUI.ScaleHiRes = FILTER_EPXA;
			GUI.NextScaleHiRes = FILTER_EPXA;
			break;
		case 6:
			GUI.Scale =	FILTER_HQ2X;
			GUI.NextScale =	FILTER_HQ2X;
			GUI.ScaleHiRes = FILTER_HQ2X;
			GUI.NextScaleHiRes = FILTER_HQ2X;
			break;
		}


		m_FilterGroup.SetCurSel(Settings.Filter);

		SetEffectValue( 20 );

        return S_OK;
    }

VOID   CInGameOptions::SetEffectValue( INT nValue )
{
        CEffectScene* pEffectScene;

        // Since we implement CEffectScene in this sample, we can just retrieve
        // a CEffectScene instance pointer from the handle contained in m_EffectScene
        XuiObjectFromHandle( m_EffectScene, ( VOID** )&pEffectScene );

        
        pEffectScene->SetDisplacementFactor( nValue / 50.0f );
}

HRESULT CInGameOptions::OnInGameMenu( int iVal1,  BOOL& bHandled )
{ 
	DeleteFile("cache:\\preview.png");

	if (S9xDoScreenshot(256, 224, "cache:\\preview.png"))
	{
		HRESULT hr;
 
		m_PreviewImage.DiscardResources(XUI_DISCARD_ALL);
		m_PreviewSmallImage.DiscardResources(XUI_DISCARD_ALL);

		hr = m_PreviewImage.SetBasePath(L"file://cache:/");
		hr = m_PreviewImage.SetImagePath(L"file://cache:/preview.png");
		hr = m_PreviewSmallImage.SetBasePath(L"file://cache:/");
		hr = m_PreviewSmallImage.SetImagePath(L"file://cache:/preview.png");
 
		 

		bHandled = TRUE;
	}
	
    return( S_OK );
}

void InGameMenuFirstFunc(XUIMessage *pMsg, InGameMenuStruct* pData, int iVal1)
{
    XuiMessage(pMsg,XM_MESSAGE_ON_INGAME_MENU);
    _XuiMessageExtra(pMsg,(XUIMessageData*) pData, sizeof(*pData));

	
}

 
 