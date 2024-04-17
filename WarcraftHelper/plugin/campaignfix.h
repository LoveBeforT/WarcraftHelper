#pragma once

#include "plugin.h"

class CampaignFix : IPlugin {
public:
	CampaignFix() = default;
	virtual void Start();
	virtual void Stop();
};

