#pragma once

#include "hook.h"

class UnlockFPS : IHook {
public:
	UnlockFPS() = default;
	UnlockFPS(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	virtual void Start();
	virtual void Stop();
	virtual void ResetD3D();
	virtual void ResetOpenGL();
	virtual void WriteFPSLimit();
};

