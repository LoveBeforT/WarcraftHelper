#include "showhpbar.h"
#include "game/warcraft.h"
#include "config/config.h"
#include <mutex>
#include <iostream>

static DWORD gameStatus = 0;
static std::mutex mutex;

int(__fastcall* orgSetGameStatus)(DWORD *ecx, DWORD edx, DWORD status) = 0;
int __fastcall SetGameStatus(DWORD *ecx, DWORD edx, DWORD status) {
	DWORD ret = orgSetGameStatus(ecx, edx, status);
	gameStatus = ecx[156];
	return ret;
}

int(__fastcall* orgSetCmdToQueue)(DWORD ecx, DWORD edx, DWORD a3, DWORD a4, DWORD a5) = 0;
int __fastcall SetCmdToQueue(DWORD ecx, DWORD edx, DWORD a3, DWORD a4, DWORD a5) {
	DWORD ret = 0;
	std::lock_guard<std::mutex> guard(mutex);

	if (edx == 274 || edx == 275) {
		if (ecx == 7) {
			ret = orgSetCmdToQueue(8, edx, a3, a4, a5);
		}
		else if (ecx == 8) {
			ret = orgSetCmdToQueue(7, edx, a3, a4, a5);
		}
		else if (ecx == 27){
			ret = orgSetCmdToQueue(7, edx, a3, a4, a5);
		}
		else if (ecx == 28){
			ret = orgSetCmdToQueue(8, edx, a3, a4, a5);
		}
		else {
			ret = orgSetCmdToQueue(ecx, edx, a3, a4, a5);
		}
	}
	else {
		ret = orgSetCmdToQueue(ecx, edx, a3, a4, a5);
	}
	return ret;
}

DWORD __stdcall ShowBar(LPVOID lpThreadParameter) {
	while (1)
	{
		if (gameStatus == 4) {
			if (!System::IsKeyDown(VK_OEM_4)) {
				SetCmdToQueue(27, 274, 0, 0, 0);
			}
			if (!System::IsKeyDown(VK_OEM_6)) {
				SetCmdToQueue(27, 275, 0, 0, 0);
			}
		}
		Sleep(1000);
	}
	return 0;
}

void ShowHPBar::Start() {
	DWORD setGameStatusAddr = 0;
	DWORD setCmdToQueueAddr = (DWORD)GetModuleHandle("war3.exe");
	byte patterns[] = { 0x55, 0x8B, 0xEC, 0xA1,
						0x80, 0xCB, 0x45, 0x00,
						0x83, 0xF8, 0x0F, 0x56,
						0x57, 0x75, 0x04 };

    if (!GetConfig()->m_showHpBar) {
        return;
    }

	switch (GetGameInstance()->GetGameVersion()) {
	case Version::v120e:
		setGameStatusAddr = 0x6F283B60;
		setCmdToQueueAddr = GetGameInstance()->SearchPatterns(
			patterns, 15, setCmdToQueueAddr + 0x10000, setCmdToQueueAddr+ 0x50000);
		break;
	default:
		return;
	}

	this->thread = CreateThread(NULL, NULL, ShowBar, NULL, NULL, NULL);
	Game::InlineHook((void*)setGameStatusAddr, SetGameStatus, (void*&)orgSetGameStatus);
	Game::InlineHook((void*)setCmdToQueueAddr, SetCmdToQueue, (void*&)orgSetCmdToQueue);
}

void ShowHPBar::Stop() {
	Game::DetachHook((void*)orgSetGameStatus, SetGameStatus);
	TerminateThread(this->thread, 0);
}
