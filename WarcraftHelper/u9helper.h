#pragma once

#include "hook.h"

class U9Helper : IHook {
public:
	U9Helper() = default;
	U9Helper(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	void Start();
	void Stop();
};

