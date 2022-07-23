#include "showfps.h"

bool ShowFPS_Patched = false;

ShowFPS::ShowFPS() {}
ShowFPS::~ShowFPS() {}

void ShowFPS::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (ShowFPS_Patched) {
		return;
	}
	ShowFPS_Patched = true;
	if (!m_GamedllBase) {
		MessageBoxA(0, "GameDll≥ı ºªØ ß∞‹", "CrashFixer", 0);
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
}

void ShowFPS::Stop() {

}
