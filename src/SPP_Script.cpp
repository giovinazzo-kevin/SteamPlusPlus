#include "SteamPlusPlus.h"

int spp::SteamPlusPlus::createGlobals(lua_State* L)
{
	// This is the main table containing functions that interface directly with SPP.
	// Note: setfield pops the value off the stack.
	
	lua_newtable(L);
	
	lua_pushcfunction(L, spp::lua::l_print);
	lua_setfield(L, -2, "print");
	
	lua_pushcfunction(L, spp::lua::l_printerr);
	lua_setfield(L, -2, "err");
	
	lua_pushcfunction(L, spp::lua::l_printinfo);
	lua_setfield(L, -2, "info");
	
	lua_setglobal(L, "spp");
	// Return spp table, which is already at the top of the stack.
	return 1;
}

int spp::SteamPlusPlus::runScript(const char* script, int argc, const char** argv)
{
	if(!m_initialized) {
		return kE_Uninitialized;
	}
	
	lua_State* L = m_scripts[script];
	
	// If it's the first time running this script, create a new environment for it.
	if(m_scripts.find(script) == m_scripts.end()) {
		L = m_scripts[script] = luaL_newstate();
		// 		Step 1. Load file
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
		// 		Step 2. Create global symbols
		createGlobals(L);
	}
	
	// 		Step 3. Call the script's entry point
	// TODO: Push init() function to the stack and call it passing argc and argv!!
	// TODO: Create an error handler that prints the stack trace!
	int ret = lua_pcall(L, 0, 0, 0);
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

int spp::lua::l_print(lua_State* L)
{
	const char* message = lua_tostring(L, 1);
	spp::printf(spp::kPrintNormal, message);
	return 0;
}

int spp::lua::l_printerr(lua_State* L)
{
	const char* message = lua_tostring(L, 1);
	spp::printf(spp::kPrintError, message);
	return 0;
}

int spp::lua::l_printinfo(lua_State* L)
{
	const char* message = lua_tostring(L, 1);
	spp::printf(spp::kPrintInfo, message);
	return 0;
}