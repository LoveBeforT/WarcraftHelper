/**
 * warcrafthelper main
*/

#include "helper.hpp"
#include "game/warcraft.hpp"
#include "plugin/sizebypass.hpp"
#include "plugin/unlockfps.hpp"
#include "plugin/widescreen.hpp"
#include "plugin/windowfixer.hpp"
#include "plugin/autorep.hpp"
#include "plugin/showfps.hpp"
#include "plugin/pathfix.hpp"
#include "plugin/showhpbar.hpp"
#include "plugin/campaignfix.hpp"
#include "plugin/u9helper.hpp"
#include "plugin/replayview.hpp"
#include "plugin/fpslimiter.hpp"

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
	m_plugins.push_back(reinterpret_cast<IPlugin*>(new FpsLimiter()));
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