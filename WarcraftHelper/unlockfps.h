#pragma once

#include "hook.h"

class UnlockFPS {
public:
	UnlockFPS(DWORD base, Version version);

	void Start();
	void Stop();
private:
	DWORD m_GamedllBase;
	Version m_War3Version;
	bool m_Hooked;
};

