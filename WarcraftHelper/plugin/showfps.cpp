#include "showfps.h"

void ShowFPS::Start() {
	DWORD showfps = 0;
	unsigned char bytes[] = { 0x01 };
	DWORD offset = GetGameInstance()->GetGameDllBase();

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
	
	showfps = System::ReadDwordFromReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "showfps");
	System::WriteDwordToReg("SOFTWARE\\Blizzard Entertainment\\Warcraft III\\Video", "showfps", showfps);
	if (!showfps) {
		return;
	}

	Game::PatchMemory(offset, bytes, 1);
}

void ShowFPS::Stop() {}
