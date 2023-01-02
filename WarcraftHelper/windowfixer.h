#pragma once

#include "hook.h"

class WindowFixer : IHook {
public:
	WindowFixer() = default;
	WindowFixer(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	virtual void Start();
	virtual void Stop();
private:
	HANDLE thread = 0;
};
