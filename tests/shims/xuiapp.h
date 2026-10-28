#ifndef TEST_XUIAPP_H
#define TEST_XUIAPP_H
#include <xtl.h>
typedef void* HXUIOBJ;
#define XUI_MB_CENTER_ON_PARENT 0
extern int messageCount;
extern HRESULT navigateResult;
struct CXuiScene
{
    HRESULT ShowMessageBoxEx(LPCWSTR, HXUIOBJ, LPCWSTR, LPCWSTR, int, LPCWSTR*, int, DWORD, HXUIOBJ*)
    { ++messageCount; return S_OK; }
    HRESULT NavigateForward(HXUIOBJ) { return navigateResult; }
};
HRESULT XuiSceneCreate(LPCWSTR, LPCWSTR, void*, HXUIOBJ*);
void XuiDestroyObject(HXUIOBJ);
#endif
