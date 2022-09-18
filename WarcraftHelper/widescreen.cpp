#include "widescreen.h"
#include <iostream>
#include <process.h>

bool WideScreen_Hooked = false;
HWND g_hWnd = NULL;

void(__fastcall* p_orgCreateMatrixPerspectiveFov) (uint32_t outMatrix, uint32_t unused, float fovY, float aspectRatio, float nearZ, float farZ) = 0;
void __fastcall CreateMatrixPerspectiveFov(uint32_t outMatrix, uint32_t unused, float fovY, float aspectRatio, float nearZ, float farZ)
{
	RECT r;
	float fWideScreenMul = 1.0f;
	if (GetWindowRect(g_hWnd, &r))
	{
		float width = float(r.right - r.left);
		float rHeight = 1.0f / (r.bottom - r.top);
		fWideScreenMul = width * rHeight * 0.75f; // (width / height) / (4.0f / 3.0f)
	}

	float yScale = 1.0f / tan(fovY * 0.5f / sqrt(aspectRatio * aspectRatio + 1.0f));
	float xScale = yScale / (aspectRatio * fWideScreenMul);
	((float*)outMatrix)[0] = xScale;
	((float*)outMatrix)[5] = yScale;
	((float*)outMatrix)[10] = (nearZ + farZ) / (farZ - nearZ);
	((float*)outMatrix)[14] = (-2.0f * nearZ * farZ) / (farZ - nearZ);

	((float*)outMatrix)[1] = 0.0f;
	((float*)outMatrix)[2] = 0.0f;
	((float*)outMatrix)[3] = 0.0f;
	((float*)outMatrix)[4] = 0.0f;
	((float*)outMatrix)[6] = 0.0f;
	((float*)outMatrix)[7] = 0.0f;
	((float*)outMatrix)[8] = 0.0f;
	((float*)outMatrix)[9] = 0.0f;
	((float*)outMatrix)[11] = 1.0f;
	((float*)outMatrix)[12] = 0.0f;
	((float*)outMatrix)[13] = 0.0f;
	((float*)outMatrix)[15] = 0.0f;
}

WideScreen::WideScreen() {}
WideScreen::~WideScreen() {}

void WideScreen::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (WideScreen_Hooked) {
		return;
	}
	WideScreen_Hooked = true;
	if (!m_GamedllBase) {
		MessageBoxA(0, "GameDll初始化失败", "WideScreen", 0);
		return;
	}
	g_hWnd = GetActiveWindow();
	if (!g_hWnd) {
		MessageBoxA(0, "War3窗口获取失败", "WideScreen", 0);
		return;
	}
	// 设置全屏
	DWORD auto_fullscreem = ReadDwordFromReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "autofullscreen");
	WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "autofullscreen", auto_fullscreem);
	if (auto_fullscreem) {
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		DWORD last_style = GetWindowLong(g_hWnd, GWL_STYLE);
		SetWindowLongPtr(g_hWnd, GWL_STYLE, last_style ^ 0xCF0000);
		SetWindowPos(g_hWnd, NULL, 0, 0, w, h, SWP_NOZORDER);
	}
	// 设置宽屏
	DWORD offset = m_GamedllBase;
	switch (m_War3Version) {
	case Version::v120e:
		offset += 0x0DBD40;
		break;
	case Version::v124e:
		offset += 0x7B6E90;
		break;
	case Version::v127a:
		offset += 0x0D31D0;
		break;
	default:
		return;
	}
	InlineHook((void*)offset, CreateMatrixPerspectiveFov, (void*&)p_orgCreateMatrixPerspectiveFov);
}

void WideScreen::Stop() {
	DetachHook((void*)p_orgCreateMatrixPerspectiveFov, CreateMatrixPerspectiveFov);
}

