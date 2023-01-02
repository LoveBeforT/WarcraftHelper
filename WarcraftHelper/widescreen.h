#pragma once

#include "hook.h"

class WideScreen : IHook {
public:
	WideScreen() = default;
	WideScreen(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	virtual void Start();
	virtual void Stop();
};
