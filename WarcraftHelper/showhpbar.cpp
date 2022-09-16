#include "showhpbar.h"
#include <iostream>

bool ShowHPBar_Patched = false;
bool ShowHPBar_Closed = false;
DWORD *is_chat_addr = (DWORD*)0x45CB8C;
DWORD game_status = 0;

int(__fastcall* orgSetGameStatus)(DWORD *ecx, DWORD edx, DWORD status) = 0;
int __fastcall SetGameStatus(DWORD *ecx, DWORD edx, DWORD status) {
	DWORD ret = orgSetGameStatus(ecx, edx, status);
	game_status = ecx[156];
	return ret;
}

DWORD __stdcall ShowBar(LPVOID lpThreadParameter) {
	while (1)
	{
		if (ShowHPBar_Closed) {
			ExitThread(0);
		}
		if (IsBadReadPtr(is_chat_addr, 4)) {
			break;
		}
		if (*is_chat_addr) {
			continue;
		}
		if (game_status != 4) {
			continue;
		}
		keybd_event(219, 0, 0, 0);
		keybd_event(221, 0, 0, 0);
		Sleep(50);
	}
	return 0;
}

ShowHPBar::ShowHPBar() {}
ShowHPBar::~ShowHPBar() {}

void ShowHPBar::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (ShowHPBar_Patched) {
		return;
	}
	ShowHPBar_Patched = true;
	if (!m_GamedllBase) {
		MessageBoxA(0, "GameDll³õÊ¼»¯Ê§°Ü", "ShowHPBar", 0);
		return;
	}
	DWORD SetGameStatus_addr = 0;
	switch (m_War3Version) {
	case Version::v120e:
		SetGameStatus_addr = 0x6F283B60;
		break;
	default:
		return;
	}


	DWORD is_showfps = ReadDwordFromReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Gameplay", "healthbars");
	WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Gameplay", "healthbars", is_showfps);
	if (!is_showfps) {
		return;
	}
	this->thread = CreateThread(NULL, NULL, ShowBar, NULL, NULL, NULL);
	InlineHook((void*)SetGameStatus_addr, SetGameStatus, (void*&)orgSetGameStatus);
}

void ShowHPBar::Stop() {
	ShowHPBar_Closed = true;
	DetachHook((void*)orgSetGameStatus, SetGameStatus);
	TerminateThread(this->thread, 0);
}
