//--------------------------------------------------------------------------------------
// XuiTutorial.cpp
//
// Shows how to display and use a simple XUI scene.
//
// Xbox Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <xtl.h>
#include <xui.h>
#include <xuiapp.h>
#include <xfilecache.h>
#include <xmp.h>
#include "Main.h"
#include "RomList.h"
#include "FavoritesList.h"
#include "InGameOptions.h"
#include "Storage.h"
#include "RomSettings.h"

//#include "achievements.spa.h"  // Achievements disabled
#include "XboxContext.h"  // Minimal context/property definitions (achievements excluded)

//static const DWORD  ACHIEVEMENT_COUNT = 5;

extern void XboxSetDefaultValues();


//struct AchievementPicture  // Achievements disabled
//{
//    IDirect3DTexture9* Texture;        // Texture of the picture
//    DWORD dwImageId;				   // Id of the picture
//};


// XMP Stuff
XMP_HANDLE hXMPPlaylist;           // The music player playlist
XMP_SONGINFO songInfo;             // Info about the current song.
XMP_STATE XMPState;                // The current status of the music player
HANDLE hNotificationListener;

XMP_SONGDESCRIPTOR g_SongDescriptors[] =
{
    {
        L"GAME:\\media\\sounds\\background.mp3",    // File path.
        L"Snes360 Music",                           // Song title.
        L"",                                        // Song artist.
        L"",                                        // Song album.
        L"",                                        // Song album artist.
        L"",                                        // Genre.
        0,                                          // Track number in the album.
        69000,                                      // Duration in milliseconds.
        XMP_SONGFORMAT_MP3,                         // Format of the song.
    }
};

//AchievementPicture gAchievement;  // Achievements disabled

GameStorage snesStoreage;
char    GamerName[ 256 ];

//BYTE* m_Achievements;  // Achievements disabled
//DWORD dwAchievementCount = 0;  // Achievements disabled

//void DoAchievo(DWORD AcheivoID);  // Achievements disabled
//VOID EnumerateAchievements();  // Achievements disabled



CEffectScene::CEffectScene()
{

	m_dwTexWidth = 900;
	m_dwTexHeight = 540;
	m_pTexTarget = NULL;
	m_pTexDisplacement = NULL;
	m_pRenderTarget = NULL;
	m_pVertexShader = NULL;
	m_pPixelShader = NULL;
	m_pVertexDecl = NULL;
	m_fDisplacementFactor = 0.0f;
	m_fDisplacement = 0.0f;
}

CEffectScene::~CEffectScene()
{
    if( m_pTexTarget )
        m_pTexTarget->Release();
    if( m_pTexDisplacement )
        m_pTexDisplacement->Release();
    if( m_pRenderTarget )
        m_pRenderTarget->Release();
    if( m_pVertexShader )
        m_pVertexShader->Release();
    if( m_pPixelShader )
        m_pPixelShader->Release();
    if( m_pVertexDecl )
        m_pVertexDecl->Release();
}
 
    VOID    CEffectScene::SetDisplacementFactor( FLOAT fFactor )
    {
        m_fDisplacementFactor = fFactor;
    }

	HRESULT CEffectScene::InitSurfacesAndShaders( IDirect3DDevice9* pDevice )
	{
		HRESULT hr;

		DWORD pdwFileSize = 0;
		DWORD pdwFileSizeShader = 0;

		// create our render target and texture for rendering our children.
		hr = pDevice->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexTarget,
									 NULL );
		if( FAILED( hr ) )
			return S_FALSE;

		hr = pDevice->CreateRenderTarget( m_dwTexWidth, m_dwTexHeight, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0,
										  0, &m_pRenderTarget, NULL );
		if( FAILED( hr ) )
			return S_FALSE;

		// load our displacement texture
		hr = D3DXCreateTextureFromFile( pDevice, "game:\\media\\XuiEffect_Texture.png", &m_pTexDisplacement );
		if( FAILED( hr ) )
			return S_FALSE;

		// load our vertex and pixel shaders
		VOID* pCode = NULL;
		hr = LoadFile( "game:\\Media\\Shaders\\ShadeXuiEffectVertex.xvu", &pCode, &pdwFileSize);
		if( FAILED( hr ) )
			return S_FALSE;

		hr = pDevice->CreateVertexShader( ( DWORD* )pCode, &m_pVertexShader );
		if( FAILED( hr ) )
			return S_FALSE;

		UnloadFile( pCode );

		 

		hr = LoadFile( "game:\\Media\\Shaders\\ShadeXuiEffectPixel.xpu", &pCode, &pdwFileSizeShader );
		if( FAILED( hr ) )
			return S_FALSE;

		hr = pDevice->CreatePixelShader( ( DWORD* )pCode, &m_pPixelShader );
		if( FAILED( hr ) )
			return S_FALSE;

		UnloadFile( pCode );

		// create our vertex declaration.  This matches the CEffectScene::VertexType structure
		static const D3DVERTEXELEMENT9 vertexDecl [] =
		{
			{ 0,  0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
			{ 0,  8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,   D3DDECLUSAGE_TEXCOORD, 0},
			D3DDECL_END()
		};

		hr = pDevice->CreateVertexDeclaration( vertexDecl, &m_pVertexDecl );
		if( FAILED( hr ) )
			return hr;

		return S_OK;
	}



	VOID CEffectScene::RenderChildrenToTexture( XUIMessageRender* pData )
	{
		// save the color factor and blend mode. We always want to render the children
		// without alpha and using normal blending
		DWORD dwOrigFactor = pData->dwColorFactor;
		XUI_BLEND_MODE nOrigBlendMode = pData->nBlendMode;

		// set the new factor and blending mode
		pData->dwColorFactor = D3DCOLOR_ARGB( 255, 255, 255, 255 );

		pData->nBlendMode = XUI_BLEND_NORMAL;

		// save the view matrix so we can restore it after rendering to our texture
		D3DXMATRIX matOrigView;
		XuiRenderGetViewTransform( pData->hDC, &matOrigView );


		// compute the inverse of our transformation matrix so we can render the
		// children to the top-left of our render target
		D3DXMATRIX matView;
		XuiElementGetFullXForm( m_hObj, &matView );

		D3DXMatrixInverse( &matView, NULL, &matView );

		// set the view transform to the inverse of our full-screen transform matrix
		//XuiRenderSetViewTransform( pData->hDC, &matView );

		// now let the base class implementation actually render the children
		CXuiElement::RenderChildren( pData );

		// restore the view matrix
		//XuiRenderSetViewTransform( pData->hDC, &matOrigView );

		// restore the color factor and blend mode
		pData->dwColorFactor = dwOrigFactor;
		pData->nBlendMode = nOrigBlendMode;
		XuiSetBlendMode( pData->hDC, nOrigBlendMode );
	}


	VOID CEffectScene::RenderObjectTexture( IDirect3DDevice9* pDevice )
	{
		// disable the viewport transformation.  Our vertex shader assumes the output
		// is in screen space
		pDevice->SetRenderState( D3DRS_VIEWPORTENABLE, FALSE );

		// select our vertex shader into the device and set the appropriate shader constants
		pDevice->SetVertexShader( m_pVertexShader );

		D3DXMATRIX matWorld;
		XuiElementGetFullXForm( m_hObj, &matWorld );

		D3DXMatrixTranspose( &matWorld, &matWorld );
		pDevice->SetVertexShaderConstantF( 0, ( FLOAT* )&matWorld, 4 );


		// select our pixel shader into the device and set the appropriate shader constants
		pDevice->SetPixelShader( m_pPixelShader );

		D3DXVECTOR4 vColorFactor( 1, 1, 1, 1 );
		pDevice->SetPixelShaderConstantF( 0, ( FLOAT* )&vColorFactor, 1 );

		D3DXVECTOR4 vDisplacementFactor( m_fDisplacementFactor, m_fDisplacementFactor, 0, 0 );
		pDevice->SetPixelShaderConstantF( 1, ( FLOAT* )&vDisplacementFactor, 1 );

		D3DXVECTOR4 vDisplacement( m_fDisplacement, m_fDisplacement, 0, 0 );
		pDevice->SetPixelShaderConstantF( 2, ( FLOAT* )&vDisplacement, 1 );

		// update our displacement amount and wrap around on the 1.0f boundary
		m_fDisplacement += .001f;
		if( m_fDisplacement > 1 )
			m_fDisplacement = 1 - m_fDisplacement;

		// setup the sampler states and select our textures into the device
		pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

		pDevice->SetTexture( 0, m_pTexTarget );
		pDevice->SetTexture( 1, m_pTexDisplacement );

		// get the dimensions of our scene.  We use this as the size of the quad to
		// render
		FLOAT fWidth, fHeight;
		XuiElementGetBounds( m_hObj, &fWidth, &fHeight );

		VertexType points[] =
		{
			{ 0, 0, 0, 0 },
			{ fWidth, 0, 1, 0 },
			{ fWidth, fHeight, 1, 1 },
			{ 0, fHeight, 0, 1 },
		};

		// render a quad with the dimensions of our scene
		pDevice->SetVertexDeclaration( m_pVertexDecl );
		pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, points, sizeof( points[0] ) );
	}

	HRESULT CEffectScene::OnRender( XUIMessageRender* pRenderData, BOOL& bHandled )
	{
		// mark the message as handled since we fully render this object and its children
		bHandled = TRUE;

		// retrieve the D3D device.  For the sample we just retrieve this from 
		// pRenderData->hDC.  In a real title, this will be the same as the title owned D3D
		// device.
		IDirect3DDevice9* pDevice;
		XuiRenderGetDevice( pRenderData->hDC, &pDevice );
		if( !pDevice )
			return S_OK;

		// if we haven't initialized our render target, etc do so now
		if( !m_pTexTarget )
		{
			HRESULT hr = InitSurfacesAndShaders( pDevice );
			if( FAILED( hr ) )
			{
				pDevice->Release();
				return hr;
			}
		}

		assert( m_pTexTarget != NULL && m_pRenderTarget != NULL );

		// begin the XUI rendering process for this scene.  This sets up the internal XUI
		// rendering state
		XUIRenderStruct rs;
		BeginRender( pRenderData, &rs );

		// switch to our render target before rendering the children
		IDirect3DSurface9* pOrigRenderTarget = NULL;
		pDevice->GetRenderTarget( 0, &pOrigRenderTarget );
		pDevice->SetRenderTarget( 0, m_pRenderTarget );

		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB( 0, 255, 255, 255 ), 1, 0 );

		D3DXMATRIX mat;
		D3DXMatrixIdentity( &mat );

		// The BeginRender call causes the current transform to be cached internal to
		// XUI.  We call XuiRenderRestoreState to ensure that the new identity world
		// transform is applied.
		XuiRenderSetTransform( pRenderData->hDC, &mat );
		XuiRenderRestoreState( pRenderData->hDC );

		// now actually render the children to our render target
		RenderChildrenToTexture( pRenderData );

		// resolve the contents of our render target to our texture and restore the
		// render target on the device to the original state
		pDevice->Resolve( 0, NULL, m_pTexTarget, NULL, 0, 0, NULL, 0, 0, NULL );
		pDevice->SetRenderTarget( 0, pOrigRenderTarget );
		pOrigRenderTarget->Release();

		// finally render our texture to the device
		RenderObjectTexture( pDevice );

		// complete the rendering process and call XuiRenderRestoreState to notify XUI
		// that we have changed the state of the device render states etc during the 
		// rendering process
		EndRender( pRenderData, &rs );
		XuiRenderRestoreState( pRenderData->hDC );

		pDevice->Release();
		return S_OK;
	}

//--------------------------------------------------------------------------------------
// Scene implementation class.
//--------------------------------------------------------------------------------------
class CSnes360Menu : public CXuiSceneImpl
{
 

public:
	CXuiControl m_SignInLabel;
	CXuiTextElement m_VersionText;
protected:
    CXuiControl m_RomList;
	CXuiNavButton m_button2;  // Favorites button
	CXuiControl m_button3;
	CXuiControl m_button4;
	CXuiControl m_button5;
	CXuiControl m_Storage;
	CXuiScene m_EffectScene;      // the effect scene we're controlling

	CXuiControl m_MusicToggle;
 
	//CXuiImage m_Logo;
   
 
    // Message map.
    XUI_BEGIN_MSG_MAP()
        XUI_ON_XM_INIT( OnInit )
        XUI_ON_XM_NOTIFY_PRESS( OnNotifyPress )
		XUI_ON_XM_RENDER( OnRender )
    XUI_END_MSG_MAP()

 
 
public:
	void SetSignedInName()
	{
		wchar_t w_gamertag[XUSER_NAME_SIZE];
		wchar_t w_gamertagText[255];
		MultiByteToWideChar(CP_ACP, 0, GamerName, -1, w_gamertag, XUSER_NAME_SIZE);

		if (strlen(GamerName) == 0)
		{
			swprintf_s(w_gamertagText, L"Not Signed in");
		}
		else
		{
			swprintf_s(w_gamertagText,L"Signed in as : %S",GamerName);
		}

		m_SignInLabel.SetText(w_gamertagText);		  
	}

	// Handler for the XM_NOTIFY message
    HRESULT OnNotifyPress( HXUIOBJ hObjPressed, 
       BOOL& bHandled )
    {
        // Check if favorites button is being pressed
        if (hObjPressed == m_button2.m_hObj || hObjPressed == m_button2)
        {
			OutputDebugStringW(L"Favorites button pressed - loading scene\n");
			// Favorites button - load FavoritesListScene.xur
			HXUIOBJ hFavoritesScene;
			HRESULT hr = XuiSceneCreate( L"file://game:/media/Snes360.xzp#..\\Xbox\\Skin\\", L"FavoritesListScene.xur", NULL, &hFavoritesScene );
			if (SUCCEEDED(hr))
			{
				OutputDebugStringW(L"FavoritesListScene.xur loaded successfully, navigating...\n");
				this->NavigateForward(hFavoritesScene);
			}
			else
			{
				// Debug: Show error if scene creation fails
				WCHAR errorMsg[256];
				swprintf_s(errorMsg, L"Failed to load FavoritesListScene.xur: 0x%08X\n", hr);
				OutputDebugStringW(errorMsg);
			}
			bHandled = TRUE;
			return S_OK;
		}
        
		if( hObjPressed == m_button4 )
		{
			const WCHAR * button_text = L"OK";
			ShowMessageBoxEx(NULL,NULL,L"About", L"Snes360 V0.33 Beta\n\nOriginal by Anonymous\n07-16-2010 11:59pm\n\nEnhanced by frankischilling\n2025\n\nGreets to r0wdy, Arak0n, kl0wn, idc, direw0lf, PeteNub, MomDad, Odb718, Angerwound, Redline99, TJ_CRS, Xenon7, Xantium, _skitzo_\n\nDonations not required!\n\nFUCK Hawk, Jester\n\n", 1, (LPCWSTR*)&button_text,NULL,  XUI_MB_CENTER_ON_PARENT, NULL);
			//DoAchievo(ACHIEVEMENT_FREEXEX_DISS);  // Achievements disabled
			

		}
		else if (hObjPressed == m_MusicToggle)
		{
			XMP_STATE peState;
			XMPGetStatus(&peState);

			if (peState == XMP_STATE_PLAYING)
			{
				XMPPause(NULL);
			}
			else
			{

				XMPContinue(NULL);
			}
	
		}
		else if (hObjPressed == m_button5 )
		{
			// Quit to Dash
			XLaunchNewImage( "", 0 );

		}
		else if (hObjPressed == m_Storage)
		{
		 	snesStoreage.ShowDeviceUI();
			return S_OK;
		}
		else if (hObjPressed == m_button3)
		{
			//XShowAchievementsUI( snesStoreage.GetSignedInUser() );  // Achievements disabled
		}
		else
		{
			return S_OK;
		}
            
        bHandled = TRUE;
        return S_OK;
    }

	HRESULT OnRender(XUIMessageRender *pRenderData, BOOL &bHandled)
	{
		XUserGetName(snesStoreage.GetSignedInUser(), GamerName, sizeof(GamerName));
		SetSignedInName();

	
		return S_OK;
 
	 
	}

    VOID   SetEffectValue( INT nValue )
    {
        CEffectScene* pEffectScene;

        // Since we implement CEffectScene in this sample, we can just retrieve
        // a CEffectScene instance pointer from the handle contained in m_EffectScene
        XuiObjectFromHandle( m_EffectScene, ( VOID** )&pEffectScene );

        assert( pEffectScene != NULL );

        pEffectScene->SetDisplacementFactor( nValue / 50.0f );
    }


	HRESULT LoadFile( const CHAR* strFileName, VOID** ppFileData, DWORD* pdwFileSize )
	{
		assert( ppFileData );
		if( pdwFileSize )
			*pdwFileSize = 0L;

		// Open the file for reading
		HANDLE hFile = CreateFile( strFileName, GENERIC_READ, 0, NULL,
								   OPEN_EXISTING, 0, NULL );

		if( INVALID_HANDLE_VALUE == hFile )
			return E_HANDLE;

		DWORD dwFileSize = GetFileSize( hFile, NULL );
		VOID* pFileData = malloc( dwFileSize );

		if( NULL == pFileData )
		{
			CloseHandle( hFile );
			return E_OUTOFMEMORY;
		}

		DWORD dwBytesRead;
		if( !ReadFile( hFile, pFileData, dwFileSize, &dwBytesRead, NULL ) )
		{
			CloseHandle( hFile );
			free( pFileData );
			return E_FAIL;
		}

		// Finished reading file
		CloseHandle( hFile );

		if( dwBytesRead != dwFileSize )
		{
			free( pFileData );
			return E_FAIL;
		}

		if( pdwFileSize )
			*pdwFileSize = dwFileSize;
		*ppFileData = pFileData;

		return S_OK;
	}


	VOID UnloadFile( VOID* pFileData )
	{
		assert( pFileData != NULL );
		free( pFileData );
	}


    //----------------------------------------------------------------------------------
    // Performs initialization tasks - retreives controls.
    //----------------------------------------------------------------------------------
    HRESULT OnInit( XUIMessageInit* pInitData, BOOL& bHandled )
    {
		 
        // Retrieve controls for later use.
        GetChildById( L"XuiRoms", &m_RomList );
        
        // Enable and show the favorites button
        if (SUCCEEDED(GetChildById( L"XuiFavorites", &m_button2 )))
        {
            m_button2.SetShow(true);  // Explicitly show the favorites button
            m_button2.SetOpacity(1.0f);  // Ensure full opacity
            OutputDebugStringW(L"Favorites button found and enabled\n");
        }
        else
        {
            OutputDebugStringW(L"WARNING: Favorites button (XuiFavorites) not found!\n");
        }
        GetChildById( L"XuiOptions", &m_button3 );
		
		m_button3.SetText(L"N/A");  // Rename achievements button to N/A (achievements disabled)
		GetChildById( L"XuiAbout", &m_button4 );
		GetChildById( L"XuiQuit", &m_button5 );
		GetChildById( L"XuiStorageDevice", &m_Storage );
		GetChildById( L"EffectScene", &m_EffectScene );
	 
		GetChildById( L"XuiLabelSignInInfo", &m_SignInLabel );
		GetChildById( L"XuiMusicToggle", &m_MusicToggle );
		GetChildById( L"XuiText1", &m_VersionText );
		//GetChildById( L"XuiLogo", &m_Logo );
		 
		// Set version text dynamically
		m_VersionText.SetText(L"Version 0.33 Beta");
	 
		UpdatePresence(CONTEXT_GAMESTATE_IDLE);
 
		SetEffectValue( 20 );

        return S_OK;
    }




public:

    // Define the class. The class name must match the ClassOverride property
    // set for the scene in the UI Authoring tool.
    XUI_IMPLEMENT_CLASS( CSnes360Menu, L"MainScene", XUI_CLASS_SCENE )
};

 

HRESULT RenderGame( IDirect3DDevice9 *pDevice )
	{
    // Render game graphics.
    pDevice->Clear(
        0,
        NULL,
        D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
        D3DCOLOR_ARGB( 255, 32, 32, 64 ),
        1.0,
        0 );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: RegisterXuiClasses
// Desc: Registers all the scene classes.
//--------------------------------------------------------------------------------------
HRESULT CSnes360App::RegisterXuiClasses()
{
    // We must register the video control classes
	XuiVideoRegister();
	XuiSoundXAudioRegister();
	CSnes360Menu::Register();
	CRomListScene::Register();
	CRomList::Register();
	CFavoritesListScene::Register();
	CFavoritesList::Register();
	CInGameOptions::Register();
	CEffectScene::Register();

    // Register any other classes necessary for the app/scene
    return S_OK;
}

HRESULT InitD3D( IDirect3DDevice9 **ppDevice, 
    D3DPRESENT_PARAMETERS *pd3dPP )
{
    IDirect3D9 *pD3D;

	// Get the user video settings.
	XVIDEO_MODE VideoMode; 
	XMemSet( &VideoMode, 0, sizeof(XVIDEO_MODE) ); 
	XGetVideoMode( &VideoMode );

    pD3D = Direct3DCreate9( D3D_SDK_VERSION );


    // Set up the presentation parameters.
    ZeroMemory( pd3dPP, sizeof( D3DPRESENT_PARAMETERS ) );

	if (!VideoMode.fIsWideScreen)
	{
		pd3dPP->Flags |=  D3DPRESENTFLAG_NO_LETTERBOX;
	}

    pd3dPP->BackBufferWidth        = 1280;
    pd3dPP->BackBufferHeight       = 720;
    pd3dPP->BackBufferFormat       = D3DFMT_X8R8G8B8;
    pd3dPP->BackBufferCount        = 1;
    pd3dPP->MultiSampleType        = D3DMULTISAMPLE_NONE;
    pd3dPP->SwapEffect             = D3DSWAPEFFECT_DISCARD;
    pd3dPP->PresentationInterval   = D3DPRESENT_INTERVAL_ONE;
    
    // Create the device.
    return pD3D->CreateDevice(
                    0, 
                    D3DDEVTYPE_HAL,
                    NULL,
                    D3DCREATE_HARDWARE_VERTEXPROCESSING,
                    pd3dPP,
                    ppDevice );
}



//--------------------------------------------------------------------------------------
// Name: UnregisterXuiClasses
// Desc: Unregisters all the scene classes.
//--------------------------------------------------------------------------------------
HRESULT CSnes360App::UnregisterXuiClasses()
{
    CSnes360Menu::Unregister();
	CRomListScene::Unregister();
	CRomList::Unregister();
	CFavoritesListScene::Unregister();
	CFavoritesList::Unregister();
	CInGameOptions::Unregister();
	CEffectScene::Unregister();
    return S_OK;
}

 
 
//--------------------------------------------------------------------------------------
// Name: main
// Desc: Application entry point.
//--------------------------------------------------------------------------------------

IDirect3DDevice9 *pDevice;
D3DPRESENT_PARAMETERS d3dpp;
BOOL IsCurrentlyInGame = false;


CSnes360App app;
CRomPathSettings romPaths;
HXUIOBJ phObj = NULL;

// In Game Scene
HXUIOBJ hScene;
HXUIOBJ hMainScene;
HXUIOBJ hRomListScene;
 

VOID __cdecl main()
{
    // Declare an instance of the XUI framework application.
    
	
    HRESULT hr;

	MapInternalDrives();
	XSetFileCacheSize(0x10000);
	XMountUtilityDriveEx(XMOUNTUTILITYDRIVE_FORMAT0,8192, 0);
	XboxSetDefaultValues();
 
    // Initialize D3D
    hr = InitD3D( &pDevice, &d3dpp );
 
    // Initialize the application.    
	 
    hr = app.InitShared( pDevice, &d3dpp, 
        XuiD3DXTextureLoader );
 
    if( FAILED( hr ) )
    { 
        OutputDebugString( "Failed intializing application.\n" );
        return;
    }

    // Register a default typeface
    hr = app.RegisterDefaultTypeface( L"Arial Unicode MS", L"file://game:/media/Snes360.ttf" );
    if( FAILED( hr ) )
    {
        OutputDebugString( "Failed to register default typeface.\n" );
        return;
    }

	// Try to load settings.xml, create default if it doesn't exist
	if (!romPaths.Load("GAME:\\settings.xml"))
	{
		// Create default settings.xml file
		romPaths.CreateDefaultSettings("GAME:\\settings.xml");
		// Load the newly created file
		romPaths.Load("GAME:\\settings.xml");
	}

	if (GetFileAttributes(romPaths.m_PreviewPath.c_str()) == -1)
	{
		// create preview dir if it doesnt exist
		wchar_t opath[MAX_PATH]; 
		swprintf_s(opath, L"%S", romPaths.m_PreviewPath.c_str());
		CreateDirectoryAnyDepth(opath);			

	}
	
	snesStoreage.Initialise();

    // Create the notification listener to listen for XMP notifications
    //hNotificationListener = XNotifyCreateListener( XNOTIFY_XMP );
     
    // Initialize XMP state variable
    XMPGetStatus( &XMPState );

	XMPCreateTitlePlaylist( g_SongDescriptors,
                                       1,
                                       XMP_CREATETITLEPLAYLISTFLAG_NONE,
                                       L"MyPlaylist",
                                       NULL,
                                       &hXMPPlaylist );

    XMPSetPlaybackBehavior( XMP_PLAYBACKMODE_INORDER,
                            XMP_REPEATMODE_PLAYLIST,
                            0,
                            NULL );

    // Play the playlist
    XMPPlayTitlePlaylist( hXMPPlaylist, NULL, NULL );

 
    // Load the skin file used for the scene.
	app.LoadSkin( L"file://game:/media/Snes360.xzp#..\\Xbox\\Skin\\skin.xur" );
     	
	XuiSceneCreate( L"file://game:/media/Snes360.xzp#..\\Xbox\\Skin\\", L"Mario.xur", NULL, &hMainScene );
	 

	XuiSceneNavigateFirst( app.GetRootObj(), hMainScene, XUSER_INDEX_FOCUS );
  
	//m_Achievements = new BYTE[XACHIEVEMENT_SIZE_FULL * ACHIEVEMENT_COUNT];  // Achievements disabled
	//DoAchievo(ACHIEVEMENT_HOMEBREW_UNLOCKED);  // Achievements disabled

    while( TRUE ) {

		if (!IsCurrentlyInGame)
		{			 
			// Render game graphics.
 
			RenderGame( pDevice );
 
			snesStoreage.Update();
			 
			// Update XUI
			app.RunFrame();
			
			// Per-frame updates for ROM list scene (keyboard handling, etc.)
			if (hRomListScene)
			{
				CRomListScene* pScene = NULL;
				if (SUCCEEDED(XuiObjectFromHandle(hRomListScene, (VOID**)&pScene)))
				{
					pScene->UpdatePerFrame();
				}
			}

			// Render XUI
			hr = app.Render();

			/*D3DXMATRIX matView;
			int NewWidth = uWidth - (HOverscan * 2);
			int NewHeight = uHeight - (VOverscan * 2);
			D3DXVECTOR2 vScaling = D3DXVECTOR2( NewWidth / 1280.0f, NewHeight / 720.0f );
			D3DXVECTOR2 vTranslation = D3DXVECTOR2( (float)HOverscan, (float)VOverscan );
			D3DXMatrixTransformation2D( &matView, NULL, 0.0f, &vScaling, NULL, 0.0f, &vTranslation );
			XuiRenderSetViewTransform( app.GetDC(), &matView );*/

			

			// Update XUI Timers
			hr = XuiTimersRun();

			// Present the frame.
			pDevice->Present( NULL, NULL, NULL, NULL );
		}
    }

    // Free resources, unregister custom classes, and exit.
	//delete(m_Achievements);  // Achievements disabled
    app.Uninit();
    pDevice->Release();


}


//VOID EnumerateAchievements()  // Achievements disabled
//{
//    // Achievements disabled - function body commented out
//    return;
    /*  // All achievement code below is commented out

// Annotation:
// Reading an achievement will cause an MU access which is slow.  Therefore,
// reading achievements is an asynchronous operation.  Creating an event to
// signal the completion of the operation allows you to continue other
// processing and rendering while the write takes place.

    // Create event for asynchronous enumeration
    HANDLE hEventComplete = CreateEvent( NULL, FALSE, FALSE, NULL );
 
    XOVERLAPPED xov;

    ZeroMemory( &xov, sizeof(XOVERLAPPED) );
    xov.hEvent = hEventComplete;

    HANDLE hEnum;
    DWORD cbBuffer;

// Annotation:
// This sample code retrieves the full details of an achievement.  The
// dwDetailFlags (4th) parameter below can be used to control the actual
// fields of each achievement that are filled in.  This can be helpful
// when memory is at a premium and you don't need the image data (for
// example).
//
// The two first two parameters control the title and the user 
// making the request. The third is for the user
// whose achievements are read. When the third parameter is set to
// INVALID_XUID, the read is for the user's own achievements.

    // Create enumerator for the default device
    DWORD dwStatus = XUserCreateAchievementEnumerator(
        0,                          // retrieve achievements for the current title
        0,                          // local signed-in user 0 is making the request
		INVALID_XUID ,               // achievements for the current user are to be found 
        XACHIEVEMENT_DETAILS_ALL,   // information on all achievements is to be retrieved
        0,                          // starting achievement index
        ACHIEVEMENT_COUNT,          // number of achievements to return
        &cbBuffer,             // bytes needed
        &hEnum );

    if( dwStatus != ERROR_SUCCESS )
    {
        CloseHandle( hEventComplete );
         
    }
    

    // Enumerate display names

// Annotation
// Once the enumerator is created it can be pumped for information.
// The read is asynchronous so be sure to use that to your advantage
// to keep your title responsive.

    if( XEnumerate( hEnum, m_Achievements, sizeof( m_Achievements ),
                    NULL, &xov ) == ERROR_IO_PENDING )
    {
        DWORD dwReturnCount;

        // Wait on hEventComplete handle
        if( XGetOverlappedResult( &xov, &dwReturnCount, TRUE ) == ERROR_SUCCESS )
        {
            dwAchievementCount = dwReturnCount;

        }
    }

    CloseHandle( hEnum );
    CloseHandle( hEventComplete );
    */
//}  // Achievements disabled

void UpdatePresence(DWORD type)
{	 
	XUserSetContext(  snesStoreage.GetSignedInUser(), CONTEXT_GAMESTATE, type );	

	            int iCursorY = 32;
                                   

	XUserSetProperty( snesStoreage.GetSignedInUser(), PROPERTY_NUM_ROMS, sizeof( iCursorY ), &iCursorY );

	XUserSetContext(  snesStoreage.GetSignedInUser(), X_CONTEXT_PRESENCE, CONTEXT_PRESENCE_PRESENCE );

}
 
//void DoAchievo(DWORD AchievoID)  // Achievements disabled
//{
//	// Achievements disabled - function body commented out
//	return;
	/*  // All achievement code below is commented out

	HANDLE hEventComplete = CreateEvent( NULL, FALSE, FALSE, FALSE, NULL );

	XOVERLAPPED xov;

    ZeroMemory( &xov, sizeof( XOVERLAPPED ) );
    xov.hEvent = hEventComplete;

	XUSER_ACHIEVEMENT Achievements;
	Achievements.dwUserIndex = snesStoreage.GetSignedInUser();
    Achievements.dwAchievementId = AchievoID;

	XUserWriteAchievements( 1, &Achievements, &xov );

	XGetOverlappedResult( &xov, NULL, TRUE );
     
    CloseHandle( hEventComplete );
    */
//}  // Achievements disabled
 
 
