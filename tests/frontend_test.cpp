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
static DWORD keyboardExtendedError = ERROR_SUCCESS;
static bool signalled = false, eventFailure = false;
static HRESULT sceneResult = S_OK;
static std::wstring input;
static int launches = 0, events = 0, closes = 0, cancellations = 0;
static int emulationResult = 0;
static std::string launchedDirectory, launchedFilename;
static std::vector<std::string> applied;
HANDLE CreateEvent(void*, BOOL manualReset, BOOL, const char*)
{ assert(manualReset); if (eventFailure) return NULL; ++events; return reinterpret_cast<HANDLE>(1); }
BOOL CloseHandle(HANDLE handle) { assert(handle); ++closes; return TRUE; }
DWORD XCancelOverlapped(XOVERLAPPED*) { ++cancellations; return ERROR_SUCCESS; }
DWORD XGetOverlappedResult(XOVERLAPPED*, DWORD*, BOOL) { return completion; }
DWORD XGetOverlappedExtendedError(XOVERLAPPED* overlapped) { return overlapped->dwExtendedError; }
DWORD WaitForSingleObject(HANDLE, DWORD) { return signalled ? WAIT_OBJECT_0 : 258; }
DWORD XShowKeyboardUI(DWORD, DWORD, LPCWSTR, LPCWSTR, LPCWSTR, WCHAR* result, DWORD capacity, XOVERLAPPED* overlapped)
{
    assert(input.size() < capacity);
    std::wcscpy(result, input.c_str());
    overlapped->dwExtendedError = keyboardExtendedError;
    return keyboardResult;
}
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
    return emulationResult;
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
    const DWORD directCancellations[] = {ERROR_CANCELLED, ERROR_OPERATION_ABORTED};
    for (size_t i = 0; i < sizeof(directCancellations) / sizeof(directCancellations[0]); ++i)
    {
        keyboardResult = directCancellations[i]; input = L"BAD";
        int beforeLaunches = launches, beforeMessages = messageCount;
        launch.Begin(owner, "USB:\\", "direct-cancel.smc", true);
        assert(!launch.Busy() && launches == beforeLaunches + 1 && applied.empty());
        assert(messageCount == beforeMessages);
    }
    // The Guide reports B/Guide cancellation in the extended completion status.
    // The general status can be ERROR_FUNCTION_FAILED instead of ERROR_CANCELLED.
    struct KeyboardCase { DWORD status; DWORD extended; bool cancelled; };
    const KeyboardCase keyboardCases[] = {
        {ERROR_FUNCTION_FAILED, ERROR_CANCELLED, true},
        {ERROR_SUCCESS, ERROR_CANCELLED, true},
        {ERROR_CANCELLED, ERROR_SUCCESS, true},
        {ERROR_OPERATION_ABORTED, ERROR_SUCCESS, true},
        {ERROR_FUNCTION_FAILED, ERROR_OPERATION_ABORTED, true},
        {ERROR_FUNCTION_FAILED, 5, false},
        {ERROR_SUCCESS, 5, false},
        {ERROR_FUNCTION_FAILED, ERROR_SUCCESS, false}
    };
    keyboardResult = ERROR_IO_PENDING;
    for (size_t i = 0; i < sizeof(keyboardCases) / sizeof(keyboardCases[0]); ++i)
    {
        completion = keyboardCases[i].status;
        keyboardExtendedError = keyboardCases[i].extended;
        input = L"BAD"; // Cancel must discard even an invalid unfinished entry.
        int beforeLaunches = launches, beforeMessages = messageCount;
        assert(launch.Begin(owner, "USB:\\", "skip.smc", true) == S_OK);
        assert(launch.Busy() && launches == beforeLaunches);
        launch.Update();
        assert(!launch.Busy());
        assert(launches == beforeLaunches + (keyboardCases[i].cancelled ? 1 : 0));
        assert(messageCount == beforeMessages + (keyboardCases[i].cancelled ? 0 : 1));
        assert(g_NumPendingGameGenieCodes == 0 && applied.empty());
        launch.Update();
        assert(launches == beforeLaunches + (keyboardCases[i].cancelled ? 1 : 0));
    }
    keyboardExtendedError = ERROR_SUCCESS;
    // A completion signal is not enough if the request still reports pending.
    completion = ERROR_IO_INCOMPLETE; input = L"C222-D4DD";
    int beforeLaunches = launches;
    launch.Begin(owner, "USB:\\", "still-pending.smc", true);
    launch.Update(); assert(launch.Busy() && launches == beforeLaunches);
    completion = ERROR_SUCCESS;
    launch.Update(); assert(!launch.Busy() && launches == beforeLaunches + 1);
    assert(applied.size() == 1 && applied[0] == "C222-D4DD");
    emulationResult = 1;
    beforeLaunches = launches;
    int beforeMessages = messageCount;
    launch.Begin(owner, "USB:\\", "bad-rom.smc", false);
    assert(launches == beforeLaunches + 1 && messageCount == beforeMessages + 1);
    assert(!launch.Busy() && !hScene && g_NumPendingGameGenieCodes == 0);
    emulationResult = 0;
    {
        CRomLaunch abandoned;
        abandoned.Begin(owner, "USB:\\", "abandoned.smc", true);
    }
    assert(cancellations == 1 && events == closes);
    std::cout << "Frontend component tests passed: parser, long titles, large-list selection, keyboard lifecycle and launch errors.\n";
}
