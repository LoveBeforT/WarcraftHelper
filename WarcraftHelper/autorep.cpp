#include "autorep.h"
#include <time.h>
#include <iostream>

bool AutoRep_Hooked = false;
DWORD SaveRep_addr = 0;

void CheckNwgReplay(char* nwgpath, char* reppath) {
	// 判断文件是否存在
	HANDLE hFile = CreateFileA(nwgpath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	DWORD err = GetLastError();
	if (err == ERROR_FILE_NOT_FOUND || hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// 读取文件
	DWORD size = GetFileSize(hFile, NULL);
	DWORD readed = 0;
	char* nwg_data = new char[size];
	if (!nwg_data) {
		return;
	}
	ReadFile(hFile, nwg_data, size, &readed, 0);
	CloseHandle(hFile);
	if (readed != size) {
		delete nwg_data;
		return;
	}

	// 查找到header
	std::string buf(nwg_data, size);
	std::string dest(WarcraftRecordHeader, strlen(WarcraftRecordHeader));
	size_t pos = buf.find(dest);
	if (pos == std::string::npos) {
		delete nwg_data;
		return;
	}

	// 写入文件
	hFile = CreateFile(reppath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE ||
		SetFilePointer(hFile, 0, NULL, FILE_BEGIN)
		) {
		CloseHandle(hFile);
		delete nwg_data;
		return;
	}
	DWORD writtenbytes = 0;
	WriteFile(hFile, nwg_data + pos, size - pos, &writtenbytes, 0);
	delete nwg_data;
	CloseHandle(hFile);
}

int(__fastcall *orgSaveRep)(DWORD pthis, DWORD unused, const char* path) = 0;

int __fastcall SaveRep(DWORD pthis, DWORD unused, const char* path) {
	DWORD len = strlen(path);
	if (len > 14 && !strncmp(path + len - 14, "LastReplay.w3g", 14)) {
		char whpath[MAX_PATH];
		char repname[MAX_PATH];
		char nwgpath[MAX_PATH];
		DWORD ret = 0;
		DWORD replen = 0;
		time_t timep;

		memset(whpath, 0, MAX_PATH * sizeof(char));
		memset(nwgpath, 0, MAX_PATH * sizeof(char));
		memset(repname, 0, MAX_PATH * sizeof(char));

		time(&timep);
		GetModuleFileName(NULL, whpath, MAX_PATH);
		whpath[strrchr(whpath, '\\') - whpath + 1] = 0;
		strftime(repname, MAX_PATH, "\\Replay\\WHReplay\\%Y_%m_%d_%H_%M_%S.w3g", localtime(&timep));
		strcat(whpath, repname);
		replen = strlen(whpath);
		if (replen >= MAX_PATH) {
			// 溢出了
			replen = MAX_PATH - 1;
			whpath[replen] = 0;
		}
		ret = orgSaveRep(pthis, unused, whpath);

		// 如果是官方平台，那么录像会变成nwg格式，需要转换成默认的w3g
		strncpy(nwgpath,whpath, replen-3);
		strcat(nwgpath, "nwg");
		CheckNwgReplay(nwgpath, whpath);
		return ret;
	}
	return orgSaveRep(pthis, unused, path);
}

AutoRep::AutoRep() {}
AutoRep::~AutoRep() {}

void AutoRep::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (AutoRep_Hooked) {
		return;
	}
	AutoRep_Hooked = true;
	if (!m_GamedllBase) {
		MessageBox(0, "GameDll初始化失败", "AutoRep", 0);
		return;
	}
	switch (m_War3Version) {
	case Version::v120e:
		SaveRep_addr = m_GamedllBase + 0x28D320;
		break;
	case Version::v124e:
		SaveRep_addr = m_GamedllBase + 0x53EE60;
		break;
	case Version::v127a:
		SaveRep_addr = m_GamedllBase + 0x3122C0;
		break;
	default:
		return;
	}
	InlineHook((void*)SaveRep_addr, SaveRep, (void*&)orgSaveRep);
}

void AutoRep::Stop() {
	DetachHook((void*)orgSaveRep, SaveRep);
}
