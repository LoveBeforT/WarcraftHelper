#pragma once

#include "hook.h"

class ReplayView : IHook {
public:
	ReplayView() = default;
	ReplayView(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	void Start();
	void Stop();
};

