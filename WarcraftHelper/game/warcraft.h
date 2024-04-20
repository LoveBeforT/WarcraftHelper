/**
 * warcraft base functions
*/

#ifndef GAME_WARCRAFT
#define GAME_WARCRAFT

#include "version.h"
#include <stdint.h>

#define ERROR_GAMEDLL_INIT() {MessageBoxA(0, "GameDll was unable to init", "WHelper", 0);}
#define ERROR_GAMEWINDOW_INIT() {MessageBoxA(0, "War3 windows was unable to init", "WHelper", 0);}

static const char* WarcraftRecordHeader = "Warcraft III recorded game";

class System
{
public:
    static DWORD ReadDwordFromReg(LPCTSTR regPath, LPCTSTR keyName)
    {
        HKEY hKey = 0;
        long lret = ::RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey);
        if (lret == ERROR_SUCCESS)
        {
            DWORD value;
            DWORD dwType = REG_DWORD;
            DWORD dwSize = sizeof(DWORD);

            lret = ::RegQueryValueEx(hKey, keyName, 0, &dwType, (BYTE *)&value, &dwSize);
            if ((lret == ERROR_SUCCESS) && (dwType == REG_DWORD))
            {
                ::RegCloseKey(hKey);
                return value;
            }
            ::RegCloseKey(hKey);
        }

        return 0;
    }

    static BOOL WriteDwordToReg(LPCTSTR regPath, LPCTSTR keyName, DWORD value)
    {
        HKEY hKey = 0;
        long lret = ::RegCreateKey(HKEY_CURRENT_USER, regPath, &hKey);
        if (lret == ERROR_SUCCESS)
        {
            lret = ::RegSetValueEx(hKey, keyName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD));
            if (lret == ERROR_SUCCESS)
            {
                ::RegCloseKey(hKey);
                return TRUE;
            }
            ::RegCloseKey(hKey);
        }

        return FALSE;
    }

    static BOOL IsKeyDown(int key) {
        short state = GetAsyncKeyState(key);
        return (state & 0x8000) ? true : false;
    }
};

class Game
{
public:
    Game();
    ~Game();

    static void Hook(void *pOldFuncAddr, void *pNewFuncAddr);
    static void InlineHook(void *pOldFuncAddr, void *pNewFuncAddr, void *&pCallBackFuncAddr);
    static void DetachHook(void *pOldFuncAddr, void *pNewFuncAddr);
    static void PatchMemory(uintptr_t pAddress, unsigned char *bytes, uint32_t size);
    static void WriteBytes(void *pAddress, BYTE data);
    static void WriteNOP(void *pAddress, DWORD dwCount);

    DWORD SearchPatterns(void *pPattern, DWORD dwPatternLen, DWORD dwSearchStart, DWORD dwSearchEnd);
    HWND GetGameWindow();
    Version GetGameVersion();
    DWORD GetGameDllBase();
    bool IsWarcraft();

private:
    HWND m_hWnd{NULL};
    DWORD m_gamedllBase{NULL};
    HANDLE m_gamedllHandle{NULL};
    Version m_war3Version{Version::unknown};
};

Game* GetGameInstance();
#endif