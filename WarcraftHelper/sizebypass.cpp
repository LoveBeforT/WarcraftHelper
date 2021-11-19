#include "sizebypass.h"
#include <Psapi.h>
#include <array>
#include <strsafe.h>

#pragma comment(lib, "Psapi.lib")
#define BUFSIZE 512

Version sizebypass_war3version;
DWORD(__stdcall * p_orgGetFileSize) (HANDLE file, LPDWORD lpFileSizeHigh);

BOOL IsMapFile(char x1, char x2, char x3, char x4)
{
	char fileext[5];
	memset(fileext, 0, 5);
	fileext[0] = x1;
	fileext[1] = x2;
	fileext[2] = x3;
	fileext[3] = x4;
	return _stricmp(fileext, ".w3x") == 0 || _stricmp(fileext, ".w3m") == 0;
}

char * GetFileNameFromHandle(HANDLE hFile)
{
	TCHAR * pszFilename = new TCHAR[MAX_PATH + 1];
	HANDLE hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		1,
		NULL);
	if (!hFileMap)
	{
		return pszFilename;
	}
	void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
	if (!pMem)
	{
		CloseHandle(hFileMap);
		return pszFilename;
	}
	if (!GetMappedFileName(GetCurrentProcess(),
		pMem,
		pszFilename,
		MAX_PATH))
	{
		UnmapViewOfFile(pMem);
		CloseHandle(hFileMap);
		return pszFilename;
	}
	// Translate path with device name to drive letters.
	TCHAR szTemp[BUFSIZE];
	szTemp[0] = '\0';
	if (!GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
	{
		UnmapViewOfFile(pMem);
		CloseHandle(hFileMap);
		return pszFilename;
	}
	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;
	do
	{
		// Copy the drive letter to the template string
		*szDrive = *p;
		// Look up each device name
		if (QueryDosDevice(szDrive, szName, MAX_PATH))
		{
			size_t uNameLen = strlen(szName);
			if (uNameLen < MAX_PATH)
			{
				bFound = strnicmp(pszFilename, szName, uNameLen) == 0
					&& *(pszFilename + uNameLen) == ('\\');
				if (bFound)
				{
					// Reconstruct pszFilename using szTempFile
					// Replace device path with DOS path
					TCHAR szTempFile[MAX_PATH];
					StringCchPrintf(szTempFile,
						MAX_PATH,
						TEXT("%s%s"),
						szDrive,
						pszFilename + uNameLen);
					StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, strlen(szTempFile));
				}
			}
		}
		while (*p++);
	} while (!bFound && *p);
	UnmapViewOfFile(pMem);
	CloseHandle(hFileMap);
	return pszFilename;
}

//获取文件大小函数
DWORD  __stdcall myGetFileSize(HANDLE file, LPDWORD lpFileSizeHigh)
{
	DWORD retval = p_orgGetFileSize(file, lpFileSizeHigh);
	if (retval <=0 )
	{
		return retval;
	}
	char * filenamex = GetFileNameFromHandle(file);
	if (!filenamex)
	{
		delete[] filenamex;
		return retval;
	}
	int strlenx = strlen(filenamex);
	if (strlenx > 5 && IsMapFile(filenamex[strlenx - 4], filenamex[strlenx - 3], filenamex[strlenx - 2], filenamex[strlenx - 1]))
	{
		if (sizebypass_war3version >= Version::v124a)
		{
			if (retval > 0x7FFFFF)
			{
				retval = 0x7FFFFF;
			}
		}
		else
		{
			if (retval > 0x2FFFFF)
			{
				retval = 0x2FFFFF;
			}
		}
	}
	delete[] filenamex;
	return retval;
}

SizeBypass::SizeBypass(DWORD base, Version version)
	:m_GamedllBase(base),m_War3Version(version),m_Hooked(false)
{
	sizebypass_war3version = version;
	this->m_kernel = GetModuleHandle("Kernel32.dll");
}

void SizeBypass::Start() {
	if (this->m_Hooked) {
		return;
	}
	if (!this->m_kernel) {
		MessageBox(0, "kernel32初始化失败", "SizeBypass", 0);
		return;
	}
	DWORD GetFileSize_addr = (DWORD)GetProcAddress(this->m_kernel, "GetFileSize");
	if (!GetFileSize_addr) {
		MessageBox(0, "GetFileSize初始化失败", "SizeBypass", 0);
		return;
	}
	InlineHook((void*)GetFileSize_addr, myGetFileSize,(void*&)p_orgGetFileSize);
	this->m_Hooked = true;
}

void SizeBypass::Stop() {

}
