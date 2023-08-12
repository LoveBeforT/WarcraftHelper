#include "replayview.h"

void ReplayView::Start()
{
    DWORD replayMaxSpeedAddr01 = this->m_GamedllBase;
    DWORD replayMaxSpeedAddr02 = this->m_GamedllBase;
    DWORD replayMaxSpeedAddr03 = this->m_GamedllBase;
    BYTE newSpeed = 0x10;

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

    WriteByte((void *)replayMaxSpeedAddr01, newSpeed);
    WriteByte((void *)replayMaxSpeedAddr02, newSpeed);
    WriteByte((void *)replayMaxSpeedAddr03, newSpeed);
}

void ReplayView::Stop()
{
}
