#pragma once

#include "hook.h"

class PathFix : IHook {
public:
	PathFix() = default;
	PathFix(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	virtual void Start();
	virtual void Stop();
};

