#ifndef GAME_GENIE_INPUT_H
#define GAME_GENIE_INPUT_H

#include <string>
#include <vector>

// Parse the keyboard result without changing the ROM list or launching a game.
inline bool ParseGameGenieInput(const std::wstring& input,
    std::vector<std::string>& codes)
{
    codes.clear();
    std::vector<std::string> parsed;
    std::string token;
    for (size_t i = 0; i <= input.size(); ++i)
    {
        wchar_t ch = i < input.size() ? input[i] : L'\0';
        if (ch == L'\0' || ch == L',' || ch == L' ' || ch == L'\t' || ch == L'\r' || ch == L'\n')
        {
            if (!token.empty())
            {
                if (token.size() != 9 || token[4] != '-' || parsed.size() == 3)
                    return false;
                parsed.push_back(token);
                token.clear();
            }
        }
        else
        {
            if (ch >= L'a' && ch <= L'f') ch -= L'a' - L'A';
            if (!((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'F') ||
                  (ch == L'-' && token.size() == 4)) || token.size() >= 9)
                return false;
            token += static_cast<char>(ch);
        }
    }
    codes.swap(parsed);
    return true;
}

#endif
