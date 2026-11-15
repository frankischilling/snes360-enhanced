#include <xtl.h>
#include "RomSettings.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <sstream>

static const char* favoritesPath = "GAME:\\favorites.txt";
static bool writeFailure = false, deleteFailure = false, readFailure = false;
static DWORD lastError = ERROR_SUCCESS;
static bool replaceFailure = false, controllerWriteFailure = false;
static bool shortWrite = false, shortRead = false, flushFailure = false, closeFailure = false;
static bool sizeOverride = false;
static DWORD reportedSize = 0, reportedHighSize = 0;

static void WriteText(const char* path, const std::string& value)
{ std::ofstream file(path, std::ios::binary); file << value; assert(file.good()); }
static std::string ReadText(const char* path)
{ std::ifstream file(path, std::ios::binary); std::ostringstream value; value << file.rdbuf(); return value.str(); }

HANDLE CreateFile(const char* path, DWORD access, DWORD, void*, DWORD creation, DWORD, HANDLE)
{
    if (access == GENERIC_WRITE && writeFailure)
        return INVALID_HANDLE_VALUE;
    int flags = access == GENERIC_READ ? O_RDONLY : O_WRONLY | O_CREAT;
    if (creation == CREATE_NEW) flags |= O_EXCL;
    if (creation == CREATE_ALWAYS) flags |= O_TRUNC;
    int fd = open(path, flags, 0600);
    if (fd < 0) { lastError = errno == ENOENT ? ERROR_FILE_NOT_FOUND : 5; return INVALID_HANDLE_VALUE; }
    return static_cast<HANDLE>(fdopen(fd, access == GENERIC_READ ? "rb" : "wb"));
}
DWORD GetFileSize(HANDLE file, DWORD* high)
{
    if (high) *high = sizeOverride ? reportedHighSize : 0;
    if (sizeOverride) return reportedSize;
    FILE* stream = static_cast<FILE*>(file);
    long position = std::ftell(stream);
    std::fseek(stream, 0, SEEK_END);
    long size = std::ftell(stream);
    std::fseek(stream, position, SEEK_SET);
    return static_cast<DWORD>(size);
}
BOOL ReadFile(HANDLE file, void* data, DWORD size, DWORD* read, void*)
{
    *read = std::fread(data, 1, shortRead ? size / 2 : size, static_cast<FILE*>(file));
    return !readFailure;
}
BOOL WriteFile(HANDLE file, const void* data, DWORD size, DWORD* written, void*)
{
    *written = std::fwrite(data, 1, controllerWriteFailure || shortWrite ? size / 2 : size,
        static_cast<FILE*>(file));
    return !controllerWriteFailure;
}
BOOL DeleteFile(const char* path)
{
    if (!std::strcmp(path, favoritesPath) && deleteFailure) { lastError = 5; return FALSE; }
    if (std::remove(path) == 0) return TRUE;
    lastError = errno == ENOENT ? ERROR_FILE_NOT_FOUND : 5;
    return FALSE;
}
DWORD GetLastError() { return lastError; }
BOOL CloseHandle(HANDLE file)
{ bool closed = std::fclose(static_cast<FILE*>(file)) == 0; return closed && !closeFailure; }
BOOL FlushFileBuffers(HANDLE file) { return std::fflush(static_cast<FILE*>(file)) == 0 && !flushFailure; }
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
    assert(reloaded.RemoveFavorite("metroid.smc") && reloaded.GetFavorites().empty());
    assert(access(favoritesPath, F_OK) != 0);
    assert(reloaded.LoadFavorites() && reloaded.GetFavorites().empty());
    assert(reloaded.RemoveFavorite("missing.smc"));
    WriteText(favoritesPath, "Mixed.smc\r\nmixed.SMC\r\nOther.smc\r\n");
    reloaded.LoadFavorites();
    assert(reloaded.RemoveFavorite("MIXED.SMC"));
    reloaded.LoadFavorites();
    assert(reloaded.GetFavorites().size() == 1 && reloaded.IsFavorite("Other.smc"));
    // Storage errors must leave the saved list and the active list intact.
    const std::string originalFavorites = ReadText(favoritesPath);
    bool* saveFailures[] = {&writeFailure, &controllerWriteFailure, &shortWrite,
        &flushFailure, &closeFailure, &replaceFailure};
    for (size_t i = 0; i < sizeof(saveFailures) / sizeof(saveFailures[0]); ++i)
    {
        *saveFailures[i] = true;
        assert(!reloaded.AddFavorite("New.smc"));
        *saveFailures[i] = false;
        assert(!reloaded.IsFavorite("New.smc") && ReadText(favoritesPath) == originalFavorites);
        assert(access("GAME:\\favorites.txt.tmp", F_OK) != 0);
    }
    WriteText("GAME:\\favorites.txt.tmp", "another writer");
    assert(!reloaded.AddFavorite("New.smc"));
    assert(ReadText("GAME:\\favorites.txt.tmp") == "another writer");
    assert(std::remove("GAME:\\favorites.txt.tmp") == 0);
    assert(!reloaded.AddFavorite(std::string(MAX_PATH, 'R')));
    assert(!reloaded.AddFavorite("Injected.smc\nOther.smc"));
    assert(ReadText(favoritesPath) == originalFavorites);
    readFailure = true;
    assert(!reloaded.LoadFavorites() && reloaded.IsFavorite("Other.smc"));
    readFailure = false; shortRead = true;
    assert(!reloaded.LoadFavorites() && reloaded.IsFavorite("Other.smc"));
    shortRead = false; sizeOverride = true;
    reportedSize = INVALID_FILE_SIZE;
    assert(!reloaded.LoadFavorites() && reloaded.IsFavorite("Other.smc"));
    reportedSize = 4 * 1024 * 1024;
    assert(!reloaded.LoadFavorites());
    reportedSize = 10; reportedHighSize = 1;
    assert(!reloaded.LoadFavorites());
    sizeOverride = false;
    WriteText(favoritesPath, std::string(10000, 'R') + "\nValid.smc\n");
    assert(reloaded.LoadFavorites() && reloaded.GetFavorites().size() == 1);
    assert(reloaded.IsFavorite("Valid.smc"));
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

    // Missing XML attributes and empty values must not crash the ROM browser.
    WriteText(xmlPath, "<Settings><MappedDrive/><MappedDrive DriveName=\"missing-path\"/>"
        "<MappedDrive Path=\"USB0:\\Roms\\\"/><MappedDrive DriveName=\"\" Path=\"\"/>"
        "<PreviewPath/></Settings>");
    assert(modes.Load(xmlPath));
    assert(modes.GetDeviceMapBegin() != modes.GetDeviceMapEnd());
    assert(modes.FindDevice("USB") == modes.GetDeviceMapEnd());
    assert(!modes.m_PreviewPath.empty() && !modes.IsGameGenieEnabled());
    WriteText(xmlPath, "<Settings><MappedDrive DriveName=\"NEW\" Path=\"USB0:\\Roms\"/></Settings>");
    assert(modes.Load(xmlPath));
    assert(modes.FindDevice("NEW")->second == "USB0:\\Roms\\");
    assert(modes.FindDevice("USB") == modes.GetDeviceMapEnd());
    WriteText(xmlPath, std::string("<Settings><MappedDrive DriveName=\"") + std::string(1000, 'D') +
        "\" Path=\"USB0:\\Roms\\\"/><MappedDrive DriveName=\"LONG\" Path=\"" +
        std::string(1000, 'P') + "\"/></Settings>");
    assert(modes.Load(xmlPath));
    assert(modes.FindDevice("LONG") == modes.GetDeviceMapEnd());
    assert(modes.GetDeviceMapBegin() != modes.GetDeviceMapEnd());
    WriteText(xmlPath, "<Settings>");
    assert(!modes.Load(xmlPath));
    assert(ReadText(xmlPath) == "<Settings>");
    assert(modes.GetDeviceMapBegin() != modes.GetDeviceMapEnd());
    assert(std::remove(xmlPath) == 0);
    assert(!modes.Load(xmlPath) && modes.GetDeviceMapBegin() != modes.GetDeviceMapEnd());
    assert(std::remove(favoritesPath) == 0);
    std::cout << "Controller settings tests passed: per-game choices, reload, casing, XML escaping, preserved settings and save failure rollback.\n";
    std::cout << "Favorites persistence tests passed: atomic writes, failure rollback, bounded reads, long entries and malformed settings.\n";
}
