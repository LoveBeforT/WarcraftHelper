#include "unlockfps.h"
#include <iostream>
#include <d3d9types.h>
#include <d3d9caps.h>
#include <d3d9.h>

bool UnlockFPS_Hooked = false;

DWORD(*GetGameOpt)() = 0;
DWORD(__fastcall* SetGameOptValue)(DWORD pthis, DWORD unused, DWORD idx, DWORD valuee) = 0;

UnlockFPS::UnlockFPS() {}
UnlockFPS::~UnlockFPS() {}

DWORD(__fastcall* org_GetD3d9Parameters)(DWORD pthis, DWORD unused, D3DPRESENT_PARAMETERS* pPresentationParameters) = 0;
DWORD __fastcall GetD3d9Parameters(DWORD pthis, DWORD unused, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	DWORD result = org_GetD3d9Parameters(pthis, unused, pPresentationParameters);
	if (pPresentationParameters) {
		pPresentationParameters->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	//IDirect3DDevice9* pReturnedDeviceInterface = *(IDirect3DDevice9**)(pthis + 1412);
	//if (pReturnedDeviceInterface) {
	//	// reset d3d9
	//	pReturnedDeviceInterface->Reset(pPresentationParameters);
	//}
	return result;
}

void UnlockFPS::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (UnlockFPS_Hooked) {
		return;
	}
	UnlockFPS_Hooked = true;
	DWORD addr = m_GamedllBase;
	DWORD d3d9_addr = 0;
	DWORD *is_enable_d3d_addr = 0;
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
		is_enable_d3d_addr = (DWORD*)(m_GamedllBase + 0x7FA744);
		break;
	case Version::v124e:
		GetGameOpt = (DWORD(*)())(m_GamedllBase + 0x5720);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(m_GamedllBase + 0x57F0);
		addr += 0x62DF9B;
		is_enable_d3d_addr = (DWORD*)(m_GamedllBase + 0xA9E764);
		break;
	case Version::v127a:
		GetGameOpt = (DWORD(*)())(m_GamedllBase + 0x23E00);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(m_GamedllBase + 0x25A70);
		addr += 0x5FCFB;
		d3d9_addr = m_GamedllBase + 0xEC6B0;
		is_enable_d3d_addr = (DWORD*)(m_GamedllBase + 0xB665C8);
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
	InlineHook((void*)d3d9_addr, GetD3d9Parameters, (void*&)org_GetD3d9Parameters);

	//// opengl 解锁垂直同步
	//if (is_enable_d3d_addr && !*is_enable_d3d_addr) {
	//	HMODULE hOpengl = GetModuleHandle("opengl32.dll");
	//	if (!hOpengl) {
	//		return;
	//	}
	//	DWORD(*wglGetProcAddress)(const char*) = (DWORD(*)(const char*))GetProcAddress(hOpengl, "wglGetProcAddress");
	//	if (!wglGetProcAddress) {
	//		return;
	//	}
	//	DWORD(*wglSwapIntervalEXT)(DWORD) = (DWORD(*)(DWORD))wglGetProcAddress("wglSwapIntervalEXT");
	//	if (!wglSwapIntervalEXT) {
	//		return;
	//	}
	//	wglSwapIntervalEXT(1);
	//}

	// d3d重设窗口
	if (d3d9_addr && is_enable_d3d_addr && *is_enable_d3d_addr) {
		HWND target = GetActiveWindow();
		ShowWindow(target, SW_MINIMIZE);
		ShowWindow(target, SW_SHOWNORMAL);
	}
}

void UnlockFPS::Stop() {
	DetachHook((void*)org_GetD3d9Parameters, GetD3d9Parameters);
}
