#include "showfps.h"

void ShowFPS::Start() {
	if (this->m_Hooked) {
		return;
	}
	this->m_Hooked = true;
	if (!this->m_GamedllBase) {
		MessageBoxA(0, "GameDll初始化失败", "ShowFPS", 0);
		return;
	}
	DWORD offset = this->m_GamedllBase;
	switch (this->m_War3Version) {
	case Version::v124e:
		offset += 0x34B248;
		break;
	case Version::v126a:
		offset += 0x34A708;
		break;
	case Version::v127a:
		offset += 0x397E5A;
		break;
	case Version::v127b:
		offset += 0x3B55FA;
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

void ShowFPS::Stop() {}
