#pragma once
#pragma once

#include "hook.h"

class ShowHPBar {
public:
	ShowHPBar();
	~ShowHPBar();
	void Start(DWORD m_GamedllBase, Version m_War3Version);
	void Stop();
private:
	HANDLE thread;
};

