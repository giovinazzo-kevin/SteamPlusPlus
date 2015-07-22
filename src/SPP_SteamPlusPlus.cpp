#include <mutex>
#include "SteamPlusPlus.h"

static std::mutex running_mtx;

spp::SteamPlusPlus::~SteamPlusPlus()
{
	for( auto it = m_scripts.begin(); it != m_scripts.end(); ++it )
	{
		killScript(it->first.c_str());
	}
	
	cleanupSteamworks();
}

bool spp::SteamPlusPlus::isRunning()
{
	running_mtx.lock();
	bool ret = m_initialized;
	running_mtx.unlock();
	return ret;
}