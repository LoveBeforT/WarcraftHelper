#include "fpslimiter.hpp"
#include "config/config.hpp"
#include <time.h>
#include <iostream>
#include <chrono>
#include <thread>

static double g_frameMicro = 0.0;

DWORD (__fastcall *orgDrawCall)(DWORD ecx, DWORD edx, DWORD unk) = 0;
DWORD __fastcall DrawCall(DWORD ecx, DWORD edx, DWORD unk)
{
    static std::chrono::steady_clock::time_point g_lastPresentMilli = std::chrono::high_resolution_clock::now();

    if (GetConfig()->m_fpsLimit && GetConfig()->m_targetFps > 1) {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        std::chrono::steady_clock::time_point currentMilli = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration<double, std::micro>(currentMilli - g_lastPresentMilli);

        if (diff.count() < g_frameMicro)
		{
			std::this_thread::sleep_for(std::chrono::microseconds((long)(g_frameMicro - diff.count())));
		}
        g_lastPresentMilli = currentMilli;  
    }
    return orgDrawCall(ecx, edx, unk);
}

void FpsLimiter::Start() {
	DWORD gxDrawAddr = 0;
	DWORD gxTypeAddr = 0;
	GxType gxType = GxType_Unknown;
	auto gameAddr = GetGameInstance()->GetGameDllBase();

	switch (GetGameInstance()->GetGameVersion()) {
	case Version::v127a:
        gxDrawAddr = gameAddr + 0xED080;
        gxTypeAddr = gameAddr + 0xB665C8;
		break;
	case Version::v127b:
        gxDrawAddr = gameAddr + 0x140950;
        gxTypeAddr = gameAddr + 0xCE3D50;
		break;
	default:
		return;
	}

    if (gxTypeAddr == 0 || gxDrawAddr == 0) {
        return;
    }

    gxType = *(GxType *)gxTypeAddr;

    if (gxType != GxType_Direct3D) {
        return;
    }

    g_frameMicro = 1000000.0 / GetConfig()->m_targetFps;
	Game::InlineHook((void*)gxDrawAddr, DrawCall, (void*&)orgDrawCall);
}

void FpsLimiter::Stop() {}
