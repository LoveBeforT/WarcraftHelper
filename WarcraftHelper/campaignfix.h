#pragma once

#include "hook.h"

class CampaignFix : IHook {
public:
	CampaignFix() = default;
	CampaignFix(DWORD gamedllBase, Version war3Version) : IHook(gamedllBase, war3Version) {};
	virtual void Start();
	virtual void Stop();
};

