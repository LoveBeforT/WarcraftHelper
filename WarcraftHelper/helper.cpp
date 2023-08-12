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
	this->m_IsWar3 = IsWar3();
	if (!this->m_IsWar3) {
		return;
	}
	DWORD gamedll = (DWORD)GetModuleHandle("Game.dll");
	Version gamedversion = GetWar3Version();
	// 获取GameDll
	this->m_GamedllBase = gamedll;
	// 获取游戏版本
	this->m_War3Version = gamedversion;

	this->m_SizeBypass = SizeBypass(gamedll, gamedversion);
	this->m_WideScreen = WideScreen(gamedll, gamedversion);
	this->m_UnlockFPS = UnlockFPS(gamedll, gamedversion);
	this->m_WindowFixer = WindowFixer(gamedll, gamedversion);
	this->m_AutoRep = AutoRep(gamedll, gamedversion);
	this->m_ShowFPS = ShowFPS(gamedll, gamedversion);
	this->m_PathFix = PathFix(gamedll, gamedversion);
	this->m_ShowHPBar = ShowHPBar(gamedll, gamedversion);
	this->m_CampaignFix = CampaignFix(gamedll, gamedversion);
	this->m_U9Helper = U9Helper(gamedll, gamedversion);
	this->m_ReplayView = ReplayView(gamedll, gamedversion);
}

Helper::~Helper() {
}

void Helper::Start() {
	if (!this->m_IsWar3) {
		return;
	}
#ifdef _DEBUG
	InitConsole();
#endif
	
	this->m_UnlockFPS.Start();
	this->m_SizeBypass.Start();
	this->m_WideScreen.Start();
	this->m_AutoRep.Start();
	this->m_ShowFPS.Start();
	this->m_PathFix.Start();
	this->m_ShowHPBar.Start();
	this->m_WindowFixer.Start();
	this->m_CampaignFix.Start();
	this->m_U9Helper.Start();
	this->m_ReplayView.Start();

	this->LoadPlugins();
}

void Helper::Stop() {
	if (!this->m_IsWar3) {
		return;
	}

	this->m_UnlockFPS.Stop();
	this->m_SizeBypass.Stop();
	this->m_WideScreen.Stop();
	this->m_WindowFixer.Stop();
	this->m_AutoRep.Stop();
	this->m_ShowFPS.Stop();
	this->m_PathFix.Stop();
	this->m_ShowHPBar.Stop();
	this->m_CampaignFix.Stop();
	this->m_U9Helper.Stop();
	this->m_ReplayView.Stop();
	Sleep(60);
}

bool IsWar3() {
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


bool InitConsole()
{
	AllocConsole();

	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	SetConsoleTitleA("WarcraftHelper");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, false);
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(hwnd, GWL_STYLE, style);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_MAXIMIZE, MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_SIZE, MF_BYCOMMAND);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_FRAMECHANGED);
	DrawMenuBar(hwnd);

	ShowWindow(hwnd, SW_MINIMIZE);

	return true;
}

void Helper::LoadPlugins() {
	if (!this->m_GamedllBase) {
		return;
	}


}