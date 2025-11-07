
#ifndef TINYXML_H
#define TINYXML_H
#pragma once

#include "tinyxml\tinyxml.h"
#include <string>
#include <map>
#include <set>

#define DEFINE_XML_PROP(name) \
	public:\
		string Get##name() const;\
	private:\
		string m_##name;

using namespace std;

class CRomPathSettings
{

public:
	CRomPathSettings();
	virtual ~CRomPathSettings();

public:
	bool Load(const string & sXmlPath);
	bool CreateDefaultSettings(const string & sXmlPath);

	map<string,string>::iterator GetDeviceMapBegin();
	map<string,string>::iterator GetDeviceMapEnd();
	map<string,string>::iterator FindDevice(std::string Device);
	
	// Favorites management (uses text file, not XML)
	bool AddFavorite(const string & sRomName);
	bool RemoveFavorite(const string & sRomName);
	bool IsFavorite(const string & sRomName) const;
	set<string> GetFavorites() const;
	bool LoadFavorites();
	bool SaveFavorites();
	bool Save(const string & sXmlPath);

	DEFINE_XML_PROP(XboxGamePath)
	DEFINE_XML_PROP(Xbox360GamePath)
	DEFINE_XML_PROP(DefaultFontName)

private:
	TiXmlDocument m_XmlDoc;
	bool m_bLoaded;
	map<string, string> m_DeviceMap;
	set<string> m_Favorites;
	string m_sXmlPath;
public:
	string m_PreviewPath;
};

#endif
