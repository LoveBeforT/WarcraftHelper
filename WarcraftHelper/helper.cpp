/**
 * warcrafthelper main
*/

#include "helper.h"
#include "game/warcraft.h"
#include "plugin/sizebypass.h"
#include "plugin/unlockfps.h"
#include "plugin/widescreen.h"
#include "plugin/windowfixer.h"
#include "plugin/autorep.h"
#include "plugin/showfps.h"
#include "plugin/pathfix.h"
#include "plugin/showhpbar.h"
#include "plugin/campaignfix.h"
#include "plugin/u9helper.h"
#include "plugin/replayview.h"

#include <atlstr.h>

#pragma comment(lib, "Version.lib")
#pragma warning(disable:4996) 

Helper::Helper() {
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new SizeBypass()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new WideScreen()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new UnlockFPS()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new WindowFixer()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new AutoRep()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new ShowFPS()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new PathFix()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new ShowHPBar()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new CampaignFix()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new U9Helper()));
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new ReplayView()));
}

Helper::~Helper() {}

void Helper::LoadPlugins() {
	auto game = GetGameInstance();

	if (game == NULL || !game->IsWarcraft()) {
		return;
	}

#ifdef _DEBUG
	InitConsole();
#endif

	if (!game->GetGameDllBase()) {
		ERROR_GAMEDLL_INIT();
		return;
	}
	for (size_t i = 0; i < m_plugins.size(); i++) {
		m_plugins[i]->Start();
	}
}

void Helper::InitConsole() {
	AllocConsole();

	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	SetConsoleTitleA("WarcraftHelper");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, false);
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(hwnd, GWL_STYLE, style);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_MAXIMIZE, MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_SIZE, MF_BYCOMMAND);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_FRAMECHANGED);
	DrawMenuBar(hwnd);

	ShowWindow(hwnd, SW_MINIMIZE);
}