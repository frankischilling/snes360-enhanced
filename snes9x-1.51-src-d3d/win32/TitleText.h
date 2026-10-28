#ifndef TITLE_TEXT_H
#define TITLE_TEXT_H

#include <xtl.h>
#include <string>

inline std::wstring RomTitleText(const char* title)
{
    if (!title) return std::wstring();
    int length = MultiByteToWideChar(CP_ACP, 0, title, -1, NULL, 0);
    if (length <= 0) return std::wstring();
    std::wstring text(length, L'\0');
    if (!MultiByteToWideChar(CP_ACP, 0, title, -1, &text[0], length))
        return std::wstring();
    text.resize(length - 1);
    return text;
}

#endif
