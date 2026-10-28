#include "RomLaunch.h"
#include "GameGenieInput.h"
#include "TitleText.h"
#include "RomFilter.h"
#include <cassert>
#include <iostream>
#include <vector>

int messageCount = 0;
HRESULT navigateResult = S_OK;
HXUIOBJ hScene = NULL;
extern int g_NumPendingGameGenieCodes;
extern char g_PendingGameGenieCodes[3][256];
static DWORD keyboardResult = ERROR_IO_PENDING, completion = ERROR_SUCCESS;
static bool signalled = false, eventFailure = false;
static HRESULT sceneResult = S_OK;
static std::wstring input;
static int launches = 0, events = 0, closes = 0, cancellations = 0;
static std::string launchedDirectory, launchedFilename;
static std::vector<std::string> applied;
HANDLE CreateEvent(void*, BOOL, BOOL, const char*)
{ if (eventFailure) return NULL; ++events; return reinterpret_cast<HANDLE>(1); }
BOOL CloseHandle(HANDLE handle) { assert(handle); ++closes; return TRUE; }
DWORD XCancelOverlapped(XOVERLAPPED*) { ++cancellations; return ERROR_SUCCESS; }
DWORD XGetOverlappedResult(XOVERLAPPED*, DWORD*, BOOL) { return completion; }
DWORD WaitForSingleObject(HANDLE, DWORD) { return signalled ? WAIT_OBJECT_0 : 258; }
DWORD XShowKeyboardUI(DWORD, DWORD, LPCWSTR, LPCWSTR, LPCWSTR, WCHAR* result, DWORD capacity, XOVERLAPPED*)
{ assert(input.size() < capacity); std::wcscpy(result, input.c_str()); return keyboardResult; }
HRESULT XuiSceneCreate(LPCWSTR, LPCWSTR, void*, HXUIOBJ* scene)
{ *scene = reinterpret_cast<HXUIOBJ>(2); return sceneResult; }
void XuiDestroyObject(HXUIOBJ scene) { assert(scene); }
void XMPPause(void*) {}
void XMPContinue(void*) {}
void SaveConfig() {}
int RunEmulation(char* directory, char* filename)
{
    ++launches; launchedDirectory = directory; launchedFilename = filename; applied.clear();
    for (int i = 0; i < g_NumPendingGameGenieCodes; ++i) applied.push_back(g_PendingGameGenieCodes[i]);
    return 0;
}

int main()
{
    CXuiScene owner;
    std::vector<std::string> codes;
    assert(ParseGameGenieInput(L"c222-d4dd, 8b99-17dd\tDD8C-C7A7", codes) && codes.size() == 3);
    assert(codes[0] == "C222-D4DD");
    assert(ParseGameGenieInput(L" , \t", codes) && codes.empty());
    assert(!ParseGameGenieInput(L"C222-D4DD,BAD", codes) && codes.empty());
    assert(!ParseGameGenieInput(L"C222-D4DD,C222-D4DD,C222-D4DD,C222-D4DD", codes));
    assert(!ParseGameGenieInput(L"C222-D4DX", codes));
    assert(!ParseGameGenieInput(L"C222D4DD-", codes));
    assert(!ParseGameGenieInput(std::wstring(1000, L'A'), codes));
    for (int length = 0; length < 1024; ++length)
    {
        std::string name(length, 'R');
        assert(RomTitleText(name.c_str()) == std::wstring(length, L'R'));
    }
    assert(RomTitleText(NULL).empty());

    CRomLaunch launch;
    std::vector<std::string> roms(6000, "game.smc"); roms[5999] = "last-game.smc";
    assert(FilterRomNames(roms, L"LAST-").size() == 1);
    assert(FilterRomNames(roms, L"no match").empty());
    assert(FilterRomNames(roms, L"").size() == 6000);
    std::vector<std::string> empty;
    assert(FilterRomNames(empty, L"").empty());
    input = L"c222-d4dd";
    assert(launch.Begin(owner, "GAME:\\Roms\\", roms[5999], true) == S_OK);
    roms.clear(); // Selection and list storage may change after opening the keyboard.
    assert(launch.Busy() && launches == 0);
    assert(launch.Begin(owner, "USB:\\", "other.smc", true) == S_FALSE);
    launch.Update(); assert(launches == 0);
    signalled = true; launch.Update();
    assert(!launch.Busy() && launches == 1 && applied.size() == 1);
    assert(launchedFilename == "last-game.smc" && launchedDirectory == "GAME:\\Roms\\");
    assert(applied[0] == "C222-D4DD" && g_NumPendingGameGenieCodes == 0);
    launch.Update(); assert(launches == 1);

    completion = ERROR_CANCELLED; input = L"C222-D4DD";
    launch.Begin(owner, "USB:\\", "cancel.smc", true); launch.Update();
    assert(launches == 2 && applied.empty());
    completion = ERROR_SUCCESS; input = L"BAD";
    launch.Begin(owner, "USB:\\", "invalid.smc", true); launch.Update();
    assert(launches == 2 && !launch.Busy() && g_NumPendingGameGenieCodes == 0);
    keyboardResult = ERROR_SUCCESS; input = L"8b99-17dd";
    launch.Begin(owner, "USB:\\", "immediate.smc", true);
    assert(launches == 3 && applied[0] == "8B99-17DD");
    launch.Begin(owner, "USB:\\", "disabled.smc", false);
    assert(launches == 4 && applied.empty());
    keyboardResult = 5;
    launch.Begin(owner, "USB:\\", "keyboard-error.smc", true);
    assert(launches == 4 && !launch.Busy());
    keyboardResult = ERROR_IO_PENDING; completion = 5;
    launch.Begin(owner, "USB:\\", "async-error.smc", true); launch.Update();
    assert(launches == 4 && !launch.Busy());
    sceneResult = E_FAIL;
    launch.Begin(owner, "USB:\\", "missing-media.smc", false);
    assert(launches == 4 && g_NumPendingGameGenieCodes == 0);
    sceneResult = S_OK; navigateResult = E_FAIL;
    launch.Begin(owner, "USB:\\", "navigation-error.smc", false);
    assert(launches == 4 && !launch.Busy());
    navigateResult = S_OK; eventFailure = true;
    assert(launch.Begin(owner, "USB:\\", "event-error.smc", true) == E_FAIL);
    assert(!launch.Busy()); eventFailure = false;
    assert(launch.Begin(owner, "USB:\\", std::string(260, 'R'), false) == E_FAIL);
    assert(launches == 4);
    {
        CRomLaunch abandoned;
        abandoned.Begin(owner, "USB:\\", "abandoned.smc", true);
    }
    assert(cancellations == 1 && events == closes);
    std::cout << "Frontend component tests passed: parser, long titles, large-list selection, keyboard lifecycle and launch errors.\n";
}
