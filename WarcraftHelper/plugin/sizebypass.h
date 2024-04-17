#pragma once

#include "plugin.h"

class SizeBypass : IPlugin {
public:
	SizeBypass() = default;
	virtual void Start();
	virtual void Stop();
};

