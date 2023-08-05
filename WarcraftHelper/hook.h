#ifndef IHOOK
#define IHOOK

#include "version.h"
#include <stdint.h>

#define ERROR_GAMEDLL_INIT() {MessageBoxA(0, "GameDll was unable to init", "WHelper", 0);}
#define ERROR_GAMEWINDOW_INIT() {MessageBoxA(0, "War3 windows was unable to init", "WHelper", 0);}

static HWND g_hWnd = NULL;

void Hook(void* pOldFuncAddr, void* pNewFuncAddr);
void InlineHook(void* pOldFuncAddr, void* pNewFuncAddr, void*& pCallBackFuncAddr);
void PatchMemory(uintptr_t pAddress, unsigned char* bytes, uint32_t size);
DWORD War3Search(void* pPattern, DWORD dwPatternLen, DWORD dwSearchStart, DWORD dwSearchEnd);
DWORD ReadDwordFromReg(LPCTSTR regPath, LPCTSTR keyName);
BOOL WriteDwordToReg(LPCTSTR regPath, LPCTSTR keyName, DWORD value);
void DetachHook(void* pOldFuncAddr, void* pNewFuncAddr);
void WriteNOP(void* pAddress, DWORD dwCount);
HWND GetWar3Window();

class IHook {
protected:
	DWORD m_GamedllBase;
	Version m_War3Version;
	bool m_Hooked;

public:
	IHook() = default;
	IHook(DWORD gamedllBase, Version war3Version) : m_GamedllBase(gamedllBase), m_War3Version(war3Version), m_Hooked(false) {};
	~IHook() {};

	virtual void Start() = 0;
	virtual void Stop() = 0;
};

#endif
