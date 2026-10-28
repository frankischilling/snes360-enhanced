#include "RomLaunch.h"
#include "GameGenieInput.h"
#include <xmp.h>
#include <cstring>

extern int RunEmulation(char* directory, char* filename);
extern void SaveConfig(void);
extern HXUIOBJ hScene;

char g_PendingGameGenieCodes[3][256] = {{0}};
int g_NumPendingGameGenieCodes = 0;

CRomLaunch::CRomLaunch() : m_owner(NULL), m_active(false), m_pending(false), m_event(NULL)
{
    memset(&m_overlapped, 0, sizeof(m_overlapped));
    memset(m_result, 0, sizeof(m_result));
}

CRomLaunch::~CRomLaunch()
{
    if (m_pending)
    {
        XCancelOverlapped(&m_overlapped);
        XGetOverlappedResult(&m_overlapped, NULL, TRUE);
    }
    CloseKeyboard();
}

void CRomLaunch::CloseKeyboard()
{
    m_pending = false;
    if (m_event) CloseHandle(m_event);
    m_event = NULL;
}

void CRomLaunch::ShowError(LPCWSTR message)
{
    LPCWSTR button = L"OK";
    m_owner->ShowMessageBoxEx(NULL, NULL, L"Launch ROM", message, 1, &button,
        0, XUI_MB_CENTER_ON_PARENT, NULL);
    m_active = false;
}

HRESULT CRomLaunch::Begin(CXuiScene& owner, const std::string& directory,
    const std::string& filename, bool prompt)
{
    if (m_active) return S_FALSE;
    m_owner = &owner;
    m_directory = directory;
    m_filename = filename;
    g_NumPendingGameGenieCodes = 0;
    memset(g_PendingGameGenieCodes, 0, sizeof(g_PendingGameGenieCodes));
    memset(m_result, 0, sizeof(m_result));
    if (filename.empty() || directory.size() + filename.size() >= MAX_PATH)
    {
        ShowError(L"The selected ROM path is empty or too long.");
        return E_FAIL;
    }
    m_active = true;
    if (!prompt)
    {
        Finish(ERROR_SUCCESS);
        return S_OK;
    }
    m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!m_event)
    {
        ShowError(L"Could not open the Game Genie keyboard. Please try again.");
        return E_FAIL;
    }
    memset(&m_overlapped, 0, sizeof(m_overlapped));
    m_overlapped.hEvent = m_event;
    DWORD result = XShowKeyboardUI(0, 0, L"", L"Game Genie codes",
        L"Enter up to 3 codes separated by commas or spaces. B skips codes.",
        m_result, sizeof(m_result) / sizeof(m_result[0]), &m_overlapped);
    if (result == ERROR_IO_PENDING) m_pending = true;
    else Finish(result);
    return S_OK;
}

void CRomLaunch::Update()
{
    if (!m_pending || WaitForSingleObject(m_event, 0) != WAIT_OBJECT_0) return;
    DWORD result = XGetOverlappedResult(&m_overlapped, NULL, FALSE);
    if (result != ERROR_IO_INCOMPLETE) Finish(result);
}

void CRomLaunch::Finish(DWORD result)
{
    CloseKeyboard();
    if (result == ERROR_CANCELLED || result == ERROR_OPERATION_ABORTED)
        m_result[0] = L'\0';
    else if (result != ERROR_SUCCESS)
    {
        ShowError(L"The Game Genie keyboard failed. Please try again.");
        return;
    }
    m_result[255] = L'\0';
    std::vector<std::string> codes;
    if (!ParseGameGenieInput(m_result, codes))
    {
        ShowError(L"Enter up to 3 Game Genie codes in XXXX-XXXX format, separated by commas or spaces.");
        return;
    }
    for (size_t i = 0; i < codes.size(); ++i)
        memcpy(g_PendingGameGenieCodes[i], codes[i].c_str(), codes[i].size() + 1);
    g_NumPendingGameGenieCodes = static_cast<int>(codes.size());

    HRESULT hr = XuiSceneCreate(L"file://game:/media/Snes360.xzp#..\\Xbox\\Skin\\",
        L"InGameOptions.xur", NULL, &hScene);
    if (FAILED(hr))
    {
        g_NumPendingGameGenieCodes = 0;
        ShowError(L"Could not load the in-game menu. Check the media folder.");
        return;
    }
    hr = m_owner->NavigateForward(hScene);
    if (SUCCEEDED(hr))
    {
        XMPPause(NULL);
        RunEmulation(const_cast<char*>(m_directory.c_str()), const_cast<char*>(m_filename.c_str()));
        SaveConfig();
        XMPContinue(NULL);
    }
    XuiDestroyObject(hScene);
    hScene = NULL;
    // A failed ROM load must not leave codes for the next launch.
    g_NumPendingGameGenieCodes = 0;
    memset(g_PendingGameGenieCodes, 0, sizeof(g_PendingGameGenieCodes));
    m_active = false;
}
