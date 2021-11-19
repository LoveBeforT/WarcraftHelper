#pragma once

#include "sizebypass.h"
#include "unlockfps.h"
#include "widescreen.h"
#include "CrashFixer.h"
#include "windowfixer.h"
#include "autorep.h"

class Helper {
public:
	Helper();

	void Start();
	void Stop();
private:
	DWORD m_GamedllBase;
	Version m_War3Version;

	SizeBypass *m_SizeBypass;
	WideScreen *m_WideScreen;
	UnlockFPS *m_UnlockFPS;
	CrashFixer *m_CrashFixer;
	WindowFixer *m_WindowFixer;
	AutoRep *m_AutoRep;

	bool IsWar3();
};

