#include "widescreen.h"
#include <iostream>
#include <process.h>

void(__fastcall* p_orgCreateMatrixPerspectiveFov) (float*, DWORD, float, float, float, float) = 0;
void __fastcall CreateMatrixPerspectiveFov(float* outMatrix, DWORD edx, float fovY, float aspectRatio, float nearZ, float farZ)
{
	RECT r;
	float fWideScreenMul = 1.0f;
	if (GetWindowRect(GetGameInstance()->GetGameWindow(), &r))
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
	DWORD fullscreen = 0;
	HWND hwar3 = GetGameInstance()->GetGameWindow();
	DWORD offset = GetGameInstance()->GetGameDllBase();

	if (!hwar3) {
		ERROR_GAMEWINDOW_INIT();
		return;
	}

	// 设置全屏
	fullscreen = System::ReadDwordFromReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "autofullscreen");
	System::WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "autofullscreen", fullscreen);
	if (fullscreen) {
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		DWORD last_style = GetWindowLong(hwar3, GWL_STYLE);
		SetWindowLongPtr(hwar3, GWL_STYLE, last_style ^ 0xCF0000);
		SetWindowPos(hwar3, NULL, 0, 0, w, h, SWP_NOZORDER);
	}

	// 设置宽屏
	switch (GetGameInstance()->GetGameVersion()) {
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
	Game::InlineHook((void*)offset, CreateMatrixPerspectiveFov, (void*&)p_orgCreateMatrixPerspectiveFov);
}

void WideScreen::Stop() {}

