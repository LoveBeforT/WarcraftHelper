#pragma once

#include "hook.h"

class SizeBypass {
public:
	void Start(DWORD m_GamedllBase, Version m_War3Version);
	void Stop();
};

