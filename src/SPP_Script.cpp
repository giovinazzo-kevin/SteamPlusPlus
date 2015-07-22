#include "SteamPlusPlus.h"

int spp::SteamPlusPlus::runScript(const char* script, int argc, const char** argv)
{
	if(!m_initialized) {
		return kE_Uninitialized;
	}
	
	lua_State* L = m_scripts[script] = luaL_newstate();
	
	int ret = luaL_loadfile(L, script);
	if( ret != 0 ) {
		killScript(script);
		switch(ret)
		{
			case LUA_ERRFILE:
				return kE_FileNotFound;
			case LUA_ERRSYNTAX:
				// Print additional information
				spp::printf( kPrintError, "%s\n", lua_tostring(L, -1) );
				return kE_Fail;
			case LUA_ERRMEM:
				return kE_Memory;
			default:
				return kE_Unknown;
		}
	}
	
	// TODO: Push init() function to the stack and call it passing argc and argv!!
	// TODO: Create an error handler that prints the stack trace!
	ret = lua_pcall(L, 0, 0, 0);
	if( ret != 0 ) {
		killScript(script);
		switch(ret)
		{
			case LUA_ERRRUN:
				// The additional information is handled by the error function.
				return kE_Fail;
			case LUA_ERRERR:
				spp::printf(spp::kPrintError, "An error occurred while trying to handle an error. Yo, dawg...\n");
				return kE_Fail;
			case LUA_ERRMEM:
				return kE_Memory;
			default:
				return kE_Unknown;
		}
	}
	
	return kE_OK;
}

int spp::SteamPlusPlus::killScript(const char* script)
{
	if(!m_initialized) {
		return kE_Uninitialized;
	}
	
	if( m_scripts[script] ) {
		lua_close( m_scripts[script] );
		return spp::kE_OK;
	}
	else {
		return spp::kE_FileNotFound;
	}
}

spp::SteamPlusPlus::~SteamPlusPlus()
{
	for( auto it = m_scripts.begin(); it != m_scripts.end(); ++it )
	{
		killScript(it->first);
	}
}