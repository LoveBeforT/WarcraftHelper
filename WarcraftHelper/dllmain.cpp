// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "helper.h"

#pragma comment(lib, "detours.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    auto h = &Helper();
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		h->Start();
		break;
    case DLL_PROCESS_DETACH:
        h->Stop();
        break;
    }
    return TRUE;
}
