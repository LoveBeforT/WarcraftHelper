#pragma once

#include "plugin.hpp"

class CampaignFix : IPlugin {
public:
	CampaignFix() = default;
	virtual void Start();
	virtual void Stop();
};

