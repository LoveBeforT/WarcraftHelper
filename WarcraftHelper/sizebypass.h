#pragma once

#include "hook.h"

class SizeBypass :IHook {
public:
	SizeBypass() = default;
	SizeBypass(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	virtual void Start();
	virtual void Stop();
};

