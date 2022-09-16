#pragma once

#include "sizebypass.h"
#include "unlockfps.h"
#include "widescreen.h"
#include "windowfixer.h"
#include "autorep.h"
#include "showfps.h"
#include "pathfix.h"
#include "showhpbar.h"


class Helper {
public:
	Helper();
	~Helper();

	void Start();
	void Stop();
private:
	DWORD m_GamedllBase;
	Version m_War3Version;
	bool m_IsWar3;

	SizeBypass *m_SizeBypass;
	WideScreen *m_WideScreen;
	UnlockFPS *m_UnlockFPS;
	WindowFixer *m_WindowFixer;
	AutoRep *m_AutoRep;
	ShowFPS* m_ShowFPS;
	PathFix* m_PathFix;
	ShowHPBar* m_ShowHPBar;

	bool IsWar3();
};

bool InitConsole();