/* Direct3D.cpp - written by OV2 */

#pragma comment( lib, "d3d9" )
#pragma comment( lib, "d3dx9" )
#pragma comment( lib, "DxErr9" )

#include <xtl.h>
#include <xui.h>
#include "snes9x.h"

#include <tchar.h>
#include "direct3d.h"
#include "gfx.h"
#include "wsnes9x.h"
#include "GeneralFunctions.h"
#include "Storage.h"

extern bool in_display_dlg;
extern bool wasInXUI;
extern bool exitFromXUI;
extern BOOL run_loop;
extern HXUIOBJ hScene;
extern HXUIOBJ phObj;
extern GameStorage snesStoreage;
extern int bAdjustScreen;
extern struct SSettings Settings;
void ConvertDepth (SSurface *src, SSurface *dst, RECT *);
int Init_2xSaI (uint32 BitFormat);
#define RenderMethod ((Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512) ? RenderMethodHiRes : RenderMethod)
 
D3DVERTEXELEMENT9 decl[] = 
{
{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
D3DDECL_END()
};

//--------------------------------------------------------------------------------------
// Vertex and pixel shaders for gradient background rendering
//--------------------------------------------------------------------------------------
static const CHAR* g_strGradientShader =
    "struct VS_IN                                              \n"
    "{                                                         \n"
    "   float4   Position     : POSITION;                      \n"
    "   float2   UV        : TEXCOORD0;                        \n"
    "};                                                        \n"
    "                                                          \n"
    "struct VS_OUT                                             \n"
    "{                                                         \n"
    "   float4 Position       : POSITION;                      \n"
    "   float2 UV        : TEXCOORD0;                        \n"
    "};                                                        \n"
    "                                                          \n"
    "VS_OUT GradientVertexShader( VS_IN In )                   \n"
    "{                                                         \n"
    "   VS_OUT Out;                                            \n"
    "   Out.Position = In.Position;                            \n"
    "   Out.UV  = In.UV;                               \n"
    "   return Out;                                            \n"
    "}                                                         \n";
   
//-------------------------------------------------------------------------------------
// Pixel shader
//-------------------------------------------------------------------------------------
const char*                                 g_strPixelShaderProgram =
    " struct PS_IN                                 "
    " {                                            "
    "     float2 TexCoord : TEXCOORD;              "
    " };                                           "  // the vertex shader
    "                                              "
    " sampler detail;                              "
    "                                              "
    " float4 main( PS_IN In ) : COLOR              "
    " {                                            "
    "     return tex2D( detail, In.TexCoord );     "  // Output color
    " }     ";   

/* CDirect3D::CDirect3D()
sets default values for the variables
*/

RECT drawRect;

HANDLE RenderThread = NULL;

CDirect3D::CDirect3D()
{
 
	init_done = false;
	pD3D = NULL;
	pDevice = NULL;
	drawSurface = NULL;
	vertexBuffer = NULL;
	afterRenderWidth = 0;
	afterRenderHeight =0;
	quadTextureSize = 0;
	fullscreen = true;
	iFilterScale = 0;

		/*if(!(RenderThread = (HANDLE)_beginthreadex(NULL,0,&SoundThread,(void *)this,0,NULL))) {
		MessageBox (GUI.hWnd, TEXT("Unable to create mixing thread, XAudio2 will not work."),
                        TEXT("Snes9X - XAudio2"),
                        MB_OK | MB_ICONWARNING);
		DeInitXAudio2();
		return false;*/
}

/* CDirect3D::~CDirect3D()
releases allocated objects
*/
CDirect3D::~CDirect3D()
{
	deInitialize();
}


/*  CDirect3D::initialize
initializes Direct3D (always in window mode)
IN:
hWnd	-	the HWND of the window in which we render/the focus window for fullscreen
-----
returns true if successful, false otherwise
*/
bool CDirect3D::initialize(D3DDevice *pDev)
{
 
	HRESULT hr;
	pDevice = pDev;
	VOID* pCode = NULL;
    DWORD dwSize = 0;

	// Get the user video settings.
	XVIDEO_MODE VideoMode; 
	XMemSet( &VideoMode, 0, sizeof(XVIDEO_MODE) ); 
	XGetVideoMode( &VideoMode );

	ZeroMemory(&dPresentParams, sizeof(dPresentParams));

	if (!VideoMode.fIsWideScreen)
	{
		dPresentParams.Flags |=  D3DPRESENTFLAG_NO_LETTERBOX;
	}

	dPresentParams.BackBufferFormat = D3DFMT_X8R8G8B8; 
	dPresentParams.BackBufferWidth = 1280;
	dPresentParams.BackBufferHeight = 720;
	dPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE ;
 
	pDevice->Reset(&dPresentParams);
 

	    // Compile pixel shader.
    ID3DXBuffer* pPixelShaderCode;
    ID3DXBuffer*  pPixelErrorMsg;
    hr = D3DXCompileShader( g_strPixelShaderProgram,
                            ( UINT )strlen( g_strPixelShaderProgram ),
                            NULL,
                            NULL,
                            "main",
                            "ps_2_0",
                            0,
                            &pPixelShaderCode,
                            &pPixelErrorMsg,
                            NULL );
    if( FAILED( hr ) )
    {
        if( pPixelErrorMsg )
            OutputDebugString( ( char* )pPixelErrorMsg->GetBufferPointer() );
        return E_FAIL;
    }

    // Create pixel shader.
    pDevice->CreatePixelShader( ( DWORD* )pPixelShaderCode->GetBufferPointer(),
                                     &g_pPixelShader );

	    // Create vertex declaration
    if( NULL == g_pGradientVertexDecl )
    {
        static const D3DVERTEXELEMENT9 decl[] =
        {
			{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };

        if( FAILED( pDevice->CreateVertexDeclaration( decl, &g_pGradientVertexDecl ) ) )
            return E_FAIL;
    }
 
	    // Create vertex shader
    if( NULL == g_pGradientVertexShader )
    {
        ID3DXBuffer* pShaderCode;
        if( FAILED( D3DXCompileShader( g_strGradientShader, strlen( g_strGradientShader ),
                                       NULL, NULL, "GradientVertexShader", "vs_2_0", 0,
                                       &pShaderCode, NULL, NULL ) ) )
            return false;

        if( FAILED( pDevice->CreateVertexShader( ( DWORD* )pShaderCode->GetBufferPointer(),
                                                      &g_pGradientVertexShader ) ) )
            return false;

        pShaderCode->Release();
    }

	if (NULL == vertexDeclaration)
	{
		pDevice->CreateVertexDeclaration(decl,&vertexDeclaration);
	}

	if (NULL == vertexBuffer)
	{
		hr = pDevice->CreateVertexBuffer(sizeof(triangleStripVertices),D3DUSAGE_CPU_CACHED_MEMORY,0,0,&vertexBuffer,NULL);
		if(FAILED(hr)) {
			DXTRACE_ERR_MSGBOX(TEXT("Error creating vertex buffer"), hr);
			return false;
		}
	}

	//D3DXCompileShaderFromFile("game:\\Media\\Shaders\\Super2xSaI.fx",NULL,NULL,"S_FRAGMENT","ps_3_0",D3DXSHADER_MICROCODE_BACKEND_NEW,&ppShader,NULL,NULL);
	 
	//hr = pDevice->CreatePixelShader((DWORD*)ppShader->GetBufferPointer(),&g_pPixelShader);
	  
	//VideoMemory controls if we want bilinear filtering
	if(GUI.VideoMemory) {
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	} else {
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}

	pDevice->SetVertexShader( g_pGradientVertexShader );		
	pDevice->SetPixelShader( g_pPixelShader );

    pDevice->SetVertexDeclaration( g_pGradientVertexDecl );
	pDevice->SetRenderState( D3DRS_VIEWPORTENABLE, FALSE );

	

	init_done = true;

	return true;

}
 

void CDirect3D::deInitialize()
{
	pDevice->UnsetAll();

	destroyDrawSurface();
 
	if (g_pGradientVertexDecl)
	{
		g_pGradientVertexDecl->Release();
		g_pGradientVertexDecl = NULL;
	}

	if (g_pGradientVertexShader)
	{
		g_pGradientVertexShader->Release();
		g_pGradientVertexShader = NULL;

	}

	if (g_pPixelShader)
	{
		g_pPixelShader->Release();
		g_pPixelShader = NULL;

	}

	if(vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}

	if (vertexDeclaration)
	{
		vertexDeclaration->Release();
		vertexDeclaration = NULL;
	}


	init_done = false;
	afterRenderWidth = 0;
	afterRenderHeight = 0;
	quadTextureSize = 0;
	fullscreen = false;
	iFilterScale = 0;
}

/*  CDirect3D::render
does the actual rendering, changes the draw surface if necessary and recalculates
the vertex information if filter output size changes
IN:
Src		-	the input surface
*/
void CDirect3D::render(SSurface Src)
{
	SSurface Dst;
	RECT dstRect;
	int iNewFilterScale;
	D3DLOCKED_RECT lr;
	D3DLOCKED_RECT lrConv;
	HRESULT hr;

	if(!init_done) return;
	
	if (bAdjustScreen == 1)
	{
		doStretch();
	}

	if (wasInXUI)
	{
		wasInXUI = false;	

		initialize(pDevice);	
		pDevice->SetStreamSource(0, NULL, 0, 0);
		vertexBuffer->Release();
		vertexBuffer = NULL;
		if (NULL == vertexBuffer)
		{
			hr = pDevice->CreateVertexBuffer(sizeof(triangleStripVertices),D3DUSAGE_CPU_CACHED_MEMORY,0,0,&vertexBuffer,NULL);
			changeDrawSurfaceSize(2);
		 
		}
		 
		
		if (exitFromXUI)
		{
			run_loop = false;
 
		}


	}
 
	iNewFilterScale = 2;
	if(iNewFilterScale!=iFilterScale) {
		changeDrawSurfaceSize(iNewFilterScale);
	}
 

	if(FAILED(hr = drawSurface->LockRect(0, &lr, NULL, D3DLOCK_NOOVERWRITE))) {
		DXTRACE_ERR_MSGBOX( TEXT("Unable to lock texture"), hr);
		return;
	} else {
		Dst.Surface = (unsigned char *)lr.pBits;
		Dst.Height = quadTextureSize;
		Dst.Width = quadTextureSize;
		Dst.Pitch = lr.Pitch;

		
		RenderMethod (Src, Dst, &dstRect);
 
		// Display FPS and other messages on the rendered surface
		if(!Settings.AutoDisplayMessages)
		{
			int filterScale = GetFilterScale(GUI.Scale);
			S9xDisplayMessages((uint16*)Dst.Surface, Dst.Pitch/2, dstRect.right - dstRect.left, dstRect.bottom - dstRect.top, filterScale);
		}

		drawSurface->UnlockRect(0);
	}

	//if the output size of the render method changes we need new vertices
	if(afterRenderHeight != dstRect.bottom || afterRenderWidth != dstRect.right) {
		afterRenderHeight = dstRect.bottom;
		afterRenderWidth = dstRect.right;
		
		setupVertices();
	}
 
	
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    pDevice->SetTexture( 0, drawSurface); 
	pDevice->SetStreamSource(0,vertexBuffer,0,sizeof(VERTEX));		
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2); 	 
	pDevice->Present(NULL, NULL, NULL, NULL);
   
	return;
}


void CDirect3D::renderTextureOnly(SSurface Src)
{
	SSurface Dst;
	RECT dstRect;
	int iNewFilterScale;
	D3DLOCKED_RECT lr;
	D3DLOCKED_RECT lrConv;
	HRESULT hr;

	if(!init_done) return;

	 
	//create a new draw surface if the filter scale changes
	//at least factor 2 so we can display unscaled hi-res images
	iNewFilterScale = max(2,max(GetFilterScale(GUI.ScaleHiRes),GetFilterScale(GUI.Scale)));
	if(iNewFilterScale!=iFilterScale) {
		changeDrawSurfaceSize(iNewFilterScale);
	}
 
 
	if(FAILED(hr = drawSurface->LockRect(0, &lr, NULL, D3DLOCK_NOOVERWRITE))) {
		DXTRACE_ERR_MSGBOX( TEXT("Unable to lock texture"), hr);
		return;
	} else {
		Dst.Surface = (unsigned char *)lr.pBits;
		Dst.Height = quadTextureSize;
		Dst.Width = quadTextureSize;
		Dst.Pitch = lr.Pitch;

		
		RenderMethod (Src, Dst, &dstRect);
 
		drawSurface->UnlockRect(0);
	}
	
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    pDevice->SetTexture( 0, drawSurface); 
	pDevice->SetStreamSource(0,vertexBuffer,0,sizeof(VERTEX));		
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2); 	 
 
	return;
}

/*  CDirect3D::createDrawSurface
calculates the necessary texture size (multiples of 2)
and creates a new texture
*/
void CDirect3D::createDrawSurface()
{
	int neededSize;
	HRESULT hr;

	//we need at least 512 pixels (SNES_WIDTH * 2) so we can start with that value
	quadTextureSize = 512;
	neededSize = SNES_WIDTH * iFilterScale;
	while(quadTextureSize < neededSize)
		quadTextureSize *=2;

	if(!drawSurface) {
		hr = pDevice->CreateTexture(
			quadTextureSize, quadTextureSize,
			1, // 1 level, no mipmaps
			D3DUSAGE_CPU_CACHED_MEMORY , // dynamic textures can be locked
			D3DFMT_LIN_R5G6B5,
			0,
			&drawSurface,
			NULL );

		if(FAILED(hr)) {
			DXTRACE_ERR_MSGBOX(TEXT("Error while creating texture"), hr);
			return;
		}

		blankTexture(drawSurface);
	}

 
}

/*  CDirect3D::destroyDrawSurface
releases the old textures (if allocated)
*/
void CDirect3D::destroyDrawSurface()
{
	if(drawSurface) {
		drawSurface->Release();
		drawSurface = NULL;
	}


}

/*  CDirect3D::blankTexture
clears a texture (fills it with zeroes)
IN:
texture		-	the texture to be blanked
-----
returns true if successful, false otherwise
*/
bool CDirect3D::blankTexture(LPDIRECT3DTEXTURE9 texture)
{
	D3DLOCKED_RECT lr;
	HRESULT hr;

	if(FAILED(hr = texture->LockRect(0, &lr, NULL, 0))) {
		DXTRACE_ERR_MSGBOX( TEXT("Unable to lock texture"), hr);
		return false;
	} else {
		XMemSet(lr.pBits, 0, lr.Pitch * quadTextureSize);
		texture->UnlockRect(0);
		return true;
	}
}

/*  CDirect3D::changeDrawSurfaceSize
changes the draw surface size: deletes the old textures, creates a new texture
and calculate new vertices
IN:
iScale	-	the scale that has to fit into the textures
-----
returns true if successful, false otherwise
*/
bool CDirect3D::changeDrawSurfaceSize(int iScale)
{
	iFilterScale = iScale;

	if(pDevice) {
		destroyDrawSurface();
		createDrawSurface();
		setupVertices();
		return true;
	}
	return false;
}

/*  CDirect3D::setupVertices
calculates the vertex coordinates
(respecting the stretch and aspect ratio settings)
*/
void CDirect3D::setupVertices()
{
	float xFactor;
	float yFactor;
	float minFactor;
	float renderWidthCalc,renderHeightCalc;
	
	int hExtend = GUI.HeightExtend ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT;
	float snesAspect = (float)GUI.AspectWidth/hExtend;
	void *pLockedVertexBuffer;

	int Offset = ((512 - (SNES_HEIGHT*2))*2);
	GUI.Stretch = true;

	if(GUI.Stretch) {
		if(GUI.AspectRatio) {
			//fix for hi-res images with FILTER_NONE
			//where we need to correct the aspect ratio
			renderWidthCalc = (float)afterRenderWidth;
			renderHeightCalc = (float)afterRenderHeight;
			if(renderWidthCalc/renderHeightCalc>snesAspect)
				renderWidthCalc = renderHeightCalc * snesAspect;
			else if(renderWidthCalc/renderHeightCalc<snesAspect)
				renderHeightCalc = renderWidthCalc / snesAspect;

			xFactor = (float)dPresentParams.BackBufferWidth / renderWidthCalc;
			yFactor = (float)dPresentParams.BackBufferHeight / renderHeightCalc;
			minFactor = xFactor < yFactor ? xFactor : yFactor;

			drawRect.right = (LONG)(renderWidthCalc * minFactor);
			drawRect.bottom = (LONG)(renderHeightCalc * minFactor);

			drawRect.left = (dPresentParams.BackBufferWidth - drawRect.right) / 2;
			drawRect.top = (dPresentParams.BackBufferHeight - drawRect.bottom) / 2;

			drawRect.right += drawRect.left;
			drawRect.bottom += drawRect.top;
		} else {
			drawRect.top = 0;
			drawRect.left = 0;
			drawRect.right = dPresentParams.BackBufferWidth;
			drawRect.bottom = dPresentParams.BackBufferHeight;
		}
	} else {
		drawRect.left = ((int)(dPresentParams.BackBufferWidth) - (int)afterRenderWidth) / 2;
		drawRect.top = ((int)(dPresentParams.BackBufferHeight) - (int)afterRenderHeight) / 2;
		if(drawRect.left < 0) drawRect.left = 0;
		if(drawRect.top < 0) drawRect.top = 0;
		drawRect.right = drawRect.left + afterRenderWidth;
		drawRect.bottom = drawRect.top + afterRenderHeight;

		//the lines below would downsize the image if window size is smaller than the output, but
		//since the old directdraw code did not do that I've left it out
		//if(drawRect.right > dPresentParams.BackBufferWidth) drawRect.right = dPresentParams.BackBufferWidth;
		//if(drawRect.bottom > dPresentParams.BackBufferHeight) drawRect.bottom = dPresentParams.BackBufferHeight;
	}

	float tX = (float)afterRenderWidth / (float)quadTextureSize;
	float tY = (float)afterRenderHeight / (float)quadTextureSize;

	//we need to substract -0.5 from the x/y coordinates to match texture with pixel space
	//see http://msdn.microsoft.com/en-us/library/bb219690(VS.85).aspx
	triangleStripVertices[0] = VERTEX((float)drawRect.left + Settings.xoffset - 0.5f,(float)drawRect.bottom + Settings.yoffset - 0.5f,0.0f,1.0f,0.0f,tY);
	triangleStripVertices[1] = VERTEX((float)drawRect.left + Settings.xoffset - 0.5f,(float)drawRect.top + Settings.yoffset + Settings.yscale - 0.5f,0.0f,1.0f,0.0f,0.0f);
	triangleStripVertices[2] = VERTEX((float)drawRect.right + Settings.xoffset + Settings.xscale - 0.5f,(float)drawRect.bottom + Settings.yoffset - 0.5f,0.0f,1.0f,tX,tY);
	triangleStripVertices[3] = VERTEX((float)drawRect.right + Settings.xoffset + Settings.xscale - 0.5f,(float)drawRect.top + Settings.yoffset + Settings.yscale - 0.5f,0.0f,1.0f,tX,0.0f);


	pDevice->SetStreamSource(0, NULL, 0, 0);
	HRESULT hr = vertexBuffer->Lock(0,0,&pLockedVertexBuffer,NULL);
	XMemCpy(pLockedVertexBuffer,triangleStripVertices,sizeof(triangleStripVertices));
	vertexBuffer->Unlock();
}

void CDirect3D::setupVerticesForStretch()
{
	float xFactor;
	float yFactor;
	float minFactor;
	float renderWidthCalc,renderHeightCalc;
	
	int hExtend = GUI.HeightExtend ? SNES_HEIGHT_EXTENDED : SNES_HEIGHT;
	float snesAspect = (float)GUI.AspectWidth/hExtend;
	void *pLockedVertexBuffer;

	int Offset = ((512 - (SNES_HEIGHT*2))*2);
 
	float tX = (float)afterRenderWidth / (float)quadTextureSize;
	float tY = (float)afterRenderHeight / (float)quadTextureSize;

	//we need to substract -0.5 from the x/y coordinates to match texture with pixel space
	//see http://msdn.microsoft.com/en-us/library/bb219690(VS.85).aspx
	triangleStripVertices[0] = VERTEX((float)drawRect.left + Settings.xoffset - 0.5f,(float)drawRect.bottom + Settings.yoffset - 0.5f,0.0f,1.0f,0.0f,tY);
	triangleStripVertices[1] = VERTEX((float)drawRect.left + Settings.xoffset - 0.5f,(float)drawRect.top + Settings.yoffset + Settings.yscale - 0.5f,0.0f,1.0f,0.0f,0.0f);
	triangleStripVertices[2] = VERTEX((float)drawRect.right + Settings.xoffset + Settings.xscale - 0.5f,(float)drawRect.bottom + Settings.yoffset - 0.5f,0.0f,1.0f,tX,tY);
	triangleStripVertices[3] = VERTEX((float)drawRect.right + Settings.xoffset + Settings.xscale - 0.5f,(float)drawRect.top + Settings.yoffset + Settings.yscale - 0.5f,0.0f,1.0f,tX,0.0f);

	pDevice->SetStreamSource(0, NULL, 0, 0);
	HRESULT hr = vertexBuffer->Lock(0,0,&pLockedVertexBuffer,NULL);
	XMemCpy(pLockedVertexBuffer,triangleStripVertices,sizeof(triangleStripVertices));
	vertexBuffer->Unlock();
}

/*  CDirect3D::changeRenderSize
determines if we need to reset the device (if the size changed)
called with (0,0) whenever we want new settings to take effect
IN:
newWidth,newHeight	-	the new window size
-----
returns true if successful, false otherwise
*/
bool CDirect3D::changeRenderSize(unsigned int newWidth, unsigned int newHeight)
{
	if(!init_done)
		return false;

	//if we already have the wanted size no change is necessary
	//during fullscreen no changes are allowed
	if(fullscreen || dPresentParams.BackBufferWidth == newWidth && dPresentParams.BackBufferHeight == newHeight)
		return true;

	if(!resetDevice())
		return false;
	setupVertices();
	return true;
}

/*  CDirect3D::resetDevice
resets the device
called if surface was lost or the settings/display size require a device reset
-----
returns true if successful, false otherwise
*/
bool CDirect3D::resetDevice()
{
	if(!pDevice) return false;

	HRESULT hr;
	// Get the user video settings.
	XVIDEO_MODE VideoMode; 
	XMemSet( &VideoMode, 0, sizeof(XVIDEO_MODE) ); 
	XGetVideoMode( &VideoMode );

	//release prior to reset
	destroyDrawSurface();

	if (!VideoMode.fIsWideScreen)
	{
		dPresentParams.Flags |=  D3DPRESENTFLAG_NO_LETTERBOX;
	}

	//zero or unknown values result in the current window size/display settings
	dPresentParams.BackBufferWidth = 1280;
	dPresentParams.BackBufferHeight = 720;
	dPresentParams.BackBufferCount = GUI.DoubleBuffered?2:1;
	dPresentParams.BackBufferFormat = D3DFMT_X8R8G8B8;  
	dPresentParams.Windowed = false;
	dPresentParams.PresentationInterval = GUI.Vsync?D3DPRESENT_INTERVAL_ONE:D3DPRESENT_INTERVAL_IMMEDIATE;
	
 

	if(FAILED(hr = pDevice->Reset(&dPresentParams))) {
		DXTRACE_ERR(TEXT("Unable to reset device"), hr);
		return false;
	}

	//VideoMemory controls if we want bilinear filtering
	if(GUI.VideoMemory) {
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	} else {
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	//recreate the surface
	createDrawSurface();
	return true;
}

/*  CDirect3D::setSnes9xColorFormat
sets the color format to 16bit (since the texture is always 16bit)
no depth conversion is necessary (done by D3D)
*/
void CDirect3D::setSnes9xColorFormat()
{
	GUI.ScreenDepth = 16;
	GUI.BlueShift = 0;
	GUI.GreenShift = 6;
	GUI.RedShift = 11;
	S9xSetRenderPixelFormat (RGB565);
	Init_2xSaI (565);
	GUI.NeedDepthConvert = FALSE;
	GUI.DepthConverted = TRUE;
	return;
}

/*  CDirect3D::setFullscreen
enables/disables fullscreen mode
IN:
fullscreen	-	determines if fullscreen is enabled/disabled
-----
returns true if successful, false otherwise
*/
bool CDirect3D::setFullscreen(bool fullscreen)
{
	if(!init_done)
		return false;

 
	this->fullscreen = fullscreen;
	if(!resetDevice())
		return false;

	//present here to get a fullscreen blank even if no rendering is done
	pDevice->Present(NULL,NULL,NULL,NULL);
	setupVertices();
	return true;
}

//--------------------------------------------------------------------------------------
// Name: RenderUI
// Desc: Render the UI if the UI is active, otherwise it just returns
//--------------------------------------------------------------------------------------
HRESULT CDirect3D::RenderUI( IDirect3DDevice9* pDevice, UINT uWidth, UINT uHeight )
{


    return S_OK;
}


void CDirect3D::doStretch(void)
{	
	XINPUT_STATE state;

	pDevice->SetVertexShader( g_pGradientVertexShader );		
	pDevice->SetPixelShader( g_pPixelShader );

    pDevice->SetVertexDeclaration( g_pGradientVertexDecl );
	pDevice->SetRenderState( D3DRS_VIEWPORTENABLE, FALSE );
	while ( 1 )
	{

		XInputGetState( 0, &state );
 
		setupVerticesForStretch();
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		pDevice->SetTexture( 0, drawSurface); 
		pDevice->SetStreamSource(0,vertexBuffer,0,sizeof(VERTEX));		
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2); 	 
		pDevice->Present(NULL, NULL, NULL, NULL);
	 		 	
		if (state.Gamepad.sThumbLX <= -32000)
		{
			Settings.xoffset -= 1;
		}
		else if (state.Gamepad.sThumbLX >= 32000)
		{
			Settings.xoffset += 1;
		}

		if (state.Gamepad.sThumbLY <= -32000)
		{
			Settings.yoffset += 1;
		}	
		else if (state.Gamepad.sThumbLY >= 32000)
		{
			Settings.yoffset -= 1;
		}

		if (state.Gamepad.sThumbRX <= -32000 )
		{
			Settings.xscale -= 1;
		}
		else if (state.Gamepad.sThumbRX >= 32000)
		{
			Settings.xscale += 1;
		}

		if (state.Gamepad.sThumbRY <= -32000 )
		{
			Settings.yscale += 1;
		}	
		else if (state.Gamepad.sThumbRY >= 32000)
		{
			Settings.yscale -= 1;
		}

		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{	
			pDevice->SetTexture( 0, NULL); 
			pDevice->SetStreamSource(0, NULL, 0, 0);
			bAdjustScreen = 0;		
			break;
		}
		
	}
	 
}
