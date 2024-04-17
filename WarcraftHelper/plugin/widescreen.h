#pragma once

#include "plugin.h"

class WideScreen : IPlugin {
public:
	WideScreen() = default;
	virtual void Start();
	virtual void Stop();
};
