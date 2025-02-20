#pragma once

#include "plugin.hpp"

class ShowFPS : IPlugin {
public:
	ShowFPS() = default;
	void Start();
	void Stop();
};

