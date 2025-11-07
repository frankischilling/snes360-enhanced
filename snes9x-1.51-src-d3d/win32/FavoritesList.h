//--------------------------------------------------------------------------------------
// FavoritesList.h
//
// Favorites list implementation for SNES360
//--------------------------------------------------------------------------------------
#include <xtl.h>
#include <xui.h>
#include <xuiapp.h>
#include <algorithm>
#include <new>
#include <iostream>
#include <vector>

#include "GeneralFunctions.h"
#include "snes9x.h"

#ifndef FAVORITESLIST_H
#define FAVORITESLIST_H

class CFavoritesList : CXuiListImpl
{
public:
	XUI_IMPLEMENT_CLASS(CFavoritesList, L"SnesFavoritesList", XUI_CLASS_LIST);

	XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT(OnInit)
		XUI_ON_XM_GET_SOURCE_TEXT(OnGetSourceDataText)
		XUI_ON_XM_GET_ITEMCOUNT_ALL(OnGetItemCountAll)
		XUI_ON_XM_NOTIFY( OnNotify )
	XUI_END_MSG_MAP()

	CFavoritesList();
    HRESULT OnInit( XUIMessageInit* pInitData, BOOL& bHandled );
	HRESULT OnNotify( XUINotify *hObj, BOOL& bHandled );
	HRESULT OnGetSourceDataText(XUIMessageGetSourceText *pGetSourceTextData, BOOL& bHandled);
	HRESULT OnGetItemCountAll(XUIMessageGetItemCount *pGetItemCountData, BOOL& bHandled);
	void LoadFavorites();
};

//--------------------------------------------------------------------------------------
// Scene implementation class.
//--------------------------------------------------------------------------------------
class CFavoritesListScene : public CXuiSceneImpl
{
protected:
    // Control and Element wrapper objects.
    CXuiControl m_PlayRom;
	CXuiControl m_Back; 
	CXuiElement m_PreviewImage;
	CXuiList m_FavoritesList;
 
	CXuiScene m_EffectScene;      // the effect scene we're controlling
   
    // Message map.
    XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT( OnInit )
		XUI_ON_XM_NOTIFY_PRESS( OnNotifyPress )
    XUI_END_MSG_MAP()

public:
    HRESULT OnInit( XUIMessageInit* pInitData, BOOL& bHandled );
	HRESULT OnNotifyPress( HXUIOBJ hObjPressed, BOOL& bHandled );
	VOID   SetEffectValue( INT nValue );

public:
    // Define the class. The class name must match the ClassOverride property
    // set for the scene in the UI Authoring tool.
    XUI_IMPLEMENT_CLASS( CFavoritesListScene, L"FavoritesListScene", XUI_CLASS_SCENE )
};

#endif

