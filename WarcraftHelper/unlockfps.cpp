#include "unlockfps.h"
#include <iostream>

bool UnlockFPS_Hooked = false;

DWORD(*GetGameOpt)() = 0;
DWORD(__fastcall* SetGameOptValue)(DWORD pthis, DWORD unused, DWORD idx, DWORD valuee) = 0;

UnlockFPS::UnlockFPS() {}
UnlockFPS::~UnlockFPS() {}

void UnlockFPS::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (UnlockFPS_Hooked) {
		return;
	}
	UnlockFPS_Hooked = true;
	DWORD addr = m_GamedllBase;
	DWORD war3_addr = 0;
	byte SetFPS_pattern[] = {
			0X83, 0XE0, 0XFB, 0X53,
			0XBA, 0X11, 0X00, 0X00,
			0X00, 0X8B, 0XCE, 0x00
	};
	switch (m_War3Version) {
	case Version::v120e:
		war3_addr = (DWORD)GetModuleHandle("war3.exe");
		addr = War3_Search(SetFPS_pattern, 11, war3_addr + 0x3DA00, war3_addr+ 0x100000);
		if (!addr) {
			return;
		}
		addr += 2;
		GetGameOpt = (DWORD(*)())(m_GamedllBase + 0x2A50);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(m_GamedllBase + 0x2CC0);
		break;
	case Version::v124e:
		GetGameOpt = (DWORD(*)())(m_GamedllBase + 0x5720);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(m_GamedllBase + 0x57F0);
		addr += 0x62DF9B;
		break;
	case Version::v127a:
		GetGameOpt = (DWORD(*)())(m_GamedllBase + 0x23E00);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(m_GamedllBase + 0x25A70);
		addr += 0x5FCFB;
		break;
	default:
		return;
	}

	DEVMODE dm;
	memset(&dm, 0, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(NULL, ENUM_REGISTRY_SETTINGS, &dm);
	if (dm.dmDisplayFrequency > 60) {
		SetGameOptValue(GetGameOpt(), 0, 4, dm.dmDisplayFrequency);	// refreshrate
		// SetGameOptValue(GetGameOpt(), 0, 22, 200);	// maxfps
		// WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "refreshrate", dm.dmDisplayFrequency);
	}

	unsigned char bytes[] = { 0xFF };
	PatchMemory(addr, bytes, 1);
}

void UnlockFPS::Stop() {

}
