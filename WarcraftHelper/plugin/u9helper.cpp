#include "u9helper.hpp"
#include <mutex>

static DWORD u9MapSigAddr = 0;
static std::once_flag flag;

bool(__fastcall *orgGetMapSignature)(DWORD *ecx, DWORD edx, DWORD unk00, DWORD unk01, DWORD unk02, DWORD unk03, DWORD unk04, DWORD unk05) = 0;
bool __fastcall GetMapSignature(DWORD *ecx, DWORD edx, DWORD unk00, DWORD unk01, DWORD unk02, DWORD unk03, DWORD unk04, DWORD unk05)
{
	std::call_once(flag, [&] {
		static unsigned char bytes[] = {0x83, 0xF8, 0x04};
		Game::PatchMemory(u9MapSigAddr, bytes, 3);
    });
	return orgGetMapSignature(ecx, edx, unk00, unk01, unk02, unk03, unk04, unk05);
}

void U9Helper::Start()
{
	DWORD getMapSignatureAddr = GetGameInstance()->GetGameDllBase();

	u9MapSigAddr = GetGameInstance()->GetGameDllBase();
	switch (GetGameInstance()->GetGameVersion())
	{
	case Version::v126a:
		u9MapSigAddr += 0x7E2FDF;
		getMapSignatureAddr += 0x7E2EF0;
		break;
	default:
		return;
	}

	Game::InlineHook((void *)getMapSignatureAddr, GetMapSignature, (void *&)orgGetMapSignature);
}

void U9Helper::Stop() {}
