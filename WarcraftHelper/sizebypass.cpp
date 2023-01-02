#include "sizebypass.h"

void SizeBypass::Start() {
	if (this->m_Hooked) {
		return;
	}
	this->m_Hooked = true;
	DWORD addr1 = this->m_GamedllBase;
	DWORD addr2 = this->m_GamedllBase;
	DWORD addr3 = this->m_GamedllBase;
	DWORD bytes_size = 0;
	switch (m_War3Version) {
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
	PatchMemory(addr1, bytes, bytes_size);
	PatchMemory(addr2, bytes, 11);
	PatchMemory(addr3, bytes, 11);
}

void SizeBypass::Stop() {
}
