#ifndef TEST_XTL_H
#define TEST_XTL_H
#include <cstddef>
#include <cstring>
#include <cwchar>
#include <string>
#ifndef _WIN32
#include <strings.h>
#define _stricmp strcasecmp
#endif
typedef unsigned long DWORD;
typedef int BOOL;
typedef int HRESULT;
typedef void* HANDLE;
typedef wchar_t WCHAR;
typedef const WCHAR* LPCWSTR;
struct XOVERLAPPED { HANDLE hEvent; };
#define TRUE 1
#define FALSE 0
#define S_OK 0
#define S_FALSE 1
#define E_FAIL (-1)
#define FAILED(x) ((x) < 0)
#define SUCCEEDED(x) ((x) >= 0)
#define ERROR_SUCCESS 0
#define ERROR_IO_PENDING 997
#define ERROR_IO_INCOMPLETE 996
#define ERROR_CANCELLED 1223
#define ERROR_OPERATION_ABORTED 995
#define WAIT_OBJECT_0 0
#define MAX_PATH 260
#define CP_ACP 0
#define INVALID_HANDLE_VALUE reinterpret_cast<HANDLE>(-1)
#define GENERIC_READ 1
#define GENERIC_WRITE 2
#define OPEN_EXISTING 3
#define CREATE_ALWAYS 4
#define FILE_ATTRIBUTE_NORMAL 0
#define ERROR_FILE_NOT_FOUND 2
HANDLE CreateFile(const char*, DWORD, DWORD, void*, DWORD, DWORD, HANDLE);
DWORD GetFileSize(HANDLE, DWORD*);
BOOL ReadFile(HANDLE, void*, DWORD, DWORD*, void*);
BOOL WriteFile(HANDLE, const void*, DWORD, DWORD*, void*);
BOOL DeleteFile(const char*);
DWORD GetLastError();
HANDLE CreateEvent(void*, BOOL, BOOL, const char*);
BOOL CloseHandle(HANDLE);
DWORD XCancelOverlapped(XOVERLAPPED*);
DWORD XGetOverlappedResult(XOVERLAPPED*, DWORD*, BOOL);
DWORD WaitForSingleObject(HANDLE, DWORD);
DWORD XShowKeyboardUI(DWORD, DWORD, LPCWSTR, LPCWSTR, LPCWSTR, WCHAR*, DWORD, XOVERLAPPED*);
inline int WideCharToMultiByte(DWORD, DWORD, const WCHAR* source, int, char* dest, int capacity, void*, void*)
{
    int length = static_cast<int>(std::wcslen(source)) + 1;
    if (!dest) return length;
    if (capacity < length) return 0;
    for (int i = 0; i < length; ++i) dest[i] = static_cast<char>(source[i]);
    return length;
}
inline int MultiByteToWideChar(DWORD, DWORD, const char* source, int, WCHAR* dest, int capacity)
{
    int length = static_cast<int>(std::strlen(source)) + 1;
    if (!dest) return length;
    if (capacity < length) return 0;
    for (int i = 0; i < length; ++i) dest[i] = static_cast<unsigned char>(source[i]);
    return length;
}
#endif
