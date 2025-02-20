#pragma once

#include "plugin.hpp"

class AutoRep : IPlugin {
public:
	AutoRep() = default;
	void Start();
	void Stop();
};

