#include "pathfix.h"
#include <time.h>
#include <iostream>

bool PathFix_Hooked = false;
DWORD GetPathName_addr = 0;
DWORD PathCopy_addr = 0;
DWORD PathCopy_size = 0;

int(__fastcall* orgGetPathName)(char*, DWORD, DWORD, float, char*, int) = 0;
int __fastcall GetPathName(char* nameGB2312, DWORD edx, DWORD unk00, float unk01, char* dst, int unk02) {
	dst[0] = 0;
	DWORD rst =  orgGetPathName(nameGB2312, edx, unk00, unk01, dst, unk02);
	if (dst) {
		if (!strncmp(dst, "...", 3) || dst[0] == 0) {
			int len = MultiByteToWideChar(CP_ACP, 0, nameGB2312, -1, NULL, 0);
			wchar_t* wstr = new wchar_t[len + 1];
			memset(wstr, 0, len + 1);
			MultiByteToWideChar(CP_ACP, 0, nameGB2312, -1, wstr, len);
			len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, 0, wstr, -1, dst, len, NULL, NULL);
			if (wstr) delete[] wstr;
		}
	}

	return rst;
}

PathFix::PathFix() {}
PathFix::~PathFix() {}

void PathFix::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (PathFix_Hooked) {
		return;
	}
	PathFix_Hooked = true;
	if (!m_GamedllBase) {
		MessageBoxA(0, "GameDll≥ı ºªØ ß∞‹", "AutoRep", 0);
		return;
	}
	switch (m_War3Version) {
	case Version::v120e:
		GetPathName_addr = m_GamedllBase + 0x2603E0;
		PathCopy_addr = m_GamedllBase + 0x260489;
		PathCopy_size = 0x31;
		break;
	case Version::v124e:
		GetPathName_addr = m_GamedllBase + 0x5BD690;
		PathCopy_addr = m_GamedllBase + 0x5BD754;
		PathCopy_size = 0x3B;
		break;
	case Version::v127a:
		GetPathName_addr = m_GamedllBase + 0x2A2540;
		PathCopy_addr = m_GamedllBase + 0x2A2619;
		PathCopy_size = 0x35;
		break;
	default:
		return;
	}
	InlineHook((void*)GetPathName_addr, GetPathName, (void*&)orgGetPathName);
	WriteNOP((void*)PathCopy_addr, PathCopy_size);
}

void PathFix::Stop() {
	DetachHook((void*)orgGetPathName, GetPathName);
}
