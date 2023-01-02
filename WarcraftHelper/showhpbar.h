#pragma once
#pragma once

#include "hook.h"

class ShowHPBar : IHook {
public:
	ShowHPBar() = default;
	ShowHPBar(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	void Start();
	void Stop();
private:
	HANDLE thread = 0;
};

