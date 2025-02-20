#pragma once

#include "plugin.hpp"

class UnlockFPS : IPlugin {
public:
	UnlockFPS() = default;
	virtual void Start();
	virtual void Stop();
	virtual void ResetD3D();
	virtual void ResetOpenGL();
	virtual void WriteFPSLimit();
};

