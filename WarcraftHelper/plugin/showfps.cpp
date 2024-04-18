#include "showfps.h"
#include "config/config.h"

void ShowFPS::Start() {
	unsigned char bytes[] = { 0x01 };
	DWORD offset = GetGameInstance()->GetGameDllBase();

    if (!GetConfig()->m_showFps) {
        return;
    }

	switch (GetGameInstance()->GetGameVersion()) {
	case Version::v124e:
		offset += 0x34B248;
		break;
	case Version::v126a:
		offset += 0x34A708;
		break;
	case Version::v127a:
		offset += 0x397E5A;
		break;
	case Version::v127b:
		offset += 0x3B55FA;
		break;
	default:
		return;
	}

	Game::PatchMemory(offset, bytes, 1);
}

void ShowFPS::Stop() {}
