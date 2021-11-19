#include "widescreen.h"
#include <iostream>
#include <process.h>

bool WideScreen_Hooked = false;
HWND g_hWnd = NULL;

struct handle_data {
	unsigned long process_id;
	HWND best_handle;
};

BOOL IsMainWindow(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !IsMainWindow(handle)) {
		return TRUE;
	}
	data.best_handle = handle;
	return FALSE;
}

HWND FindMainWindow(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.best_handle = 0;
	EnumWindows(EnumWindowsCallback, (LPARAM)&data);
	return data.best_handle;
}

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

void WideScreen::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (WideScreen_Hooked) {
		return;
	}
	if (!m_GamedllBase) {
		MessageBox(0, "GameDll初始化失败", "WideScreen", 0);
		return;
	}
	g_hWnd = GetActiveWindow();
	if (!g_hWnd) {
		MessageBox(0, "War3窗口获取失败", "WideScreen", 0);
		return;
	}
	DWORD offset = 0;
	switch (m_War3Version) {
	case Version::v120e:
		offset = 0x0DBD40;
		break;
	case Version::v124e:
		offset = 0x7B6E90;
		break;
	case Version::v127a:
		offset = 0x0D31D0;
		break;
	default:
		return;
	}
	Hook((void*)(offset + m_GamedllBase), CreateMatrixPerspectiveFov);
	WideScreen_Hooked = true;
}

void WideScreen::Stop() {

}

