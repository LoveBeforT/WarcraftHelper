
#include "helper.hpp"

#include <windows.h>

#pragma comment(lib, "detours.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    static auto h = Helper();
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
		h.LoadPlugins();
		break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
