#pragma once

#include "plugin.hpp"

class PathFix : IPlugin {
public:
	PathFix() = default;
	virtual void Start();
	virtual void Stop();
};

