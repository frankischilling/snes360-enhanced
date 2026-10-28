#ifndef ROM_FILTER_H
#define ROM_FILTER_H

#include <xtl.h>
#include <string>
#include <vector>
#include <cctype>

inline std::vector<std::string> FilterRomNames(const std::vector<std::string>& all,
    const std::wstring& filter)
{
    if (filter.empty()) return all;
    int length = WideCharToMultiByte(CP_ACP, 0, filter.c_str(), -1, NULL, 0, NULL, NULL);
    if (length <= 0) return std::vector<std::string>();
    std::string needle(length, '\0');
    if (!WideCharToMultiByte(CP_ACP, 0, filter.c_str(), -1, &needle[0], length, NULL, NULL))
        return std::vector<std::string>();
    needle.resize(length - 1);
    for (size_t i = 0; i < needle.size(); ++i)
        needle[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(needle[i])));
    std::vector<std::string> result;
    for (size_t i = 0; i < all.size(); ++i)
    {
        std::string name = all[i];
        for (size_t j = 0; j < name.size(); ++j)
            name[j] = static_cast<char>(std::tolower(static_cast<unsigned char>(name[j])));
        if (name.find(needle) != std::string::npos) result.push_back(all[i]);
    }
    return result;
}

#endif
