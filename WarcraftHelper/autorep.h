#pragma once

#include "hook.h"

class AutoRep : IHook {
public:
	AutoRep() = default;
	AutoRep(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	void Start();
	void Stop();
};

static char* WarcraftRecordHeader = "Warcraft III recorded game";
