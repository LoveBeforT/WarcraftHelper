#ifndef HELPER_H
#define HELPER_H

#include "plugin/plugin.h"
#include <vector>

class Helper {
public:
	Helper();
	~Helper();

	void LoadPlugins();
private:
	std::vector<IPlugin*> m_plugins;

	static void InitConsole();
};

#endif