#include "campaignfix.h"
#include <time.h>
#include <iostream>

DWORD NetDisconnected_addr = 0;

static void DoFixFrameSync() {
#ifdef _DEBUG
    printf("OnNetDisconnected called\n");
#endif
	Sleep(1000);
}

int(__fastcall* orgOnNetDisconnected120E)(DWORD , DWORD) = 0;
int __fastcall OnNetDisconnected120E(DWORD a1, DWORD a2) {
	DoFixFrameSync();
	return orgOnNetDisconnected120E(a1, a2);
}

int(__fastcall* orgOnNetDisconnected)(DWORD *, int, int, const char *, int, int, int, int, int) = 0;
int __fastcall OnNetDisconnected(DWORD *a1, int a2, int a3, const char *ArgList, int a5, int a6, int a7, int a8, int a9) {
    DoFixFrameSync();
	return orgOnNetDisconnected(a1, a2, a3, ArgList, a5, a6, a7, a8, a9);
}

void CampaignFix::Start() {
	if (this->m_Hooked) {
		return;
	}
	this->m_Hooked = true;
	if (!this->m_GamedllBase) {
		MessageBoxA(0, "GameDll初始化失败", "CampaignFix", 0);
		return;
	}
	switch (this->m_War3Version) {
	case Version::v120e:
        NetDisconnected_addr = this->m_GamedllBase + 0x6D6E00;
		InlineHook((void*)NetDisconnected_addr, OnNetDisconnected120E, (void*&)orgOnNetDisconnected120E);
		return;
	case Version::v124e:
        NetDisconnected_addr = this->m_GamedllBase + 0x539620;
		break;
	case Version::v126a:
        NetDisconnected_addr = this->m_GamedllBase + 0x538B20;
		break;
	case Version::v127a:
        NetDisconnected_addr = this->m_GamedllBase + 0x2F6950;
		break;
	case Version::v127b:
        NetDisconnected_addr = this->m_GamedllBase + 0x3140C0;
		break;
	default:
		return;
	}
	InlineHook((void*)NetDisconnected_addr, OnNetDisconnected, (void*&)orgOnNetDisconnected);
}

void CampaignFix::Stop() {
	DetachHook((void*)orgOnNetDisconnected, OnNetDisconnected);
}
