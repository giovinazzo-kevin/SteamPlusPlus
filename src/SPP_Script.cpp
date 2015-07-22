#include "SteamPlusPlus.h"

int spp::SteamPlusPlus::runScript(const char* script, int argc, const char** argv)
{
	if(!m_initialized) {
		return kE_Uninitialized;
	}
	
	//m_scripts[script] = luaL_newstate();
	
    return kE_FileNotFound;
}