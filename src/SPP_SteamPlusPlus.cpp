#include <mutex>
#include "SteamPlusPlus.h"

static std::mutex running_mtx;

spp::SteamPlusPlus::~SteamPlusPlus()
{
	cleanupSteamworks();
}

bool spp::SteamPlusPlus::isRunning()
{
	running_mtx.lock();
	bool ret = m_initialized;
	running_mtx.unlock();
	return ret;
}

int spp::SteamPlusPlus::createSandbox(const char* script, int argc, const char** argv, int* retcode)
{
	if( !isRunning() ) {
		return k_EUninitialized;
	}
	return m_globalSandbox.runScript(script, argc, argv, retcode);
}

int spp::SteamPlusPlus::destroySandbox(const char* script)
{
	if( !isRunning() ) {
		return k_EUninitialized;
	}
	return m_globalSandbox.killScript(script);
}