#pragma once

#include "plugin.hpp"

class ReplayView : IPlugin {
public:
	ReplayView() = default;
	void Start();
	void Stop();
};

