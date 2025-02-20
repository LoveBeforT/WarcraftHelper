#pragma once
#pragma once

#include "plugin.hpp"

class ShowHPBar : IPlugin {
public:
	ShowHPBar() = default;
	void Start();
	void Stop();
private:
	HANDLE thread = 0;
};

