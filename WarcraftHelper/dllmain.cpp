// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "WarcraftHelper.h"
WarcraftHelper Helper;

#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	auto& helper = get_helper();
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		helper.InitHelper();
		break;
    case DLL_PROCESS_DETACH:
		helper.detach();
        break;
    }
    return TRUE;
}

