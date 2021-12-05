#include "showfps.h"

bool ShowFPS_Patched = false;

DWORD ReadDwordFromReg(LPCTSTR regPath, LPCTSTR keyName);
BOOL WriteDwordToReg(LPCTSTR regPath, LPCTSTR keyName, DWORD value);

void ShowFPS::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (ShowFPS_Patched) {
		return;
	}
	if (!m_GamedllBase) {
		MessageBox(0, "GameDll≥ı ºªØ ß∞‹", "CrashFixer", 0);
		return;
	}
	DWORD offset = m_GamedllBase;
	switch (m_War3Version) {
	case Version::v124e:
		offset += 0x34B248;
		break;
	case Version::v127a:
		offset += 0x397E5A;
		break;
	default:
		return;
	}
	
	DWORD is_showfps = ReadDwordFromReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "showfps");
	WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "showfps", is_showfps);
	if (!is_showfps) {
		return;
	}

	unsigned char bytes[] = { 0x01 };
	PatchMemory(offset, bytes,1);
	ShowFPS_Patched = true;
}

void ShowFPS::Stop() {

}

DWORD ReadDwordFromReg(LPCTSTR regPath, LPCTSTR keyName)
{
	HKEY hKey = 0;
	long lret = ::RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey);
	if (lret == ERROR_SUCCESS)
	{
		DWORD value;
		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(DWORD);

		lret = ::RegQueryValueEx(hKey, keyName, 0, &dwType, (BYTE*)&value, &dwSize);
		if ((lret == ERROR_SUCCESS) && (dwType == REG_DWORD))
		{
			::RegCloseKey(hKey);
			return value;
		}
		::RegCloseKey(hKey);
	}

	return 0;
}

BOOL WriteDwordToReg(LPCTSTR regPath, LPCTSTR keyName, DWORD value)
{
	HKEY hKey = 0;
	long lret = ::RegCreateKey(HKEY_CURRENT_USER, regPath, &hKey);
	if (lret == ERROR_SUCCESS)
	{
		lret = ::RegSetValueEx(hKey, keyName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD));
		if (lret == ERROR_SUCCESS)
		{
			::RegCloseKey(hKey);
			return TRUE;
		}
		::RegCloseKey(hKey);
	}

	return FALSE;
}