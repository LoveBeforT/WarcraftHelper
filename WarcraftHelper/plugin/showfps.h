#pragma once

#include "plugin.h"

class ShowFPS : IPlugin {
public:
	ShowFPS() = default;
	void Start();
	void Stop();
};

