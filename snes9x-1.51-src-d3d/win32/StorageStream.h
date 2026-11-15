#ifndef STORAGE_STREAM_H
#define STORAGE_STREAM_H

#include <xtl.h>
#include <cstring>
#include <cwchar>

inline bool CompleteContentRequest(DWORD result, XOVERLAPPED* overlapped)
{
    if (result == ERROR_IO_PENDING)
    {
        result = XGetOverlappedResult(overlapped, NULL, TRUE);
        if (result == ERROR_SUCCESS) result = XGetOverlappedExtendedError(overlapped);
    }
    return result == ERROR_SUCCESS;
}

inline bool CloseContentStreamMount(const char* root, XOVERLAPPED* overlapped)
{
    return CompleteContentRequest(XContentClose(root, overlapped), overlapped);
}

// The caller owns the overlapped structure and its event on every return path.
inline HANDLE OpenContentStream(DWORD user, XCONTENTDEVICEID device,
    const char* filename, const WCHAR* displayName, const char* root,
    const char* streamPath, bool read, XOVERLAPPED* overlapped)
{
    if (!overlapped || !overlapped->hEvent || !filename || !displayName) return NULL;
    XCONTENT_DATA content = {0};
    size_t nameLength = std::strlen(filename), displayLength = std::wcslen(displayName);
    if (nameLength >= sizeof(content.szFileName) ||
        displayLength >= sizeof(content.szDisplayName) / sizeof(WCHAR)) return NULL;
    std::memcpy(content.szFileName, filename, nameLength + 1);
    std::memcpy(content.szDisplayName, displayName, (displayLength + 1) * sizeof(WCHAR));
    content.dwContentType = XCONTENTTYPE_SAVEDGAME;
    content.DeviceID = device;
    DWORD result = XContentCreate(user, root, &content,
        read ? XCONTENTFLAG_OPENEXISTING : XCONTENTFLAG_CREATEALWAYS, NULL, NULL, overlapped);
    if (!CompleteContentRequest(result, overlapped)) return NULL;
    HANDLE file = CreateFile(streamPath, read ? GENERIC_READ : GENERIC_WRITE,
        read ? FILE_SHARE_READ : 0, NULL, read ? OPEN_EXISTING : CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        CloseContentStreamMount(root, overlapped);
        return NULL;
    }
    return file;
}

inline DWORD ReadStorageStream(HANDLE file, char* buffer, DWORD size)
{
    DWORD read = 0;
    return ReadFile(file, buffer, size, &read, NULL) ? read : 0;
}

inline DWORD WriteStorageStream(HANDLE file, const char* buffer, DWORD size)
{
    DWORD written = 0;
    return WriteFile(file, buffer, size, &written, NULL) ? written : 0;
}

inline DWORD TransferSaveContent(DWORD user, const XCONTENT_DATA* content,
    const char* root, const char* streamPath, char* buffer, DWORD size,
    bool read, DWORD* transferred)
{
    if (!content || !buffer || !transferred) return ERROR_INVALID_PARAMETER;
    *transferred = 0;
    XOVERLAPPED overlapped = {0};
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!overlapped.hEvent) return ERROR_FUNCTION_FAILED;
    HANDLE file = OpenContentStream(user, content->DeviceID, content->szFileName,
        content->szDisplayName, root, streamPath, read, &overlapped);
    bool success = false;
    if (file)
    {
        success = (read ? ReadFile(file, buffer, size, transferred, NULL) :
            WriteFile(file, buffer, size, transferred, NULL)) != FALSE;
        if (!read) success = success && *transferred == size && FlushFileBuffers(file) != FALSE;
        if (!CloseHandle(file)) success = false;
        if (!CloseContentStreamMount(root, &overlapped)) success = false;
    }
    if (!CloseHandle(overlapped.hEvent)) success = false;
    if (!success) *transferred = 0;
    return success ? ERROR_SUCCESS : ERROR_FUNCTION_FAILED;
}

#endif
