
#include "RomSettings.h"
#include <vector>
#include <xtl.h>
 

#define GET_SIMPLE_PROP(name) \
	element = root.FirstChildElement(#name).Element();\
	if (element)\
		m_##name = string(element->GetText());

#define DEFINE_GETFUNC(name)\
	string CRomPathSettings::Get##name() const\
	{\
		if (m_bLoaded)\
		return m_##name;\
		return "";\
	}
	

static string ControllerPathKey(string path)
{
    for (size_t i = 0; i < path.size(); ++i)
        if (path[i] >= 'A' && path[i] <= 'Z') path[i] += 'a' - 'A';
    return path;
}

CRomPathSettings::CRomPathSettings() : m_bLoaded(false), m_GameGenieEnabled(false)
{
}

CRomPathSettings::~CRomPathSettings()
{	
}

bool CRomPathSettings::Load(const string & sXmlPath)
{	
	m_sXmlPath = sXmlPath;
	m_Favorites.clear();
	m_MultitapModes.clear();
	
	if (!m_XmlDoc.LoadFile(sXmlPath.c_str()))
	{
		 
		return false;
	}

	TiXmlElement * element = m_XmlDoc.FirstChildElement("Settings");
	if (!element)
	{
		 
		return false;
	}

	TiXmlHandle root = TiXmlHandle(element);
 

	element = root.FirstChildElement("MappedDrive").Element();
	for(element; element; element = element->NextSiblingElement("MappedDrive"))
	{
		string DriveName = element->Attribute("DriveName");
		if (DriveName == "")
		{
			 
			continue;
		}
		string DevicePath = element->Attribute("Path");
		if (DevicePath == "")
		{
			 
			continue;
		}
		m_DeviceMap[DriveName] = DevicePath;
	}


	element = root.FirstChildElement("PreviewPath").Element();
	if (element)
		m_PreviewPath = string(element->GetText());
	
	// Load GameGenieEnabled setting
	element = root.FirstChildElement("GameGenieEnabled").Element();
	if (element && element->GetText())
	{
		const char* text = element->GetText();
		m_GameGenieEnabled = (_stricmp(text, "true") == 0 || _stricmp(text, "1") == 0);
	}
	else
	{
		m_GameGenieEnabled = false;  // Default to false if not specified
	}

	for (element = root.FirstChildElement("ControllerMode").Element(); element;
	     element = element->NextSiblingElement("ControllerMode"))
	{
		const char *path = element->Attribute("Path");
		const char *mode = element->Attribute("Multitap");
		if (path && *path && mode && (!strcmp(mode, "true") || !strcmp(mode, "false")))
			m_MultitapModes[ControllerPathKey(path)] = !strcmp(mode, "true");
	}

	// Load favorites from text file (not from XML)
	LoadFavorites();

	m_bLoaded = true;

	return true;
}

map<string,string>::iterator CRomPathSettings::GetDeviceMapBegin()
{
	return m_DeviceMap.begin();
}

map<string,string>::iterator CRomPathSettings::GetDeviceMapEnd()
{
	return m_DeviceMap.end();
}

map<string,string>::iterator CRomPathSettings::FindDevice(std::string Device)
{
	return m_DeviceMap.find(Device);
}

bool CRomPathSettings::LoadFavorites()
{
	m_Favorites.clear();
	
	// Try to load from text file first
	string favoritesPath = "GAME:\\favorites.txt";
	
	// Try to open and read the file
	HANDLE hFile = CreateFile(favoritesPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		// File doesn't exist, that's okay - return true with empty favorites
		return true;
	}
	
	// Get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == 0)
	{
		CloseHandle(hFile);
		return true;
	}
	
	// Read file content
	char* pBuffer = new char[dwFileSize + 1];
	DWORD dwBytesRead = 0;
	
	if (!ReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, NULL))
	{
		delete[] pBuffer;
		CloseHandle(hFile);
		return false;
	}
	
	CloseHandle(hFile);
	pBuffer[dwBytesRead] = '\0';
	
	// Parse lines
	string content(pBuffer);
	delete[] pBuffer;
	
	size_t pos = 0;
	while (pos < content.length())
	{
		size_t lineEnd = content.find('\n', pos);
		if (lineEnd == string::npos)
			lineEnd = content.length();
		
		string line = content.substr(pos, lineEnd - pos);
		// Remove carriage return if present
		if (!line.empty() && line[line.length() - 1] == '\r')
			line = line.substr(0, line.length() - 1);
		
		// Trim whitespace
		while (!line.empty() && (line[0] == ' ' || line[0] == '\t'))
			line = line.substr(1);
		while (!line.empty() && (line[line.length() - 1] == ' ' || line[line.length() - 1] == '\t'))
			line = line.substr(0, line.length() - 1);
		
		if (!line.empty())
		{
			m_Favorites.insert(line);
		}
		
		pos = lineEnd + 1;
	}
	
	return true;
}

bool CRomPathSettings::SaveFavorites()
{
	string favoritesPath = "GAME:\\favorites.txt";
	
	// If no favorites, delete the file if it exists
	if (m_Favorites.empty())
	{
		if (DeleteFile(favoritesPath.c_str())) return true;
        return GetLastError() == ERROR_FILE_NOT_FOUND;
	}
	
	// Open file for writing (create or truncate)
	HANDLE hFile = CreateFile(favoritesPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	// Write each favorite on a new line
	for (set<string>::iterator it = m_Favorites.begin(); it != m_Favorites.end(); ++it)
	{
		string line = *it + "\r\n";  // Windows line ending
		DWORD dwBytesWritten = 0;
		if (!WriteFile(hFile, line.c_str(), line.length(), &dwBytesWritten, NULL))
		{
			CloseHandle(hFile);
			return false;
		}
	}
	
	CloseHandle(hFile);
	return true;
}

bool CRomPathSettings::AddFavorite(const string& name)
{
    if (name.empty()) return false;
    if (IsFavorite(name)) return true;
    m_Favorites.insert(name);
    if (SaveFavorites()) return true;
    m_Favorites.erase(name);
    return false;
}

bool CRomPathSettings::RemoveFavorite(const string& name)
{
    if (name.empty()) return false;
    set<string> saved = m_Favorites;
    for (set<string>::iterator it = m_Favorites.begin(); it != m_Favorites.end(); )
    {
        if (_stricmp(it->c_str(), name.c_str()) == 0) m_Favorites.erase(it++);
        else ++it;
    }
    if (SaveFavorites()) return true;
    m_Favorites.swap(saved);
    return false;
}

bool CRomPathSettings::IsFavorite(const string& name) const
{
    for (set<string>::const_iterator it = m_Favorites.begin(); it != m_Favorites.end(); ++it)
        if (_stricmp(it->c_str(), name.c_str()) == 0) return true;
    return false;
}

set<string> CRomPathSettings::GetFavorites() const
{
	return m_Favorites;
}

int CRomPathSettings::GetMultitapMode(const string & romPath) const
{
    map<string, bool>::const_iterator it = m_MultitapModes.find(ControllerPathKey(romPath));
    return it == m_MultitapModes.end() ? -1 : (it->second ? 1 : 0);
}

bool CRomPathSettings::SetMultitapMode(const string & romPath, bool enabled)
{
    if (romPath.empty() || m_sXmlPath.empty()) return false;
    TiXmlDocument doc(m_XmlDoc);
    TiXmlElement *root = doc.FirstChildElement("Settings");
    if (!root) return false;
    map<string, bool> previous = m_MultitapModes;
    m_MultitapModes[ControllerPathKey(romPath)] = enabled;
    TiXmlElement *element = root->FirstChildElement("ControllerMode");
    while (element)
    {
        TiXmlElement *next = element->NextSiblingElement("ControllerMode");
        root->RemoveChild(element);
        element = next;
    }
    for (map<string, bool>::const_iterator it = m_MultitapModes.begin(); it != m_MultitapModes.end(); ++it)
    {
        TiXmlElement *mode = new TiXmlElement("ControllerMode");
        mode->SetAttribute("Path", it->first.c_str());
        mode->SetAttribute("Multitap", it->second ? "true" : "false");
        root->LinkEndChild(mode);
    }
    // Replace settings only after the complete temporary file reaches storage.
    const string temporary = m_sXmlPath + ".controllers.tmp";
    HANDLE file = CreateFile(temporary.c_str(), GENERIC_WRITE, 0, NULL,
                             CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        m_MultitapModes = previous;
        return false;
    }
    TiXmlPrinter printer;
    doc.Accept(&printer);
    DWORD written = 0;
    bool saved = WriteFile(file, printer.CStr(), printer.Size(), &written, NULL) != FALSE;
    saved = saved && written == printer.Size() && FlushFileBuffers(file) != FALSE;
    if (!CloseHandle(file)) saved = false;
    if (saved)
        saved = MoveFileEx(temporary.c_str(), m_sXmlPath.c_str(),
                           MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != FALSE;
    if (!saved)
    {
        DeleteFile(temporary.c_str());
        m_MultitapModes = previous;
        return false;
    }
    m_XmlDoc = doc;
    return true;
}

bool CRomPathSettings::Save(const string & sXmlPath)
{
	string pathToUse;
	if (!sXmlPath.empty())
	{
		pathToUse = sXmlPath;
	}
	else if (!m_sXmlPath.empty())
	{
		pathToUse = m_sXmlPath;
	}
	else
	{
		// Fallback to default path
		pathToUse = "GAME:\\settings.xml";
	}
	
	if (pathToUse.empty())
		return false;
	
	// Rebuild the entire XML file from scratch - simpler and more reliable
	TiXmlDocument doc;
	
	// Add XML declaration
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	
	// Create root Settings element
	TiXmlElement * root = new TiXmlElement( "Settings" );
	doc.LinkEndChild( root );
	
	// Add all mapped drives from memory
	for (map<string, string>::iterator it = m_DeviceMap.begin(); it != m_DeviceMap.end(); ++it)
	{
		TiXmlElement * mappedDrive = new TiXmlElement( "MappedDrive" );
		mappedDrive->SetAttribute( "DriveName", it->first.c_str() );
		mappedDrive->SetAttribute( "Path", it->second.c_str() );
		root->LinkEndChild( mappedDrive );
	}
	
	// Add preview path if we have one
	if (!m_PreviewPath.empty())
	{
		TiXmlElement * previewPath = new TiXmlElement( "PreviewPath" );
		TiXmlText * previewText = new TiXmlText( m_PreviewPath.c_str() );
		previewPath->LinkEndChild( previewText );
		root->LinkEndChild( previewPath );
	}
	
	// Add GameGenieEnabled setting
	TiXmlElement * gameGenieEnabled = new TiXmlElement( "GameGenieEnabled" );
	TiXmlText * gameGenieText = new TiXmlText( m_GameGenieEnabled ? "true" : "false" );
	gameGenieEnabled->LinkEndChild( gameGenieText );
	root->LinkEndChild( gameGenieEnabled );
	
	for (map<string, bool>::const_iterator it = m_MultitapModes.begin(); it != m_MultitapModes.end(); ++it)
	{
		TiXmlElement *mode = new TiXmlElement("ControllerMode");
		mode->SetAttribute("Path", it->first.c_str());
		mode->SetAttribute("Multitap", it->second ? "true" : "false");
		root->LinkEndChild(mode);
	}

	// Note: Favorites are now saved to a separate text file, not in XML
	// So we don't add them to the XML document
	
	// Save the document
	bool result = doc.SaveFile(pathToUse.c_str());
	
	// Update loaded document if save succeeded
	if (result)
	{
		m_sXmlPath = pathToUse;
		// Reload to keep in sync
		m_XmlDoc.LoadFile(pathToUse.c_str());
		m_bLoaded = true;
	}
	
	return result;
}

bool CRomPathSettings::CreateDefaultSettings(const string & sXmlPath)
{
	m_sXmlPath = sXmlPath;
	m_Favorites.clear();
	
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement( "Settings" );
	doc.LinkEndChild( root );

	// Add default mapped drives
	TiXmlElement * mappedDrive1 = new TiXmlElement( "MappedDrive" );
	mappedDrive1->SetAttribute( "DriveName", "GAME (XeX Location)" );
	mappedDrive1->SetAttribute( "Path", "GAME:\\ROMS\\" );
	root->LinkEndChild( mappedDrive1 );

	TiXmlElement * mappedDrive2 = new TiXmlElement( "MappedDrive" );
	mappedDrive2->SetAttribute( "DriveName", "USB Drive" );
	mappedDrive2->SetAttribute( "Path", "USB0:\\ROMS\\" );
	root->LinkEndChild( mappedDrive2 );

	// Add default preview path
	TiXmlElement * previewPath = new TiXmlElement( "PreviewPath" );
	TiXmlText * previewText = new TiXmlText( "hdd:\\EMUS\\SNES360\\Preview\\" );
	previewPath->LinkEndChild( previewText );
	root->LinkEndChild( previewPath );

	// Add GameGenieEnabled (default to true)
	TiXmlElement * gameGenieEnabled = new TiXmlElement( "GameGenieEnabled" );
	TiXmlText * gameGenieText = new TiXmlText( "true" );
	gameGenieEnabled->LinkEndChild( gameGenieText );
	root->LinkEndChild( gameGenieEnabled );

	// Favorites section (empty by default)
	// No favorites added in default settings

	// Save the document
	if (doc.SaveFile(sXmlPath.c_str()))
	{
		m_bLoaded = true;
		return true;
	}
	return false;
}
 