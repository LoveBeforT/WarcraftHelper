#pragma once

#include "plugin.h"

class UnlockFPS : IPlugin {
public:
	UnlockFPS() = default;
	virtual void Start();
	virtual void Stop();
	virtual void ResetD3D();
	virtual void ResetOpenGL();
	virtual void WriteFPSLimit();
};

