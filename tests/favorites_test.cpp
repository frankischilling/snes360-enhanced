#include <xtl.h>
#include "RomSettings.h"
#include <cassert>
#include <iostream>

static std::string stored;
static bool exists = false, writeFailure = false, deleteFailure = false;
static DWORD lastError = ERROR_SUCCESS;
HANDLE CreateFile(const char*, DWORD access, DWORD, void*, DWORD, DWORD, HANDLE)
{
    if ((access == GENERIC_WRITE && writeFailure) || (access == GENERIC_READ && !exists))
        return INVALID_HANDLE_VALUE;
    if (access == GENERIC_WRITE) { stored.clear(); exists = true; }
    return reinterpret_cast<HANDLE>(1);
}
DWORD GetFileSize(HANDLE, DWORD*) { return static_cast<DWORD>(stored.size()); }
BOOL ReadFile(HANDLE, void* data, DWORD size, DWORD* read, void*)
{ std::memcpy(data, stored.data(), size); *read = size; return TRUE; }
BOOL WriteFile(HANDLE, const void* data, DWORD size, DWORD* written, void*)
{ stored.append(static_cast<const char*>(data), size); *written = size; return TRUE; }
BOOL DeleteFile(const char*)
{
    if (deleteFailure) { lastError = 5; return FALSE; }
    if (!exists) { lastError = ERROR_FILE_NOT_FOUND; return FALSE; }
    exists = false; stored.clear(); return TRUE;
}
DWORD GetLastError() { return lastError; }
BOOL CloseHandle(HANDLE) { return TRUE; }

int main()
{
    CRomPathSettings settings;
    assert(!settings.RemoveFavorite(""));
    assert(settings.AddFavorite("Super Mario World.smc"));
    assert(settings.AddFavorite("SUPER MARIO WORLD.SMC"));
    assert(settings.GetFavorites().size() == 1);
    assert(settings.AddFavorite("Metroid.smc"));
    assert(settings.RemoveFavorite("super mario world.smc"));
    CRomPathSettings reloaded;
    assert(reloaded.LoadFavorites());
    assert(!reloaded.IsFavorite("Super Mario World.smc") && reloaded.IsFavorite("METROID.SMC"));
    writeFailure = true;
    assert(!reloaded.AddFavorite("Zelda.smc") && !reloaded.IsFavorite("Zelda.smc"));
    writeFailure = false; deleteFailure = true;
    assert(!reloaded.RemoveFavorite("metroid.smc") && reloaded.IsFavorite("metroid.smc"));
    deleteFailure = false;
    assert(reloaded.RemoveFavorite("metroid.smc") && reloaded.GetFavorites().empty() && !exists);
    assert(reloaded.LoadFavorites() && reloaded.GetFavorites().empty());
    assert(reloaded.RemoveFavorite("missing.smc"));
    stored = "Mixed.smc\r\nmixed.SMC\r\nOther.smc\r\n"; exists = true;
    reloaded.LoadFavorites();
    assert(reloaded.RemoveFavorite("MIXED.SMC"));
    reloaded.LoadFavorites();
    assert(reloaded.GetFavorites().size() == 1 && reloaded.IsFavorite("Other.smc"));
    std::cout << "Favorites persistence tests passed: remove, reload, casing, last item, duplicates and storage errors.\n";
}
