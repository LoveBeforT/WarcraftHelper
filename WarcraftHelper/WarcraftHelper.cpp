#include <iostream>
#include <direct.h>
#define _AFXDLL
#include <afx.h>
#include <strsafe.h>
#include <psapi.h>
#include "WarcraftHelper.h"
#include "singleton.h"
#include "INIreader.h"
#include "MinHook.h"
#include "verinfo.h"
//加载HOOK库
#include "detours.h"
#pragma comment(lib, "detours.lib" )
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "libMinHook.x86.lib")
#pragma warning(disable: 4995 )

#define BUFSIZE 512

int gameversion = 23;
BOOL oldversion = FALSE;
BOOL ingame = FALSE;
HANDLE bypassthread;
HANDLE BloodThread;
HWND g_hWnd,Timer;
HMODULE gamedll;
auto& helper = get_helper();
char WindowsText[64];
int flag;
//原获取文件大小
//typedef DWORD (WINAPI *OldGetFileSize)
//(
//	HANDLE file,
//	LPDWORD lpFileSizeHigh
//	);
typedef DWORD(__stdcall * pGetFileSize) (HANDLE file, LPDWORD lpFileSizeHigh);
pGetFileSize orgGetFileSize = NULL;
pGetFileSize ptrGetFileSize = NULL;
//加载HOOK
void WarcraftHelper::InitHelper() {
	HANDLE file = CreateFile("WarcraftHelper.ini", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY, NULL);
	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
		char  buffer[1024];
		_getcwd(buffer, 1024);
		wsprintf(IniFile, "%s\\WarcraftHelper.ini", buffer);
		ByPass = GetPrivateProfileInt("Helper", "SizeByPass", 0 , IniFile);
		WideScreen = GetPrivateProfileInt("Helper", "WideScreen", 0, IniFile);
		BloodBarFri = GetPrivateProfileInt("Helper", "BloodBarFri", 0, IniFile);
		BloodBarEny = GetPrivateProfileInt("Helper", "BloodBarEny", 0, IniFile);
		UnlockFPS = GetPrivateProfileInt("Helper", "UnlockFPS", 0, IniFile);
		g_hWnd = GetActiveWindow();
	}
	if (ByPass) {
		MapSize_ByPass();
	}
	if (WideScreen) {
		ScreenWideSupport();
	}
	if (BloodBarFri || BloodBarEny) {
		g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, g_hInstanceHandle, 0);
		BloodThread = CreateThread(0, 0, InitBlood, 0, 0, 0);
	}
	if (UnlockFPS) {
		UnlockFps();
	}
}

//卸载HOOK
void WarcraftHelper::detach() {
	//MessageBoxA(0, "Detached!", "WarcraftHelper", 0);
	if (address_CreateMatrixPerspectiveFov)
		DetourUninstall(&address_CreateMatrixPerspectiveFov, (uintptr_t)CreateMatrixPerspectiveFov_proxy);

	if (address_BuildHPBars)
		DetourUninstall(&address_BuildHPBars, (uintptr_t)BuildHPBars_proxy);

	if (helper.ByPass) {
		MH_Uninitialize();
		TerminateThread(bypassthread, 0);
	}
	if (BloodBarFri || BloodBarEny) {
		if (g_hHook)
		{
			UnhookWindowsHookEx(g_hHook);
		}
		KillTimer(Timer, 1);
		TerminateThread(BloodThread, 0);
	}
}

//安装detours
bool WarcraftHelper::DetourInstall(uintptr_t* pointer_ptr, uintptr_t detour)
{
	if (DetourTransactionBegin() == NO_ERROR)
	{
		if (DetourUpdateThread(GetCurrentThread()) == NO_ERROR)
		{
			if (DetourAttach((PVOID*)pointer_ptr, (PVOID)detour) == NO_ERROR)
			{
				if (DetourTransactionCommit() == NO_ERROR)
				{
					return true;
				}
			}
		}
		DetourTransactionAbort();
	}
	return false;
}

//卸载detours
bool WarcraftHelper::DetourUninstall(uintptr_t* pointer_ptr, uintptr_t detour)
{
	if (DetourTransactionBegin() == NO_ERROR)
	{
		if (DetourUpdateThread(GetCurrentThread()) == NO_ERROR)
		{
			if (DetourDetach((PVOID*)pointer_ptr, (PVOID)detour) == NO_ERROR)
			{
				if (DetourTransactionCommit() == NO_ERROR)
				{
					return true;
				}
			}
		}
		DetourTransactionAbort();
	}
	return false;
}
//按键HOOK
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char str[30];
	::GetWindowText(::GetForegroundWindow(), str, 30);
	if (strcmp(str, "Warcraft III") == 0)//"←_← - 记事本")==0)    //
	{
		if (nCode == HC_ACTION)
		{
			LPKBDLLHOOKSTRUCT lpk = (LPKBDLLHOOKSTRUCT)lParam;
			//[
			if (helper.BloodBarFri && 
				lpk->vkCode == 219)
			{
				if (wParam == WM_KEYDOWN)
				{
					keybd_event(219, 0, 0, 0);
				}
				return 1;
			}
			//]
			if (helper.BloodBarEny &&
				lpk->vkCode == 221)
			{
				if (wParam == WM_KEYDOWN)
				{
					keybd_event(221, 0, 0, 0);
				}
				return 1;
			}
		}
	}
	LRESULT lRetVal = CallNextHookEx(helper.g_hHook, nCode, wParam, 0);
	return lRetVal;
}
void PatchMemory(uintptr_t pAddress, unsigned char* bytes, uint32 size)
{
	void* address = reinterpret_cast<void*>(pAddress);

	unsigned long Protection;
	VirtualProtect(address, size, PAGE_READWRITE, &Protection);
	memcpy(address, (const void*)bytes, size);
	VirtualProtect(address, size, Protection, &Protection);
}
void WarcraftHelper::UnlockFps() {
	uintptr_t address_GameBase = (uintptr_t)GetModuleHandleW(L"Game.dll");
	uintptr_t address_LockFPS = 0;
	const Version gameVersion = GetGameVersion();
	switch (gameVersion)
	{
	case Version::v127a:
		address_LockFPS = address_GameBase + 0x5FCFB;
		break;
	case Version::v126a:
		address_LockFPS = address_GameBase + 0x62D7FB;
		break;
	default:
		return;
	}
	// Unlock FPS
	unsigned char bytes0[] = { 0x90,0x90 };
	PatchMemory(address_LockFPS-4, bytes0, 2);
	unsigned char bytes1[] = { 0xFF };
	PatchMemory(address_LockFPS, bytes1, 1);
}
//初始化显血
DWORD __stdcall InitBlood(LPVOID) {
	::GetWindowText(g_hWnd, WindowsText, 50);
	if (strcmp(WindowsText, "Warcraft III") == 0)
		Timer = g_hWnd;
	SetTimer(Timer, 1, 500, NULL);
	helper.OnTimer(1);
	return 0;
}
//显血
void WarcraftHelper::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		g_hWnd = ::GetForegroundWindow();
		::GetWindowText(g_hWnd, WindowsText, 50);
		if (strcmp(WindowsText, "Warcraft III") == 0)
		{
			if (flag == 0)
			{
				if(BloodBarFri)	keybd_event(219, 0, 0, 0);
				if(BloodBarEny)	keybd_event(221, 0, 0, 0);
				flag = 1;
			}
		}
		else
		{
			flag = 0;
		}

	}
	WarcraftHelper::OnTimer(nIDEvent);
}

//魔兽宽屏支持
bool WarcraftHelper::ScreenWideSupport() {
	//获取当前游戏内存基址
	uintptr_t address_GameBase = (uintptr_t)GetModuleHandleW(L"Game.dll");
	//获取窗口
	g_hWnd = GetActiveWindow();
	const Version gameVersion = GetGameVersion();
	switch (gameVersion)
	{
	case Version::v128f:
		address_CreateMatrixPerspectiveFov = 0x1554F0;
		address_BuildHPBars = 0x3C6700;
		break;
	case Version::v128e:
		address_CreateMatrixPerspectiveFov = 0x1553B0;
		address_BuildHPBars = 0x3C6560;
		break;
	case Version::v128c:
		address_CreateMatrixPerspectiveFov = 0x12B730;
		address_BuildHPBars = 0x39C670;
		break;
	case Version::v128a:
		address_CreateMatrixPerspectiveFov = 0x129150;
		address_BuildHPBars = 0x399F20;
		break;
	case Version::v127b:
		address_CreateMatrixPerspectiveFov = 0x126E30;
		address_BuildHPBars = 0x3925F0;
		break;
	case Version::v127a:
		address_CreateMatrixPerspectiveFov = 0x0D31D0;
		address_BuildHPBars = 0x374E60;
		break;
	case Version::v126a:
		address_CreateMatrixPerspectiveFov = 0x7B66F0;
		address_BuildHPBars = 0x379A30;
		break;
	case Version::v124e:
		address_CreateMatrixPerspectiveFov = 0x7B6E90;
		address_BuildHPBars = 0x37A570;
		break;
	case Version::v123a:
		address_CreateMatrixPerspectiveFov = 0x7A7BA0;
		address_BuildHPBars = 0x37A3F0;
		break;
	default:
		address_CreateMatrixPerspectiveFov = 0;
		address_BuildHPBars = 0;
		return false;
	}
	if (address_CreateMatrixPerspectiveFov)
	{
		address_CreateMatrixPerspectiveFov += address_GameBase;
		DetourInstall(&address_CreateMatrixPerspectiveFov, (uintptr_t)CreateMatrixPerspectiveFov_proxy);
	}

	if (address_BuildHPBars)
	{
		address_BuildHPBars += address_GameBase;
		DetourInstall(&address_BuildHPBars, (uintptr_t)BuildHPBars_proxy);
	}
	return true;
}
//游戏
BOOL IsGame() // my offset + public
{
	if (oldversion)
		return FALSE;
	if (gameversion == 24)
		return *(int*)((DWORD)gamedll + 0xAE64D8) > 0;
	return *(int*)((DWORD)gamedll + 0xACF678) > 0 || *(int*)((DWORD)gamedll + 0xAB62A4) > 0;
}
//躲避HACK检测
DWORD __stdcall DisableIngameHookThread(LPVOID)
{
	while (TRUE)
	{
		if (IsGame())
		{
			if (!ingame)
			{
				ingame = TRUE;
				MH_DisableHook(orgGetFileSize);
			}
		}
		else
		{
			if (ingame)
			{
				MH_EnableHook(orgGetFileSize);
			}

		}
		Sleep(100);
	}

	return 0;
}
//魔兽地图大小限制破解
bool WarcraftHelper::MapSize_ByPass() {
	MH_Initialize();
	TCHAR lpFilePATH[MAX_PATH];
	::GetModuleFileName(NULL, lpFilePATH, MAX_PATH);
	CString tmp;
	tmp.Format("%s", lpFilePATH);
	DWORD loc = tmp.ReverseFind('\\');
	if (strcmp(lpFilePATH + loc + 1, "war3.exe")) {
		return false;
	}
	// 获取 Kernel32.dll
	HMODULE krn32 = GetModuleHandle("Kernel32.dll");
	if (!krn32)
	{
		return FALSE;
	}
	// Get GetFileSize func address
	orgGetFileSize = (pGetFileSize)GetProcAddress(krn32, "GetFileSize");
	if (!orgGetFileSize)
	{
		return FALSE;
	}
	//pfGetFileSize = (FARPROC)pGetFileSize;
	MH_CreateHook(orgGetFileSize, &myGetFileSize, reinterpret_cast<void**>(&ptrGetFileSize));
	gamedll = GetModuleHandle("Game.dll");
	if (!gamedll)
	{
		return FALSE;
	}
	// file / module version info
	CFileVersionInfo gdllver;
	gdllver.Open(gamedll);
	// Game.dll version (1.XX)
	gameversion = gdllver.GetFileVersionMinor();
	oldversion = gameversion < 24;
	gdllver.Close();
	bypassthread = CreateThread(0, 0, DisableIngameHookThread, 0, 0, 0);
	MH_EnableHook(orgGetFileSize);
	return true;
}

//获取魔兽当前版本
Version WarcraftHelper::GetGameVersion() {
	DWORD dwHandle;
	DWORD sz = GetFileVersionInfoSizeW(L"Game.dll", &dwHandle);
	if (sz == 0)
	{
		return Version::unknown;
	}
	char Data[2048];
	if (!GetFileVersionInfo("Game.dll", 0, sizeof(Data), Data))
	{
		return Version::unknown;
	}
	VS_FIXEDFILEINFO *FileInfo;
	UINT nBytes = 0;
	VerQueryValue(Data, "\\", (void**)&FileInfo, &nBytes);
	uint32 buildVersion = FileInfo->dwFileVersionLS & 0xffff;
	return static_cast<Version>(buildVersion);
}

//画面矩阵修改
void __fastcall CreateMatrixPerspectiveFov_proxy(uint32 outMatrix, uint32 unused, float fovY, float aspectRatio, float nearZ, float farZ)
{
	RECT r;
	if (GetWindowRect(g_hWnd, &r))
	{
		float width = float(r.right - r.left);
		float rHeight = 1.0f / (r.bottom - r.top);
		helper.g_fWideScreenMul = width * rHeight * 0.75f; // (width / height) / (4.0f / 3.0f)
	}

	float yScale = 1.0f / tan(fovY * 0.5f / sqrt(aspectRatio * aspectRatio + 1.0f));
	float xScale = yScale / (aspectRatio * helper.g_fWideScreenMul);

	*(float*)(outMatrix) = xScale;
	*(float*)(outMatrix + 16) = 0.0f;
	*(float*)(outMatrix + 32) = 0.0f;
	*(float*)(outMatrix + 48) = 0.0f;

	*(float*)(outMatrix + 4) = 0.0f;
	*(float*)(outMatrix + 20) = yScale;
	*(float*)(outMatrix + 36) = 0.0f;
	*(float*)(outMatrix + 52) = 0.0f;

	*(float*)(outMatrix + 8) = 0.0f;
	*(float*)(outMatrix + 24) = 0.0f;
	*(float*)(outMatrix + 40) = (nearZ + farZ) / (farZ - nearZ);
	*(float*)(outMatrix + 56) = (-2.0f * nearZ * farZ) / (farZ - nearZ);

	*(float*)(outMatrix + 12) = 0.0f;
	*(float*)(outMatrix + 28) = 0.0f;
	*(float*)(outMatrix + 44) = 1.0f;
	*(float*)(outMatrix + 60) = 0.0f;
}

//修改生命值Bar
void __fastcall BuildHPBars_proxy(uint32 a1, uint32 unused, uint32 a2, uint32 a3)
{
	((void(__fastcall *)(uint32, uint32, uint32, uint32))helper.address_BuildHPBars)(a1, unused, a2, a3);

	uint32 pHPBarFrame = *((uint32*)a1 + 3);

	if (pHPBarFrame)
		*((float*)pHPBarFrame + 22) /= helper.g_fWideScreenMul;
}
//获取文件大小函数
DWORD  __stdcall myGetFileSize(HANDLE file, LPDWORD lpFileSizeHigh)
{
	DWORD retval = ptrGetFileSize(file, lpFileSizeHigh);
	if (retval > 0)
	{
		char * filenamex = GetFileNameFromHandle(file);
		if (!filenamex)
		{
			delete[] filenamex;
			return retval;
		}
		int strlenx = strlen(filenamex);
		if (strlenx > 5 && helper.IsMapFile(filenamex[strlenx - 4], filenamex[strlenx - 3], filenamex[strlenx - 2], filenamex[strlenx - 1]))
		{
			if (!oldversion)
			{
				if (retval > 0x7FFFFF)
				{
					retval = 0x7FFFFF;
				}
			}
			else
			{
				if (retval > 0x2FFFFF)
				{
					retval = 0x2FFFFF;
				}
			}
		}
		delete[] filenamex;
	}
	return retval;
}
//判断是不是地图文件
BOOL WarcraftHelper::IsMapFile(char x1, char x2, char x3, char x4)
{
	char fileext[5];
	memset(fileext, 0, 5);
	fileext[0] = x1;
	fileext[1] = x2;
	fileext[2] = x3;
	fileext[3] = x4;
	return _stricmp(fileext, ".w3x") == 0 || _stricmp(fileext, ".w3m") == 0;
}
//HANDLE转文件名
char * GetFileNameFromHandle(HANDLE hFile)
{
	TCHAR * pszFilename = new TCHAR[MAX_PATH + 1];
	HANDLE hFileMap;
	// Create a file mapping object.
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		1,
		NULL);
	if (hFileMap)
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
		if (pMem)
		{
			if (GetMappedFileName(GetCurrentProcess(),
				pMem,
				pszFilename,
				MAX_PATH))
			{
				// Translate path with device name to drive letters.
				TCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';
				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;
					do
					{
						// Copy the drive letter to the template string
						*szDrive = *p;
						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = _tcslen(szName);
							if (uNameLen < MAX_PATH)
							{
								bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
									&& *(pszFilename + uNameLen) == _T('\\');
								if (bFound)
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH];
									StringCchPrintf(szTempFile,
										MAX_PATH,
										TEXT("%s%s"),
										szDrive,
										pszFilename + uNameLen);
									StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
								}
							}
						}
						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			UnmapViewOfFile(pMem);
		}
		CloseHandle(hFileMap);
	}
	return pszFilename;
}

//获取当前运行实例
WarcraftHelper& get_helper()
{
	return base::singleton<WarcraftHelper>::instance();
}