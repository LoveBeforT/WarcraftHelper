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
	this->m_IsWar3 = this->IsWar3();
	if (!this->m_IsWar3) {
		return;
	}
	// 获取GameDll
	this->m_GamedllBase = (DWORD)GetModuleHandle("Game.dll");
	// 获取游戏版本
	this->m_War3Version = GetWar3Version();

	this->m_SizeBypass = &SizeBypass();
	this->m_WideScreen = &WideScreen();
	this->m_UnlockFPS = &UnlockFPS();
	this->m_WindowFixer = &WindowFixer();
	this->m_AutoRep = &AutoRep();
	this->m_ShowFPS = &ShowFPS();
	this->m_PathFix = &PathFix();
	this->m_ShowHPBar = &ShowHPBar();
}

Helper::~Helper() {
}

void Helper::Start() {
	if (!this->m_IsWar3) {
		return;
	}

	this->m_SizeBypass->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_WideScreen->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_UnlockFPS->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_WindowFixer->Start();
	this->m_AutoRep->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_ShowFPS->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_PathFix->Start(this->m_GamedllBase, this->m_War3Version);
	this->m_ShowHPBar->Start(this->m_GamedllBase, this->m_War3Version);
}

void Helper::Stop() {
	if (!this->m_IsWar3) {
		return;
	}

	this->m_SizeBypass->Stop();
	this->m_WideScreen->Stop();
	this->m_UnlockFPS->Stop();
	this->m_WindowFixer->Stop();
	this->m_AutoRep->Stop();
	this->m_ShowFPS->Stop();
	this->m_PathFix->Stop();
	this->m_ShowHPBar->Stop();
	Sleep(60);
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
