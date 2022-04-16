#include "windowfixer.h"

bool t_closed = false;
extern HWND g_hWnd;

#define IsKeyDown(VK_NAME) ((GetAsyncKeyState(VK_NAME) & 0x8000) ? true:false) 

DWORD __stdcall Listen(LPVOID lpThreadParameter) {
	POINT point;        // 鼠标所在位置
	HWND target;        // 目标窗口句柄
	while (1)
	{
		if (t_closed) {
			ExitThread(0);
		}
		if IsKeyDown(VK_F7)
		{
			GetCursorPos(&point);
			target = WindowFromPoint(point);
			if (target != NULL && target == g_hWnd) {
				RECT rect;
				GetWindowRect(target, &rect);
				// 重新设置窗口大小
				MoveWindow(target, rect.left, rect.top, rect.right - rect.left-1, rect.bottom - rect.top, true);
				// 恢复窗口大小
				MoveWindow(target, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
			}
		}
		Sleep(50);
	}
	return 0;
}

WindowFixer::WindowFixer() {}
WindowFixer::~WindowFixer() {}

void WindowFixer::Start() {
	this->thread = CreateThread(NULL, NULL, Listen, NULL, NULL, NULL);
}

void WindowFixer::Stop() {
	t_closed = true;
	Sleep(51);
}
