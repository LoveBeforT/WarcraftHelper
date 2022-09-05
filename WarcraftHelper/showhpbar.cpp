#include "showhpbar.h"

bool ShowHPBar_Patched = false;
bool ShowHPBar_Closed = false;
DWORD is_chat_addr = 0x45CB8C;

DWORD __stdcall ShowBar(LPVOID lpThreadParameter) {
	HWND h_war3 = FindWindow(0, "Warcraft III");
	while (1)
	{
		if (ShowHPBar_Closed) {
			ExitThread(0);
		}
		if (*(DWORD*)is_chat_addr) {
			continue;
		}
		keybd_event(219, 0, 0, 0);
		keybd_event(221, 0, 0, 0);
		Sleep(500);

		/* PostMessage(h_war3, WM_KEYDOWN, 219, 0);
		 PostMessage(h_war3, WM_KEYDOWN, 221, 0);

		 PostMessage(h_war3, WM_KEYUP, 219, 0);
		 PostMessage(h_war3, WM_KEYUP, 221, 0);*/
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
	switch (m_War3Version) {
	case Version::v120e:
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
}

void ShowHPBar::Stop() {
	ShowHPBar_Closed = true;
	Sleep(51);
	keybd_event(219, 0, 2, 0);
	keybd_event(221, 0, 2, 0);
}
