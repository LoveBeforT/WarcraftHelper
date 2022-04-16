#include "autorep.h"
#include <time.h>
#include <iostream>

bool AutoRep_Hooked = false;
DWORD SaveRep_addr = 0;


int(__fastcall *orgSaveRep)(DWORD pthis, DWORD unused, const char* path) = 0;

int __fastcall SaveRep(DWORD pthis, DWORD unused, const char* path) {
	DWORD len = strlen(path);
	if (len > 14 && !strncmp(path + len - 14, "LastReplay.w3g", 14)) {
		char whpath[MAX_PATH];
		memset(whpath, 0, MAX_PATH * sizeof(char));
		GetModuleFileName(NULL, whpath, MAX_PATH);
		whpath[strrchr(whpath, '\\') - whpath + 1] = 0;
		char name[MAX_PATH];
		time_t timep;
		time(&timep);
		strftime(name, MAX_PATH, "\\Replay\\WHReplay\\%Y_%m_%d_%H_%M_%S.w3g", localtime(&timep));
		strcat(whpath, name);
		return orgSaveRep(pthis, unused, whpath);
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
		MessageBox(0, "GameDll≥ı ºªØ ß∞‹", "AutoRep", 0);
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
