#pragma once

#include "version.h"
#include <stdint.h>

void Hook(void* pOldFuncAddr, void* pNewFuncAddr);
void InlineHook(void* pOldFuncAddr, void* pNewFuncAddr, void*& pCallBackFuncAddr);
void PatchMemory(uintptr_t pAddress, unsigned char* bytes, uint32_t size);
DWORD War3_Search(void* pPattern, DWORD dwPatternLen, DWORD dwSearchStart, DWORD dwSearchEnd);