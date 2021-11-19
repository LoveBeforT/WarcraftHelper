#include "CrashFixer.h"

bool CrashFixer_Hooked = false;

int(__fastcall *orgUnknowFunc)(DWORD ecx, DWORD edx);

int __fastcall UnknowFunc(DWORD ecx, DWORD edx) {
	DWORD result = 0;
	try {
		result = orgUnknowFunc(ecx, edx);
	} catch (...) {
		exit(0);
	}
	return result;
}

void CrashFixer::Start(DWORD m_GamedllBase, Version m_War3Version) {
	if (CrashFixer_Hooked) {
		return;
	}
	if (!m_GamedllBase) {
		MessageBox(0, "GameDll≥ı ºªØ ß∞‹", "CrashFixer", 0);
		return;
	}
	DWORD offset = m_GamedllBase;
	switch (m_War3Version) {
	case Version::v124e:
		offset += 0x4C50E0;
		break;
	default:
		return;
	}
	InlineHook((void*)offset, UnknowFunc, (void*&)orgUnknowFunc);
	CrashFixer_Hooked = true;
}

void CrashFixer::Stop() {

}
