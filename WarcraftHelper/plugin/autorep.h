#pragma once

#include "plugin.h"

class AutoRep : IPlugin {
public:
	AutoRep() = default;
	void Start();
	void Stop();
};

