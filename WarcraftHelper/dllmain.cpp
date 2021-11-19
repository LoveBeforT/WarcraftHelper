// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "helper.h"

bool HelperInited = false;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		if (!HelperInited) {
			HelperInited = true;
			auto h = &Helper();
			h->Start();
		}
		DisableThreadLibraryCalls(hModule);
		break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

