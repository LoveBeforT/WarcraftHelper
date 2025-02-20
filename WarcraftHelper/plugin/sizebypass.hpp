#pragma once

#include "plugin.hpp"

class SizeBypass : IPlugin {
public:
	SizeBypass() = default;
	virtual void Start();
	virtual void Stop();
};

