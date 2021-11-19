#pragma once

#include "hook.h"

class SizeBypass {
public:
	SizeBypass(DWORD base, Version version);

	void Start();
	void Stop();
private:
	DWORD m_GamedllBase;
	Version m_War3Version;
	HMODULE m_kernel;
	bool m_Hooked;
};

