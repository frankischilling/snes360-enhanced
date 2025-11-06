/* Direct3D.h - written by OV2 */
#ifndef W9XDIRECT3D_H
#define W9XDIRECT3D_H

#include <xtl.h>
#include <d3d9.h>
#include "render.h"
#include <vector>
#include "wsnes9x.h"
 
typedef struct _VERTEX {
		float x, y, z;
		float rhw;
		float tx, ty;
		_VERTEX() {}
		_VERTEX(float x,float y,float z,float rhw,float tx,float ty) {
			this->x=x;this->y=y;this->z=z;this->rhw=rhw;this->tx=tx;this->ty=ty;
		}
} VERTEX; //our custom vertex with a constuctor for easier assignment

 
class CDirect3D
{
private:
	bool                  init_done;					//has initialize been called?
	LPDIRECT3D9           pD3D;
	
	LPDIRECT3DTEXTURE9    drawSurface;					//the texture used for all drawing operations

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration;
	D3DVertexShader* g_pGradientVertexShader;
	D3DVertexDeclaration* g_pGradientVertexDecl;
	D3DPixelShader* g_pPixelShader;
	LPD3DXBUFFER ppShader;
	D3DPRESENT_PARAMETERS dPresentParams;
	int iFilterScale;									//the current maximum filter scale (at least 2)
	unsigned int afterRenderWidth, afterRenderHeight;	//dimensions after filter has been applied
	unsigned int quadTextureSize;						//size of the texture (only multiples of 2)
	bool fullscreen;									//are we currently displaying in fullscreen mode
	
	VERTEX triangleStripVertices[4];					//the 4 vertices that make up our display rectangle

	bool blankTexture(LPDIRECT3DTEXTURE9 texture);
	void createDrawSurface();
	void destroyDrawSurface();
	bool changeDrawSurfaceSize(int iScale);
	void setupVertices();
	void setupVerticesForStretch();
	bool resetDevice();
 
 
public:
	CDirect3D();
	~CDirect3D();
	LPDIRECT3DDEVICE9     pDevice;
	bool initialize(D3DDevice *pDevice);
	void deInitialize();
	void render(SSurface Src);
	void renderTextureOnly(SSurface Src);
	bool changeRenderSize(unsigned int newWidth, unsigned int newHeight);
	bool setFullscreen(bool fullscreen);
	void setSnes9xColorFormat();
	void fillModesListView(HWND listView,std::vector<dMode> *modeVector);
	HRESULT RenderUI( IDirect3DDevice9* pDevice, UINT uWidth, UINT uHeight );
	void doStretch(void);
};

#endif