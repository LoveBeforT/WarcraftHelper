#pragma once

#include "hook.h"

class ShowFPS {
public:
	ShowFPS();
	~ShowFPS();
	void Start(DWORD m_GamedllBase, Version m_War3Version);
	void Stop();
};

