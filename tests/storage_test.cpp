#include "StorageStream.h"
#include <cassert>
#include <iostream>
#include <string>

static DWORD mountResult = ERROR_IO_PENDING, completion = ERROR_SUCCESS;
static DWORD extended = ERROR_SUCCESS, closeResult = ERROR_IO_PENDING;
static bool fileFailure = false, readFailure = false, writeFailure = false;
static bool eventFailure = false, flushFailure = false;
static int mounts = 0, unmounts = 0, handleCloses = 0;
static int eventCreates = 0, eventCloses = 0;
static DWORD expectedAccess = GENERIC_READ, expectedCreation = OPEN_EXISTING;
static DWORD expectedMount = XCONTENTFLAG_OPENEXISTING, transferred = 4;
static HANDLE event = reinterpret_cast<HANDLE>(1), stream = reinterpret_cast<HANDLE>(2);

DWORD XContentCreate(DWORD user, const char* root, const XCONTENT_DATA* content,
    DWORD flags, DWORD*, void*, XOVERLAPPED* overlapped)
{
    assert(user == 0 && !std::strcmp(root, "save"));
    assert(content->DeviceID == 5 && content->dwContentType == XCONTENTTYPE_SAVEDGAME);
    assert(!std::strcmp(content->szFileName, "test.dat"));
    assert(!std::wcscmp(content->szDisplayName, L"Test save"));
    assert(flags == expectedMount && overlapped->hEvent == event);
    ++mounts;
    return mountResult;
}
DWORD XContentClose(const char* root, XOVERLAPPED* overlapped)
{ assert(!std::strcmp(root, "save") && overlapped->hEvent == event); ++unmounts; return closeResult; }
DWORD XGetOverlappedResult(XOVERLAPPED* overlapped, DWORD*, BOOL wait)
{ assert(overlapped->hEvent == event && wait); return completion; }
DWORD XGetOverlappedExtendedError(XOVERLAPPED*) { return extended; }
HANDLE CreateFile(const char* path, DWORD access, DWORD, void*, DWORD creation, DWORD, HANDLE)
{
    assert(!std::strcmp(path, "save:\\savegame.txt"));
    assert(access == expectedAccess && creation == expectedCreation);
    return fileFailure ? INVALID_HANDLE_VALUE : stream;
}
HANDLE CreateEvent(void*, BOOL manualReset, BOOL, const char*)
{ assert(manualReset); if (eventFailure) return NULL; ++eventCreates; return event; }
BOOL CloseHandle(HANDLE file)
{ if (file == event) ++eventCloses; else { assert(file == stream); ++handleCloses; } return TRUE; }
BOOL FlushFileBuffers(HANDLE file) { assert(file == stream); return !flushFailure; }
BOOL ReadFile(HANDLE file, void*, DWORD, DWORD* read, void*)
{ assert(file == stream); *read = transferred; return !readFailure; }
BOOL WriteFile(HANDLE file, const void*, DWORD, DWORD* written, void*)
{ assert(file == stream); *written = transferred; return !writeFailure; }

static HANDLE Open(bool read, XOVERLAPPED* overlapped)
{
    expectedAccess = read ? GENERIC_READ : GENERIC_WRITE;
    expectedCreation = read ? OPEN_EXISTING : CREATE_ALWAYS;
    expectedMount = read ? XCONTENTFLAG_OPENEXISTING : XCONTENTFLAG_CREATEALWAYS;
    return OpenContentStream(0, 5, "test.dat", L"Test save", "save", "save:\\savegame.txt", read, overlapped);
}

int main()
{
    XOVERLAPPED overlapped = {0}; overlapped.hEvent = event;
    for (int read = 0; read <= 1; ++read)
    {
        assert(Open(read != 0, &overlapped) == stream);
        assert(handleCloses == read); // Opening never closes the caller's event.
        CloseHandle(stream);
        assert(CloseContentStreamMount("save", &overlapped));
    }
    mountResult = ERROR_SUCCESS;
    assert(Open(true, &overlapped) == stream);
    CloseHandle(stream);
    closeResult = ERROR_SUCCESS;
    assert(CloseContentStreamMount("save", &overlapped));
    mountResult = 5;
    int beforeUnmounts = unmounts;
    assert(Open(true, &overlapped) == NULL && unmounts == beforeUnmounts);
    mountResult = ERROR_IO_PENDING; completion = ERROR_FUNCTION_FAILED;
    assert(Open(true, &overlapped) == NULL && unmounts == beforeUnmounts);
    completion = ERROR_SUCCESS; extended = 5;
    assert(Open(true, &overlapped) == NULL && unmounts == beforeUnmounts);
    extended = ERROR_SUCCESS; fileFailure = true;
    assert(Open(true, &overlapped) == NULL && unmounts == beforeUnmounts + 1);
    assert(Open(false, &overlapped) == NULL && unmounts == beforeUnmounts + 2);
    assert(handleCloses == 3);
    assert(Open(true, NULL) == NULL);
    overlapped.hEvent = NULL;
    assert(Open(true, &overlapped) == NULL);
    overlapped.hEvent = event;
    int beforeMounts = mounts;
    assert(OpenContentStream(0, 5, std::string(1000, 'R').c_str(), L"Test save",
        "save", "save:\\savegame.txt", true, &overlapped) == NULL);
    assert(mounts == beforeMounts);
    char data[8] = {0};
    assert(ReadStorageStream(stream, data, sizeof(data)) == transferred);
    assert(WriteStorageStream(stream, data, sizeof(data)) == transferred);
    readFailure = writeFailure = true;
    assert(ReadStorageStream(stream, data, sizeof(data)) == 0);
    assert(WriteStorageStream(stream, data, sizeof(data)) == 0);
    readFailure = writeFailure = fileFailure = false;
    XCONTENT_DATA content = {0};
    content.DeviceID = 5;
    std::strcpy(content.szFileName, "test.dat");
    std::wcscpy(content.szDisplayName, L"Test save");
    DWORD bytes = 99;
    for (int read = 0; read <= 1; ++read)
    {
        expectedAccess = read ? GENERIC_READ : GENERIC_WRITE;
        expectedCreation = read ? OPEN_EXISTING : CREATE_ALWAYS;
        expectedMount = read ? XCONTENTFLAG_OPENEXISTING : XCONTENTFLAG_CREATEALWAYS;
        transferred = sizeof(data);
        assert(TransferSaveContent(0, &content, "save", "save:\\savegame.txt",
            data, sizeof(data), read != 0, &bytes) == ERROR_SUCCESS);
        assert(bytes == sizeof(data));
        readFailure = writeFailure = true;
        assert(TransferSaveContent(0, &content, "save", "save:\\savegame.txt",
            data, sizeof(data), read != 0, &bytes) != ERROR_SUCCESS && bytes == 0);
        readFailure = writeFailure = false;
        fileFailure = true;
        assert(TransferSaveContent(0, &content, "save", "save:\\savegame.txt",
            data, sizeof(data), read != 0, &bytes) != ERROR_SUCCESS && bytes == 0);
        fileFailure = false;
    }
    expectedAccess = GENERIC_WRITE; expectedCreation = CREATE_ALWAYS;
    expectedMount = XCONTENTFLAG_CREATEALWAYS;
    transferred = sizeof(data) / 2;
    assert(TransferSaveContent(0, &content, "save", "save:\\savegame.txt",
        data, sizeof(data), false, &bytes) != ERROR_SUCCESS && bytes == 0);
    transferred = sizeof(data); flushFailure = true;
    assert(TransferSaveContent(0, &content, "save", "save:\\savegame.txt",
        data, sizeof(data), false, &bytes) != ERROR_SUCCESS && bytes == 0);
    flushFailure = false; eventFailure = true;
    assert(TransferSaveContent(0, &content, "save", "save:\\savegame.txt",
        data, sizeof(data), false, &bytes) != ERROR_SUCCESS && bytes == 0);
    assert(TransferSaveContent(0, &content, "save", "save:\\savegame.txt",
        data, sizeof(data), true, NULL) == ERROR_INVALID_PARAMETER);
    assert(eventCreates == eventCloses);
    std::cout << "Storage stream tests passed: mount errors, invalid handles, cleanup, event ownership and byte counts.\n";
}
