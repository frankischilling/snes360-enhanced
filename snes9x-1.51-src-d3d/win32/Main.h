#include <xtl.h>
#include <xui.h>
#include <xuiapp.h>


#ifndef MAIN_H
#define MAIN_H

void UpdatePresence(DWORD type);

class CSnes360App : public CXuiModule
{
protected:
    // Override RegisterXuiClasses so that CMyApp can register classes.
    virtual HRESULT RegisterXuiClasses();

    // Override UnregisterXuiClasses so that CMyApp can unregister classes. 
    virtual HRESULT UnregisterXuiClasses();
 	
};


class CEffectScene : public CXuiSceneImpl
{
    // simple vertex type used when rendering our texture
    struct VertexType
    {
        FLOAT x;
        FLOAT y;
        FLOAT u;
        FLOAT v;
    };

    DWORD m_dwTexWidth;       // width of texture and render target
    DWORD m_dwTexHeight;      // height of texture and render target
    IDirect3DTexture9* m_pTexTarget;       // texture resolved from render target
    IDirect3DTexture9* m_pTexDisplacement; // our displacement texture
    IDirect3DSurface9* m_pRenderTarget;    // used for render children to texture

    IDirect3DVertexShader9* m_pVertexShader;
    IDirect3DPixelShader9* m_pPixelShader;
    IDirect3DVertexDeclaration9* m_pVertexDecl;

    // effect parameters and state
    FLOAT m_fDisplacementFactor;
    FLOAT m_fDisplacement;

    // Message map. Here we tie messages to message handlers.
    XUI_BEGIN_MSG_MAP()
        XUI_ON_XM_RENDER( OnRender )
    XUI_END_MSG_MAP()

public:
    // Define the class. The class name must match the ClassOverride property
    // set for the scene in the UI Authoring tool.
    XUI_IMPLEMENT_CLASS( CEffectScene, L"EffectScene", XUI_CLASS_SCENE )

        

public:
    // public method to set the displacement factor.  This affects how much 
    // displacement is done in the pixel shader
    VOID    SetDisplacementFactor( FLOAT fFactor );
	HRESULT InitSurfacesAndShaders( IDirect3DDevice9* pDevice );
	VOID RenderChildrenToTexture( XUIMessageRender* pData );
	VOID RenderObjectTexture( IDirect3DDevice9* pDevice );
	HRESULT OnRender( XUIMessageRender* pRenderData, BOOL& bHandled );
	CEffectScene();
	~CEffectScene();
 

 
};

#endif