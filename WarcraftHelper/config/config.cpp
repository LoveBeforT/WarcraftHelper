/**
 * config
*/

#include "config.hpp"

Config::Config() {
    CSimpleIniA ini;

	ini.SetUnicode();
    ini.LoadFile(L"WarcraftHelper.ini");

    m_unlockFps = ini.GetBoolValue("Options", "UnlockFPS", false);
    m_showFps = ini.GetBoolValue("Options", "ShowFPS", false);
    m_wideScreen = ini.GetBoolValue("Options", "WideScreen", false);
    m_unlockMapSize = ini.GetBoolValue("Options", "UnlockMapSize", false);
    m_showHpBar = ini.GetBoolValue("Options", "ShowHPBar", false);
    m_autoRep = ini.GetBoolValue("Options", "AutoSaveReplay", false);
    m_fullScreen = ini.GetBoolValue("Options", "AutoFullScreen", false);
    m_fpsLimit = ini.GetBoolValue("Options", "FpsLimit", false);
    m_targetFps = ini.GetDoubleValue("Options", "TargetFps", 60.0);
}


Config::~Config() {}

Config *GetConfig() {
    static auto config = new Config();

    return config;
}
