#ifndef ROM_LAUNCH_H
#define ROM_LAUNCH_H

#include <xtl.h>
#include <xuiapp.h>
#include <string>

class CRomLaunch
{
public:
    CRomLaunch();
    ~CRomLaunch();
    HRESULT Begin(CXuiScene& owner, const std::string& directory,
        const std::string& filename, bool prompt);
    void Update();
    bool Busy() const { return m_active; }

private:
    CRomLaunch(const CRomLaunch&);
    CRomLaunch& operator=(const CRomLaunch&);
    void Finish(DWORD result);
    void CloseKeyboard();
    void ShowError(LPCWSTR message);
    CXuiScene* m_owner;
    std::string m_directory;
    std::string m_filename;
    bool m_active;
    bool m_pending;
    HANDLE m_event;
    XOVERLAPPED m_overlapped;
    WCHAR m_result[256];
};

#endif
