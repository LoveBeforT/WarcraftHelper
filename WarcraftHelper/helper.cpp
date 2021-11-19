#include "helper.h"
#include <atlstr.h>

#pragma comment(lib, "Version.lib")

Version GetWar3Version() {
	char Data[2048];
	memset(Data, 0, 2048);
	if (!GetFileVersionInfo("Game.dll", 0, sizeof(Data), Data))
	{
		return Version::unknown;
	}
	VS_FIXEDFILEINFO *FileInfo;
	UINT nBytes = 0;
	VerQueryValue(Data, "\\", (void**)&FileInfo, &nBytes);
	DWORD buildVersion = FileInfo->dwFileVersionLS & 0xffff;
	return static_cast<Version>(buildVersion);
}

Helper::Helper() {
	if (!this->IsWar3()) {
		return;
	}
	// 获取GameDll
	this->m_GamedllBase = (DWORD)LoadLibraryA("Game.dll");
	// 获取游戏版本
	this->m_War3Version = GetWar3Version();

	this->m_SizeBypass = &SizeBypass();
	this->m_WideScreen = &WideScreen();
	this->m_UnlockFPS = &UnlockFPS();
	this->m_CrashFixer = &CrashFixer();
	this->m_WindowFixer = &WindowFixer();
	this->m_AutoRep = &AutoRep();
}

void Helper::Start() {
	if (!this->IsWar3()) {
		return;
	}

	this->m_SizeBypass->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_WideScreen->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_UnlockFPS->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_CrashFixer->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_WindowFixer->Start();
	this->m_AutoRep->Start(this->m_GamedllBase, this->m_War3Version);
}

void Helper::Stop() {
	if (!this->IsWar3()) {
		return;
	}
	this->m_SizeBypass->Stop();
	this->m_WideScreen->Stop();
	this->m_UnlockFPS->Stop();
	this->m_CrashFixer->Stop();
	this->m_WindowFixer->Stop();
	this->m_AutoRep->Stop();
}

bool Helper::IsWar3() {
	TCHAR lpFilePATH[MAX_PATH];
	memset(lpFilePATH, 0, MAX_PATH * sizeof(TCHAR));
	::GetModuleFileName(NULL, lpFilePATH, MAX_PATH);
	CString tmp;
	tmp.Format("%s", lpFilePATH);
	DWORD loc = tmp.ReverseFind('\\');
	for (int i = loc + 1; i< MAX_PATH; i++) {
		if (lpFilePATH[i] >= 'A' && lpFilePATH[i] <= 'Z'){
			lpFilePATH[i] += 'a'-'A';
		}
	}
	if (strncmp(lpFilePATH + loc + 1, "war3.exe",8)) {
		return false;
	}
	return true;
}
