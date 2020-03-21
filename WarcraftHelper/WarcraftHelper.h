#pragma once
#define _AFXDLL
#include "War3Versions.h"
#include <afxwin.h>
using uint32 = unsigned int;

#define SIZE 6
class WarcraftHelper {
public:
	//变量
	uintptr_t address_CreateMatrixPerspectiveFov = 0;			//矩阵
	uintptr_t address_BuildHPBars = 0;							//生命值
	//uintptr_t filesizeaddr = 0;								//文件大小函数基址
	int BloodBarFri = 0;										//友军显血
	int BloodBarEny = 0;										//敌军显血
	int UnlockFPS = 0;											//FPS解锁
	float g_fWideScreenMul = 1.0f;
	HHOOK g_hHook;
	HINSTANCE g_hInstanceHandle = NULL;
	//加载卸载函数
	void InitHelper();
	void detach();
	BOOL IsMapFile(char x1, char x2, char x3, char x4);
	bool DetourInstall(uintptr_t* pointer_ptr, uintptr_t detour);
	bool DetourUninstall(uintptr_t* pointer_ptr, uintptr_t detour);
	void WarcraftHelper::UnlockFps();
	afx_msg void OnTimer(UINT nIDEvent);
private:
	//配置数据
	int ByPass = 0;												//过大小检测
	int WideScreen = 0;											//宽屏支持
	//函数
	bool MapSize_ByPass();
	bool ScreenWideSupport();
	Version GetGameVersion();
	//void ReadConfig(HANDLE file);
};

//extern WarcraftHelper helper;

void __fastcall CreateMatrixPerspectiveFov_proxy(uint32 outMatrix, uint32 unused, float fovY, float aspectRatio, float nearZ, float farZ);
void __fastcall BuildHPBars_proxy(uint32 a1, uint32 unused, uint32 a2, uint32 a3);
DWORD  __stdcall myGetFileSize(HANDLE file, LPDWORD lpFileSizeHigh);
char * GetFileNameFromHandle(HANDLE hFile);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD __stdcall InitBlood(LPVOID);
WarcraftHelper& get_helper();