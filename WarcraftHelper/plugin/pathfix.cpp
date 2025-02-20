#include "pathfix.hpp"
#include <time.h>
#include <iostream>

DWORD GetPathName_addr = 0;
DWORD PathCopy_addr = 0;
DWORD PathCopy_size = 0;

int(__fastcall* orgGetPathName)(char*, DWORD, DWORD, float, char*, int) = 0;
int __fastcall GetPathName(char* nameGB2312, DWORD edx, DWORD unk00, float unk01, char* dst, int dst_len) {
	dst[0] = 0;
	DWORD rst =  orgGetPathName(nameGB2312, edx, unk00, unk01, dst, dst_len);

	if (dst) {
		if (!strncmp(dst, "...", 3) || dst[0] == 0) {
			int len = MultiByteToWideChar(CP_ACP, 0, nameGB2312, -1, NULL, 0);
			wchar_t* wstr = new wchar_t[len + 1];
			memset(wstr, 0, len + 1);
			MultiByteToWideChar(CP_ACP, 0, nameGB2312, -1, wstr, len);
			len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
			if (len > dst_len) {
				if (wstr) delete[] wstr;
				return rst;
			}
			WideCharToMultiByte(CP_UTF8, 0, wstr, -1, dst, len, NULL, NULL);
			if (wstr) delete[] wstr;
		}
		DWORD len = strlen(dst);
#ifdef _DEBUG
		printf("map: %s len:%d\n", dst, len);
#endif
	}

	return rst;
}

void PathFix::Start() {
	auto gameAddr = GetGameInstance()->GetGameDllBase();

	switch (GetGameInstance()->GetGameVersion()) {
	case Version::v120e:
		GetPathName_addr = gameAddr + 0x2603E0;
		PathCopy_addr = gameAddr + 0x260489;
		PathCopy_size = 0x31;
		break;
	case Version::v124e:
		GetPathName_addr = gameAddr + 0x5BD690;
		PathCopy_addr = gameAddr + 0x5BD754;
		PathCopy_size = 0x3B;
		break;
	case Version::v126a:
		GetPathName_addr = gameAddr + 0x5BCEF0;
		PathCopy_addr = gameAddr + 0x5BCFB4;
		PathCopy_size = 0x3B;
		break;
	case Version::v127a:
		GetPathName_addr = gameAddr + 0x2A2540;
		PathCopy_addr = gameAddr + 0x2A2619;
		PathCopy_size = 0x35;
		break;
	case Version::v127b:
		GetPathName_addr = gameAddr + 0x2C0160;
		PathCopy_addr = gameAddr + 0x2C0239;
		PathCopy_size = 0x35;
		break;
	default:
		return;
	}
	Game::InlineHook((void*)GetPathName_addr, GetPathName, (void*&)orgGetPathName);
	Game::WriteNOP((void*)PathCopy_addr, PathCopy_size);
}

void PathFix::Stop() {}
