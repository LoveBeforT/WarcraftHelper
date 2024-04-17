#pragma once

#include "plugin.h"

class ReplayView : IPlugin {
public:
	ReplayView() = default;
	void Start();
	void Stop();
};

