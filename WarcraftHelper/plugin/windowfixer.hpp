#pragma once

#include "plugin.hpp"

class WindowFixer : IPlugin {
public:
	WindowFixer() = default;
	virtual void Start();
	virtual void Stop();
private:
	HANDLE thread = 0;
};
