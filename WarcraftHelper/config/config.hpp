
#include "SimpleIni.h"

class Config {
public:
    Config();
    ~Config();

    bool m_unlockFps = false;
    bool m_showFps = false;
    bool m_wideScreen = false;
    bool m_fullScreen = false;
    bool m_unlockMapSize = false;
    bool m_showHpBar = false;
    bool m_autoRep = false;
    bool m_fpsLimit = false;
    double m_targetFps = 0.0f;
};

Config *GetConfig();