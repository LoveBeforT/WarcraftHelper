#include "sizebypass.hpp"
#include "config/config.hpp"

void SizeBypass::Start() {
	DWORD bytes_size = 0;
	DWORD addr1 = GetGameInstance()->GetGameDllBase();
	DWORD addr2 = GetGameInstance()->GetGameDllBase();
	DWORD addr3 = GetGameInstance()->GetGameDllBase();

    if (!GetConfig()->m_unlockMapSize) {
        return;
    }

	switch (GetGameInstance()->GetGameVersion()) {
	case Version::v120e:
		addr1 += 0x6DD56D;
		bytes_size = 7;
		addr2 += 0x6EDC9E;
		addr3 += 0x6F89B1;
		break;
	case Version::v124e:
		addr1 += 0x657F83;
		bytes_size = 11;
		addr2 += 0x66F51E;
		addr3 += 0x67F400;
		break;
	case Version::v126a:
		addr1 += 0x6577E3;
		bytes_size = 11;
		addr2 += 0x66ED7E;
		addr3 += 0x67EC60;
		break;
	case Version::v127a:
		addr1 += 0x84F530;
		bytes_size = 11;
		addr2 += 0x85F9B6;
		addr3 += 0x872666;
		break;
	case Version::v127b:
		addr1 += 0x978E20;
		bytes_size = 11;
		addr2 += 0x9892A6;
		addr3 += 0x99BF66;
		break;
	default:
		return;
	}

	unsigned char bytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	Game::PatchMemory(addr1, bytes, bytes_size);
	Game::PatchMemory(addr2, bytes, 11);
	Game::PatchMemory(addr3, bytes, 11);
}

void SizeBypass::Stop() {}
