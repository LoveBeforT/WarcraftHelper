#pragma once

#include "plugin.hpp"

typedef enum {
    GxType_OpenGL,
    GxType_Direct3D,
    GxType_Unknown,
} GxType;

class FpsLimiter : IPlugin {
public:
	FpsLimiter() = default;
	virtual void Start();
	virtual void Stop();
};

