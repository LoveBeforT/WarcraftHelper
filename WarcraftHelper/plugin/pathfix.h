#pragma once

#include "plugin.h"

class PathFix : IPlugin {
public:
	PathFix() = default;
	virtual void Start();
	virtual void Stop();
};

