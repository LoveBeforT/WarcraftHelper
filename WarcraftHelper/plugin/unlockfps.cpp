#include "unlockfps.hpp"
#include "config/config.hpp"
#include <iostream>
#include <d3d9types.h>
#include <d3d9caps.h>
#include <d3d9.h>

DWORD(*GetGameOpt)() = 0;
DWORD(__fastcall* SetGameOptValue)(DWORD pthis, DWORD unused, DWORD idx, DWORD valuee) = 0;

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

void UnlockFPS::Start() {
	DWORD addr = GetGameInstance()->GetGameDllBase();
	DWORD d3d9_addr = 0;
	DWORD *is_enable_d3d_addr = 0;
	DWORD war3_addr = 0;
	byte patterns[] = {
			0X83, 0XE0, 0XFB, 0X53,
			0XBA, 0X11, 0X00, 0X00,
			0X00, 0X8B, 0XCE, 0x00
	};

    if (!GetConfig()->m_unlockFps) {
        return;
    }

	switch (GetGameInstance()->GetGameVersion()) {
	case Version::v120e:
		war3_addr = (DWORD)GetModuleHandle("war3.exe");
		addr = GetGameInstance()->SearchPatterns(patterns, 11, war3_addr + 0x3DA00, war3_addr+ 0x100000);
		if (!addr) {
			return;
		}
		addr += 2;
		GetGameOpt = (DWORD(*)())(GetGameInstance()->GetGameDllBase() + 0x2A50);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(GetGameInstance()->GetGameDllBase() + 0x2CC0);
		is_enable_d3d_addr = (DWORD*)(GetGameInstance()->GetGameDllBase() + 0x7FA744);
		break;
	case Version::v124e:
		GetGameOpt = (DWORD(*)())(GetGameInstance()->GetGameDllBase() + 0x5720);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(GetGameInstance()->GetGameDllBase() + 0x57F0);
		addr += 0x62DF9B;
		is_enable_d3d_addr = (DWORD*)(GetGameInstance()->GetGameDllBase() + 0xA9E764);
		break;
	case Version::v126a:
		GetGameOpt = (DWORD(*)())(GetGameInstance()->GetGameDllBase() + 0x5720);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(GetGameInstance()->GetGameDllBase() + 0x57F0);
		addr += 0x62D7FB;
		is_enable_d3d_addr = (DWORD*)(GetGameInstance()->GetGameDllBase() + 0xA88724);
		break;
	case Version::v127a:
		GetGameOpt = (DWORD(*)())(GetGameInstance()->GetGameDllBase() + 0x23E00);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(GetGameInstance()->GetGameDllBase() + 0x25A70);
		addr += 0x5FCFB;
		d3d9_addr = GetGameInstance()->GetGameDllBase() + 0xEC6B0;
		is_enable_d3d_addr = (DWORD*)(GetGameInstance()->GetGameDllBase() + 0xB665C8);
		break;
	case Version::v127b:
		GetGameOpt = (DWORD(*)())(GetGameInstance()->GetGameDllBase() + 0x40ED0);
		SetGameOptValue = (DWORD(__fastcall*)(DWORD, DWORD, DWORD, DWORD))(GetGameInstance()->GetGameDllBase() + 0x40FA0);
		addr += 0x7B7AB;
		d3d9_addr = GetGameInstance()->GetGameDllBase() + 0x13FED0;
		is_enable_d3d_addr = (DWORD*)(GetGameInstance()->GetGameDllBase() + 0xCE3D50);
		break;
	default:
		return;
	}

	// 解除注册表中的fps上限
	this->WriteFPSLimit();

	// 解锁d3d
	unsigned char bytes[] = { 0xFF };
	Game::PatchMemory(addr, bytes, 1);
	Game::InlineHook((void*)d3d9_addr, GetD3d9Parameters, (void*&)org_GetD3d9Parameters);

	// d3d重设窗口
	if (d3d9_addr && is_enable_d3d_addr && *is_enable_d3d_addr) {
		this->ResetD3D();
	}
	// opengl重设
	if (is_enable_d3d_addr && !*is_enable_d3d_addr) {
		this->ResetOpenGL();
	}
}

void UnlockFPS::ResetD3D() {
	// 强制游戏重新加载d3d
	HWND target = GetGameInstance()->GetGameWindow();
	ShowWindow(target, SW_MINIMIZE);
	ShowWindow(target, SW_SHOWNORMAL);
}

void UnlockFPS::ResetOpenGL() {
	//// 强制游戏重新加载opengl
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
}

void UnlockFPS::WriteFPSLimit() {
	// 修改游戏刷新率上限
	DEVMODE dm;
	memset(&dm, 0, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(NULL, ENUM_REGISTRY_SETTINGS, &dm);
	if (dm.dmDisplayFrequency > 60) {
		switch (GetGameInstance()->GetGameVersion()) {
			case Version::v127b:
				SetGameOptValue(GetGameOpt(), 0, 4,  (DWORD)&dm.dmDisplayFrequency);	// refreshrate
				break;
			default:
				SetGameOptValue(GetGameOpt(), 0, 4, dm.dmDisplayFrequency);				// refreshrate
				break;
		}
		// SetGameOptValue(GetGameOpt(), 0, 22, 200);	// maxfps
		// WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "refreshrate", dm.dmDisplayFrequency);
	}
}

void UnlockFPS::Stop() {}
