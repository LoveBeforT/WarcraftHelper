#include "widescreen.h"
#include <iostream>
#include <process.h>

void(__fastcall* p_orgCreateMatrixPerspectiveFov) (float*, DWORD, float, float, float, float) = 0;
void __fastcall CreateMatrixPerspectiveFov(float* outMatrix, DWORD edx, float fovY, float aspectRatio, float nearZ, float farZ)
{
	RECT r;
	float fWideScreenMul = 1.0f;
	if (GetWindowRect(GetWar3Window(), &r))
	{
		float width = float(r.right - r.left);
		float rHeight = 1.0f / (r.bottom - r.top);
		fWideScreenMul = width * rHeight * 0.75f; // (width / height) / (4.0f / 3.0f)
	}

	float yScale = 1.0f / tan(fovY * 0.5f / sqrt(aspectRatio * aspectRatio + 1.0f));
	float xScale = yScale / (aspectRatio * fWideScreenMul);
	outMatrix[0] = xScale;
	outMatrix[5] = yScale;
	outMatrix[10] = (nearZ + farZ) / (farZ - nearZ);
	outMatrix[14] = (-2.0f * nearZ * farZ) / (farZ - nearZ);

	outMatrix[1] = 0.0f;
	outMatrix[2] = 0.0f;
	outMatrix[3] = 0.0f;
	outMatrix[4] = 0.0f;
	outMatrix[6] = 0.0f;
	outMatrix[7] = 0.0f;
	outMatrix[8] = 0.0f;
	outMatrix[9] = 0.0f;
	outMatrix[11] = 1.0f;
	outMatrix[12] = 0.0f;
	outMatrix[13] = 0.0f;
	outMatrix[15] = 0.0f;
}

void WideScreen::Start() {
	if (this->m_Hooked) {
		return;
	}
	this->m_Hooked = true;
	if (!this->m_GamedllBase) {
		MessageBoxA(0, "GameDll初始化失败", "WideScreen", 0);
		return;
	}
	HWND hwar3 = GetWar3Window();
	if (!hwar3) {
		MessageBoxA(0, "War3窗口获取失败", "WideScreen", 0);
		return;
	}
	// 设置全屏
	DWORD auto_fullscreen = ReadDwordFromReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "autofullscreen");
	WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "autofullscreen", auto_fullscreen);
	if (auto_fullscreen) {
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		DWORD last_style = GetWindowLong(hwar3, GWL_STYLE);
		SetWindowLongPtr(hwar3, GWL_STYLE, last_style ^ 0xCF0000);
		SetWindowPos(hwar3, NULL, 0, 0, w, h, SWP_NOZORDER);
	}
	// 设置宽屏
	DWORD offset = this->m_GamedllBase;
	switch (this->m_War3Version) {
	case Version::v120e:
		offset += 0x0DBD40;
		break;
	case Version::v124e:
		offset += 0x7B6E90;
		break;
	case Version::v126a:
		offset += 0x7B66F0;
		break;
	case Version::v127a:
		offset += 0x0D31D0;
		break;
	case Version::v127b:
		offset += 0x126E30;
		break;
	default:
		return;
	}
	InlineHook((void*)offset, CreateMatrixPerspectiveFov, (void*&)p_orgCreateMatrixPerspectiveFov);
}

void WideScreen::Stop() {
	DetachHook((void*)p_orgCreateMatrixPerspectiveFov, CreateMatrixPerspectiveFov);
}

