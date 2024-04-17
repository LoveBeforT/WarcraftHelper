#pragma once

#include "plugin.h"

class WindowFixer : IPlugin {
public:
	WindowFixer() = default;
	virtual void Start();
	virtual void Stop();
private:
	HANDLE thread = 0;
};
