
#include "GeneralFunctions.h"
#include "Direct3D.h"
#include "KernelExports.h"

extern CDirect3D Direct3D;

HRESULT DXTRACE_ERR_MSGBOX( char *str, HRESULT hr )
{
	// stubbed code
	return S_OK;
}

HRESULT DXTRACE_ERR( char *str, HRESULT hr )
{
	// stubbed code
	return S_OK;
}

int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	// stubbed code

	return 0;
}

DWORD CheckMenuItem(      
    HMENU hmenu,
    UINT uIDCheckItem,
    UINT uCheck
)
{

	return 0;
}


const char *GetFilename (char *path, const char *InFileName, char *fext)
{
    static char filename [MAX_PATH + 1];
    char dir [_MAX_DIR + 1];
    char drive [_MAX_DRIVE + 1];
    char fname [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];
   _splitpath (InFileName, drive, dir, fname, ext);

   std::string fatxfname(fname);
	if (fatxfname.length() > 38)
	{
			fatxfname = fatxfname.substr(0,37);
	}

   _snprintf(filename, sizeof(filename),  "%s%s.%s",path, 
              fatxfname.c_str(), fext);
    return (filename);
}


//--------------------------------------------------------------------------------------
// Name: LoadFile()
// Desc: Helper function to load a file
//--------------------------------------------------------------------------------------
HRESULT LoadFile( const CHAR* strFileName, VOID** ppFileData, DWORD* pdwFileSize )
{
  
    if( pdwFileSize )
        *pdwFileSize = 0L;

    // Open the file for reading
    HANDLE hFile = CreateFile( strFileName, GENERIC_READ, 0, NULL,
                               OPEN_EXISTING, 0, NULL );

    if( INVALID_HANDLE_VALUE == hFile )
        return E_HANDLE;

    DWORD dwFileSize = GetFileSize( hFile, NULL );
    VOID* pFileData = malloc( dwFileSize );

    if( NULL == pFileData )
    {
        CloseHandle( hFile );
        return E_OUTOFMEMORY;
    }

    DWORD dwBytesRead;
    if( !ReadFile( hFile, pFileData, dwFileSize, &dwBytesRead, NULL ) )
    {
        CloseHandle( hFile );
        free( pFileData );
        return E_FAIL;
    }

    // Finished reading file
    CloseHandle( hFile );

    if( dwBytesRead != dwFileSize )
    {
        free( pFileData );
        return E_FAIL;
    }

    if( pdwFileSize )
        *pdwFileSize = dwFileSize;
    *ppFileData = pFileData;

    return S_OK;
}


// Name: LoadConstantTable()
// Desc: Creates a D3DX constant table object from the microcode constant table
//       contained within a microcode shader code buffer.
//       Returns NULL if no constant table is present in the shader.
//--------------------------------------------------------------------------------------
LPD3DXCONSTANTTABLE LoadConstantTable( VOID* pShaderCode )
{
    // Get the D3DXSHADER_CONSTANTTABLE structure. 
    const D3DXSHADER_CONSTANTTABLE* pDXShaderConstantTable;
    DWORD dwTableSize = 0;

    HRESULT hr = XGMicrocodeGetConstantTable( pShaderCode, &pDXShaderConstantTable, &dwTableSize );

    if( FAILED( hr ) )
    {
        return NULL;
    }

    if( dwTableSize > 0 )
    {
        // Get the LPD3DXCONSTANTTABLE object from the D3DXSHADER_CONSTANTTABLE structure. 
        D3DXSHADER_CONSTANTTABLE* pNativeConstantTable = ( D3DXSHADER_CONSTANTTABLE* )malloc( dwTableSize );
    

        XGCopyUCodeToNativeConstantTable( pDXShaderConstantTable, pNativeConstantTable, dwTableSize );

        LPD3DXCONSTANTTABLE pConstantTableObject = NULL;

        hr = XGCreateConstantTable( pNativeConstantTable, dwTableSize, &pConstantTableObject );

        free( pNativeConstantTable );

        if( FAILED( hr ) )
        {
            return NULL;
        }

        return pConstantTableObject;
    }

    return NULL;
}

//--------------------------------------------------------------------------------------
// Name: UnloadFile()
// Desc: Matching unload
//--------------------------------------------------------------------------------------
VOID UnloadFile( VOID* pFileData )
{
    
    free( pFileData );
}


wchar_t ucString[42];

LPCWSTR MultiCharToUniChar(char* mbString)
{
	int len = strlen(mbString) + 1;	
	mbstowcs(ucString, mbString, len);
	return (LPCWSTR)ucString;
}


VOID MapInternalDrives()
	{
		//Create our string array for the name of the paths for the internal drives.
		char* mappedPaths[] = 
		{
			"\\??\\dvd:",
			//"\\??\\devkit:",
			//"\\??\\flash:",
			"\\??\\hdd:", 
			//"\\??\\hddcache0:",
			//"\\??\\hddcache1:",
			//"\\??\\hddsys:", 
			//"\\??\\hddvdplayer:", 
			//"\\??\\hddvdrom:",
			//"\\??\\hddvdstorage:",
			"\\??\\memunit0:",
			"\\??\\memunit1:", 
			//"\\??\\memunitsys0:",
			//"\\??\\memunitsys1:",
			//"\\??\\network:",
			"\\??\\usb0:",
			"\\??\\usb1:",
			"\\??\\usb2:"
		};

		//Create our string array for our paths to map.
		char* pathsToMap[] = 
		{
			"\\Device\\Cdrom0",
			//"\\Device\\Harddisk0\\Partition1\\DEVKIT",
			//"\\Device\\Flash",
			"\\Device\\Harddisk0\\Partition1",
			//"\\Device\\Harddisk0\\Cache0",
			//"\\Device\\Harddisk0\\Cache1",
			//"\\Device\\Harddisk0\\SystemPartition",
			//"\\Device\\HdDvdPlayer",
			//"\\Device\\HdDvdRom",
			//"\\Device\\HdDvdStorage",
			"\\Device\\Mu0",
			"\\Device\\Mu1",
			//"\\Device\\Mu0System",
			//"\\Device\\Mu1System",
			//"\\Network",
			"\\Device\\Mass0",
			"\\Device\\Mass1",
			"\\Device\\Mass2"
		};
		//Loop for each drive to map.
		for(int i = 0; i < ARRAYSIZE(mappedPaths); ++i)
		{
			// Lets map our drives now
			if (!CreateDriveMapping(mappedPaths[i], pathsToMap[i]))
			{
				//Show our error string.
 
			}
			else
			{
				//Show our success string.
 
			}
		}
	}
	bool CreateDriveMapping(const char * szMappingName, const char * szDeviceName)
	{
		ANSI_STRING linkname, devicename;
		RtlInitAnsiString(&linkname, szMappingName);
		RtlInitAnsiString(&devicename, szDeviceName);
		NTSTATUS status = ObCreateSymbolicLink(&linkname, &devicename);
		if (status >= 0)
			return true;
		return false;
	}


void CreateDirectoryAnyDepth(const wchar_t *path)
{
	wchar_t opath[MAX_PATH]; 
	wchar_t *p; 
	size_t len; 
	wcsncpy_s(opath, path, sizeof(opath)); 
	len = wcslen(opath); 
	if(opath[len - 1] == L'/') 
	opath[len - 1] = L'\0'; 

	for(p = opath; *p; p++) 
	{
	if(*p == L'/' || *p == L'\\') 
	{ 
	*p = L'\0'; 
	if(_waccess(opath, 0)) 
	_wmkdir(opath);
	*p = L'\\';
	}
	}
	if(_waccess(opath, 0))
	_wmkdir(opath);
}

HRESULT LoadPixelShader( const CHAR* strFileName, LPDIRECT3DPIXELSHADER9* ppPS, LPD3DXCONSTANTTABLE* ppConstantTable )
{
    HRESULT hr;
    VOID* pCode = NULL;
    ( *ppPS ) = NULL;

    if( FAILED( hr = LoadFile( strFileName, &pCode ) ) )
        return hr;
	if( FAILED( hr = Direct3D.pDevice->CreatePixelShader( ( DWORD* )pCode, ppPS ) ) )
    {
        UnloadFile( pCode );
        return hr;
    }
    if( ppConstantTable != NULL )
    {
        *ppConstantTable = LoadConstantTable( pCode );
    }
    UnloadFile( pCode );

    return hr;
}
 