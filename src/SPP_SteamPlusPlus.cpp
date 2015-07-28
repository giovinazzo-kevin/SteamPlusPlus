#include <mutex>
#include "SteamPlusPlus.h"


spp::SteamPlusPlus::~SteamPlusPlus()
{
	cleanupSteamworks();
}

bool spp::SteamPlusPlus::isRunning() const
{
	return m_initialized;
}

int spp::SteamPlusPlus::createSandbox(const char* script, int argc, const char** argv, int* retcode)
{
	if( !isRunning() ) {
		return k_EUninitialized;
	}
	// Thread safety is implemented by LuaSandbox
	return m_globalSandbox.runScript(script, argc, argv, retcode);
}

int spp::SteamPlusPlus::destroySandbox(const char* script)
{
	if( !isRunning() ) {
		return k_EUninitialized;
	}
	// Thread safety is implemented by LuaSandbox
	return m_globalSandbox.killScript(script);
}