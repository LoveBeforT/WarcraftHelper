#pragma once

#include "version.h"
#include <stdint.h>

void Hook(void* pOldFuncAddr, void* pNewFuncAddr);
