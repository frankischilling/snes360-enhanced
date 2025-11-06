
#include "RomSettings.h"
 

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
	

CRomPathSettings::CRomPathSettings() : m_bLoaded(false)
{
}

CRomPathSettings::~CRomPathSettings()
{	
}

bool CRomPathSettings::Load(const string & sXmlPath)
{	
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

bool CRomPathSettings::CreateDefaultSettings(const string & sXmlPath)
{
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

	// Save the document
	if (doc.SaveFile(sXmlPath.c_str()))
	{
		return true;
	}
	return false;
}
 