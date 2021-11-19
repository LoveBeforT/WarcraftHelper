#include "unlockfps.h"
#include <iostream>

bool UnlockFPS_Hooked = false;

void UnlockFPS::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (UnlockFPS_Hooked) {
		return;
	}
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
		break;
	case Version::v124e:
		addr += 0x62DF9B;
		break;
	case Version::v127a:
		addr += 0x5FCFB;
		break;
	default:
		return;
	}
	unsigned char bytes[] = { 0xFF };
	PatchMemory(addr, bytes, 1);
	UnlockFPS_Hooked = true;
}

void UnlockFPS::Stop() {

}
