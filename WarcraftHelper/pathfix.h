#pragma once

#include "hook.h"

class PathFix {
public:
	PathFix();
	~PathFix();
	void Start(DWORD m_GamedllBase, Version m_War3Version);
	void Stop();
};

