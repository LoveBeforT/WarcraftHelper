#pragma once

#include "plugin.hpp"

class WideScreen : IPlugin {
public:
	WideScreen() = default;
	virtual void Start();
	virtual void Stop();
};
