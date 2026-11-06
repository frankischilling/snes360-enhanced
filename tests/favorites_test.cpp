#include <xtl.h>
#include "RomSettings.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <fcntl.h>

static std::string stored;
static bool exists = false, writeFailure = false, deleteFailure = false;
static DWORD lastError = ERROR_SUCCESS;
static bool replaceFailure = false, controllerWriteFailure = false;
HANDLE CreateFile(const char* path, DWORD access, DWORD, void*, DWORD creation, DWORD, HANDLE)
{
    if (std::strcmp(path, "GAME:\\favorites.txt"))
    {
        assert(creation == CREATE_NEW);
        int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
        return fd < 0 ? INVALID_HANDLE_VALUE : static_cast<HANDLE>(fdopen(fd, "wb"));
    }
    if ((access == GENERIC_WRITE && writeFailure) || (access == GENERIC_READ && !exists))
        return INVALID_HANDLE_VALUE;
    if (access == GENERIC_WRITE) { stored.clear(); exists = true; }
    return reinterpret_cast<HANDLE>(1);
}
DWORD GetFileSize(HANDLE, DWORD*) { return static_cast<DWORD>(stored.size()); }
BOOL ReadFile(HANDLE, void* data, DWORD size, DWORD* read, void*)
{ std::memcpy(data, stored.data(), size); *read = size; return TRUE; }
BOOL WriteFile(HANDLE file, const void* data, DWORD size, DWORD* written, void*)
{
    if (file != reinterpret_cast<HANDLE>(1))
    {
        *written = std::fwrite(data, 1, controllerWriteFailure ? size / 2 : size, static_cast<FILE*>(file));
        return !controllerWriteFailure;
    }
    stored.append(static_cast<const char*>(data), size); *written = size; return TRUE;
}
BOOL DeleteFile(const char* path)
{
    if (std::strcmp(path, "GAME:\\favorites.txt")) return std::remove(path) == 0;
    if (deleteFailure) { lastError = 5; return FALSE; }
    if (!exists) { lastError = ERROR_FILE_NOT_FOUND; return FALSE; }
    exists = false; stored.clear(); return TRUE;
}
DWORD GetLastError() { return lastError; }
BOOL CloseHandle(HANDLE file)
{ return file == reinterpret_cast<HANDLE>(1) || std::fclose(static_cast<FILE*>(file)) == 0; }
BOOL FlushFileBuffers(HANDLE file) { return std::fflush(static_cast<FILE*>(file)) == 0; }
BOOL MoveFileEx(const char* from, const char* to, DWORD)
{ return !replaceFailure && std::rename(from, to) == 0; }

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
    // Controller choices share settings.xml and must preserve unrelated entries.
    const char *xmlPath = "controller-settings-test.xml";
    {
        std::ofstream xml(xmlPath);
        xml << "<Settings><MappedDrive DriveName=\"USB\" Path=\"USB0:\\ROMS\\\"/>"
               "<PreviewPath>preview</PreviewPath><GameGenieEnabled>true</GameGenieEnabled>"
               "<CustomSetting value=\"keep\"/><ControllerMode/><ControllerMode Path=\"bad\" Multitap=\"invalid\"/>"
               "</Settings>";
    }
    CRomPathSettings modes;
    assert(modes.Load(xmlPath));
    assert(modes.GetMultitapMode("bad") == -1);
    assert(modes.GetMultitapMode("USB0:\\ROMS\\Mana.smc") == -1);
    assert(!modes.SetMultitapMode("", true));
    assert(modes.SetMultitapMode("USB0:\\ROMS\\Mana.smc", true));
    assert(modes.SetMultitapMode("USB0:\\ROMS\\A & B.smc", true));
    CRomPathSettings saved;
    assert(saved.Load(xmlPath));
    assert(saved.GetMultitapMode("usb0:\\roms\\MANA.SMC") == 1);
    assert(saved.GetMultitapMode("USB0:\\ROMS\\A & B.smc") == 1);
    assert(saved.GetMultitapMode("HDD:\\ROMS\\Mana.smc") == -1);
    assert(saved.SetMultitapMode("usb0:\\roms\\MANA.SMC", false));
    assert(modes.Load(xmlPath) && modes.GetMultitapMode("USB0:\\ROMS\\Mana.smc") == 0);
    TiXmlDocument document(xmlPath);
    assert(document.LoadFile());
    TiXmlElement *root = document.FirstChildElement("Settings");
    assert(root && root->FirstChildElement("CustomSetting"));
    assert(modes.IsGameGenieEnabled());
    assert(modes.FindDevice("USB") != modes.GetDeviceMapEnd());
    replaceFailure = true;
    assert(!modes.SetMultitapMode("USB0:\\ROMS\\Mana.smc", true));
    assert(modes.GetMultitapMode("USB0:\\ROMS\\Mana.smc") == 0);
    replaceFailure = false;
    controllerWriteFailure = true;
    assert(!modes.SetMultitapMode("USB0:\\ROMS\\Mana.smc", true));
    controllerWriteFailure = false;
    // A pre-existing temporary file belongs to someone else and stays intact.
    std::string temporary = std::string(xmlPath) + ".controllers.tmp";
    { std::ofstream temp(temporary.c_str()); temp << "keep"; }
    assert(!modes.SetMultitapMode("USB0:\\ROMS\\Mana.smc", true));
    { std::ifstream temp(temporary.c_str()); std::string value; temp >> value; assert(value == "keep"); }
    assert(std::remove(temporary.c_str()) == 0);
    assert(saved.Load(xmlPath) && saved.GetMultitapMode("USB0:\\ROMS\\Mana.smc") == 0);
    assert(std::remove(xmlPath) == 0);
    std::cout << "Controller settings tests passed: per-game choices, reload, casing, XML escaping, preserved settings and save failure rollback.\n";
    std::cout << "Favorites persistence tests passed: remove, reload, casing, last item, duplicates and storage errors.\n";
}
