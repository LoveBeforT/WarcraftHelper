#include "hook.h"
#include <detours.h>

void Hook(void* pOldFuncAddr, void* pNewFuncAddr)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(void*&)pOldFuncAddr, pNewFuncAddr);
	DetourTransactionCommit();
}
