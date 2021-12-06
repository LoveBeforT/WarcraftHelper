#pragma once

#include "hook.h"

class WindowFixer {
public:
	WindowFixer();
	~WindowFixer();
	void Start();
	void Stop();
private:
	HANDLE thread;
};

