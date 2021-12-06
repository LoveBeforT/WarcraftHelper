#pragma once

#include "hook.h"

class AutoRep {
public:
	AutoRep();
	~AutoRep();
	void Start(DWORD m_GamedllBase, Version m_War3Version);
	void Stop();
};

