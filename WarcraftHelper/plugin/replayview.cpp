#include "replayview.hpp"

void ReplayView::Start()
{
    DWORD replayMaxSpeedAddr01 = GetGameInstance()->GetGameDllBase();
    DWORD replayMaxSpeedAddr02 = GetGameInstance()->GetGameDllBase();
    DWORD replayMaxSpeedAddr03 = GetGameInstance()->GetGameDllBase();

    switch (GetGameInstance()->GetGameVersion())
    {
    case Version::v120e:
        replayMaxSpeedAddr01 += 0x0F1837;
        replayMaxSpeedAddr02 += 0x28FE88;
        replayMaxSpeedAddr03 += 0x28FE8C;
        break;
    case Version::v124e:
        replayMaxSpeedAddr01 += 0x2F7BC9;
        replayMaxSpeedAddr02 += 0x53F2E9;
        replayMaxSpeedAddr03 += 0x53F2EC;
        break;
    case Version::v126a:
        replayMaxSpeedAddr01 += 0x2F7089;
        replayMaxSpeedAddr02 += 0x53E7E8;
        replayMaxSpeedAddr03 += 0x53E7EC;
        break;
    case Version::v127a:
        replayMaxSpeedAddr01 += 0x3535E7;
        replayMaxSpeedAddr02 += 0x30F931;
        replayMaxSpeedAddr03 = 0;
        break;
    case Version::v127b:
        replayMaxSpeedAddr01 += 0x370D67;
        replayMaxSpeedAddr02 += 0x32D0A1;
        replayMaxSpeedAddr03 = 0;
        break;
    default:
        return;
    }

    Game::WriteBytes((void *)replayMaxSpeedAddr01, 0x10);
    Game::WriteBytes((void *)replayMaxSpeedAddr02, 0x10);
    Game::WriteBytes((void *)replayMaxSpeedAddr03, 0x10);
}

void ReplayView::Stop() {}
