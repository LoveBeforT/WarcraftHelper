#include "u9helper.h"

static DWORD u9_map_sig_addr = 0;

bool(__fastcall *orgGetMapSignature)(DWORD *ecx, DWORD edx, DWORD unk00, DWORD unk01, DWORD unk02, DWORD unk03, DWORD unk04, DWORD unk05) = 0;
bool __fastcall GetMapSignature(DWORD *ecx, DWORD edx, DWORD unk00, DWORD unk01, DWORD unk02, DWORD unk03, DWORD unk04, DWORD unk05)
{
	static bool once = false;
	static unsigned char bytes[] = {0x83, 0xF8, 0x04};

	if (!once)
	{
		PatchMemory(u9_map_sig_addr, bytes, 3);
	}
	return orgGetMapSignature(ecx, edx, unk00, unk01, unk02, unk03, unk04, unk05);
}

void U9Helper::Start()
{
	DWORD getMapSignature_addr = this->m_GamedllBase;
	u9_map_sig_addr = this->m_GamedllBase;

	if (this->m_Hooked)
	{
		return;
	}
	this->m_Hooked = true;

	if (!this->m_GamedllBase)
	{
		ERROR_GAMEDLL_INIT();
		return;
	}

	switch (this->m_War3Version)
	{
	case Version::v126a:
		u9_map_sig_addr += 0x7E2FDF;
		getMapSignature_addr += 0x7E2EF0;
		break;
	default:
		return;
	}

	InlineHook((void *)getMapSignature_addr, GetMapSignature, (void *&)orgGetMapSignature);
}

void U9Helper::Stop()
{
	DetachHook((void *)orgGetMapSignature, GetMapSignature);
}
