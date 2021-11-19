#include "hook.h"
#include <detours.h>

void Hook(void* pOldFuncAddr, void* pNewFuncAddr)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(void*&)pOldFuncAddr, pNewFuncAddr);
	DetourTransactionCommit();
}

void InlineHook(void* pOldFuncAddr, void* pNewFuncAddr, void*& pCallBackFuncAddr)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(void*&)pOldFuncAddr, pNewFuncAddr);
	DetourTransactionCommit();
	pCallBackFuncAddr = pOldFuncAddr;
}

void PatchMemory(uintptr_t pAddress, unsigned char* bytes, uint32_t size)
{
	void* address = reinterpret_cast<void*>(pAddress);

	unsigned long Protection;
	VirtualProtect(address, size, PAGE_READWRITE, &Protection);
	memcpy(address, (const void*)bytes, size);
	VirtualProtect(address, size, Protection, &Protection);
}

DWORD War3_Search(void* pPattern, DWORD dwPatternLen, DWORD dwSearchStart, DWORD dwSearchEnd)
{
	DWORD dwStartAddr = dwSearchStart;
	DWORD dwEndAddr = dwSearchEnd - dwPatternLen;

	while (dwStartAddr < dwEndAddr)
	{
		bool found = true;
		for (DWORD i = 0; i < dwPatternLen; i++)
		{
			char code = *(char*)(dwStartAddr + i);
			if (*((char*)pPattern + i) != code)
			{
				found = false;
				break;
			}
		}
		if (found)
			return dwStartAddr;
		dwStartAddr++;
	}
	return 0;
}
