﻿#include "windowfixer.hpp"

bool t_closed = false;

#define IsKeyDown(VK_NAME) ((GetAsyncKeyState(VK_NAME) & 0x8000) ? true:false) 

void ResetWindow(HWND target) {
	if (!target) {
		return;
	}
	RECT rect;
	GetWindowRect(target, &rect);
	// 重新设置窗口大小
	MoveWindow(target, rect.left, rect.top, rect.right - rect.left + 1, rect.bottom - rect.top, false);
	// 恢复窗口大小
	MoveWindow(target, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, false);
}

DWORD __stdcall Listen(LPVOID lpThreadParameter) {
	POINT point;        // 鼠标所在位置
	HWND target;        // 目标窗口句柄
	DWORD origin_pid;
	DWORD target_pid;
	HWND org = GetGameInstance()->GetGameWindow(); // 原窗口句柄
	GetWindowThreadProcessId(org, &origin_pid);
	while (1)
	{
		if (t_closed) {
			ExitThread(0);
		}
		if IsKeyDown(VK_F7)
		{
			GetCursorPos(&point);
			target = WindowFromPoint(point);
			if (target != NULL)
			{
				GetWindowThreadProcessId(target, &target_pid);
				if (origin_pid == target_pid)
				{
					ResetWindow(target);
				}
			}
		}
		Sleep(50);
	}
	return 0;
}

void WindowFixer::Start() {
	this->thread = CreateThread(NULL, NULL, Listen, NULL, NULL, NULL);
}

void WindowFixer::Stop() {
	t_closed = true;
	TerminateThread(this->thread, 0);
}
