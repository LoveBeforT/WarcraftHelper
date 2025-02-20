/**
 * warcraft base functions
 */

#include "warcraft.hpp"

#include <detours.h>
#include <utility>
#include <atlstr.h>

// from https://stackoverflow.com/questions/1888863/how-to-get-main-window-handle-from-process-id
static HWND findTopWindow(DWORD pid)
{
    std::pair<HWND, DWORD> params = {0, pid};

    // Enumerate the windows using a lambda to process each window
    BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
    {
        auto pParams = (std::pair<HWND, DWORD>*)(lParam);

        DWORD processId;
        if (GetWindowThreadProcessId(hwnd, &processId) &&
            processId == pParams->second &&
            GetWindow(hwnd, GW_OWNER) == 0)
        {
            // Stop enumerating
            SetLastError(-1);
            pParams->first = hwnd;
            return FALSE;
        }

        // Continue enumerating
        return TRUE; 
    }, (LPARAM)&params);

    if (!bResult && GetLastError() == -1 && params.first)
    {
        return params.first;
    }

    return 0;
}

Game* GetGameInstance() {
    static auto game = new Game();
    return game;
}

Game::Game()
{
    UINT bytesCount = 0;
    char data[2048] = {0};
    VS_FIXEDFILEINFO *fileInfo = NULL;

    // get game version
    if (GetFileVersionInfoA("Game.dll", 0, sizeof(data), data))
    {
        VerQueryValueA(data, "\\", (void **)&fileInfo, &bytesCount);
        m_war3Version = static_cast<Version>(fileInfo->dwFileVersionLS & 0xffff);
    }

    // game.dll handle
    m_gamedllHandle = GetModuleHandle("Game.dll");
    m_gamedllBase = (DWORD)m_gamedllHandle;
}

Game::~Game() {}

void Game::Hook(void *pOldFuncAddr, void *pNewFuncAddr)
{
    if (!pOldFuncAddr)
    {
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(void *&)pOldFuncAddr, pNewFuncAddr);
    DetourTransactionCommit();
}

void Game::InlineHook(void *pOldFuncAddr, void *pNewFuncAddr, void *&pCallBackFuncAddr)
{
    if (!pOldFuncAddr)
    {
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(void *&)pOldFuncAddr, pNewFuncAddr);
    DetourTransactionCommit();
    pCallBackFuncAddr = pOldFuncAddr;
}

void Game::DetachHook(void *pOldFuncAddr, void *pNewFuncAddr)
{
    if (!pOldFuncAddr)
    {
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(void *&)pOldFuncAddr, pNewFuncAddr);
    DetourTransactionCommit();
}

void Game::PatchMemory(uintptr_t pAddress, unsigned char *bytes, uint32_t size)
{
    if (!pAddress)
    {
        return;
    }

    unsigned long Protection;
    void *address = reinterpret_cast<void *>(pAddress);

    VirtualProtect(address, size, PAGE_READWRITE, &Protection);
    memcpy(address, (const void *)bytes, size);
    VirtualProtect(address, size, Protection, &Protection);
}

void Game::WriteNOP(void *pAddress, DWORD dwCount)
{
    if (!pAddress)
    {
        return;
    }
    static DWORD dwProtect;

    if (VirtualProtect(pAddress, dwCount, PAGE_EXECUTE_READWRITE, &dwProtect))
    {
        for (DWORD i = 0; i < dwCount; i++)
            *(BYTE *)((DWORD)pAddress + i) = 0x90;

        VirtualProtect(pAddress, dwCount, dwProtect, &dwProtect);
    }
}

void Game::WriteBytes(void *pAddress, BYTE data)
{
    if (!pAddress)
    {
        return;
    }
    static DWORD dwProtect;

    if (VirtualProtect(pAddress, 1, PAGE_EXECUTE_READWRITE, &dwProtect))
    {
        *(BYTE *)((DWORD)pAddress) = data;

        VirtualProtect(pAddress, 1, dwProtect, &dwProtect);
    }
}

DWORD Game::SearchPatterns(void *pPattern, DWORD dwPatternLen, DWORD dwSearchStart, DWORD dwSearchEnd)
{
    DWORD dwStartAddr = dwSearchStart;
    DWORD dwEndAddr = dwSearchEnd - dwPatternLen;

    while (dwStartAddr < dwEndAddr)
    {
        bool found = true;
        for (DWORD i = 0; i < dwPatternLen; i++)
        {
            char code = *(char *)(dwStartAddr + i);
            if (*((char *)pPattern + i) != code)
            {
                found = false;
                break;
            }
        }
        if (found)
            return dwStartAddr;
        dwStartAddr++;
    }
    return 0;
}

HWND Game::GetGameWindow()
{
    if (!m_hWnd)
    {
        m_hWnd = findTopWindow(GetCurrentProcessId());
    }
    return m_hWnd;
}

DWORD Game::GetGameDllBase()
{
    return m_gamedllBase;
}

Version Game::GetGameVersion()
{
    return m_war3Version;
}

bool Game::IsWarcraft()
{
    CString tmp;
    char path[MAX_PATH] = {0};

    GetModuleFileNameA(NULL, path, MAX_PATH);
    tmp.Format("%s", path);

    DWORD loc = tmp.ReverseFind('\\');
    for (int i = loc + 1; i < MAX_PATH; i++)
    {
        if (path[i] >= 'A' && path[i] <= 'Z')
        {
            path[i] += 'a' - 'A';
        }
    }

    if (strncmp(path + loc + 1, "war3.exe", 8))
    {
        return false;
    }
    return true;
}