#pragma once

#include "hook.h"

class WideScreen {
public:
	WideScreen();
	~WideScreen();
	void Start(DWORD m_GamedllBase, Version m_War3Version);
	void Stop();
};

