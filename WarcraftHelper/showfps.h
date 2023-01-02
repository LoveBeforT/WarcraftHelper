#pragma once

#include "hook.h"

class ShowFPS : IHook {
public:
	ShowFPS() = default;
	ShowFPS(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	void Start();
	void Stop();
};

