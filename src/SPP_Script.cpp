#include "SteamPlusPlus.h"

/*
static int traceback (lua_State *L) 
{
	const char *msg = lua_tostring(L, 1);
	if (msg) {
		luaL_traceback(L, L, msg, 1);
	}
	else if (!lua_isnoneornil(L, 1)) {  // is there an error object? 
		if (!luaL_callmeta(L, 1, "__tostring")) { // try its 'tostring' metamethod 
			lua_pushliteral(L, "(no error message)");
		}
	}
	spp::printf(spp::kPrintError, "%s", msg);
	return 1;
}
*/

int spp::SteamPlusPlus::initializeScript(lua_State* L)
{
	// tlua_pushcfunction(L, traceback);
	
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
	
	return 0;
}

static int _handleRuntimeError(lua_State* L, int ret) 
{
	switch(ret)
	{
		case LUA_ERRRUN:
			// Print additional information
			spp::printf( spp::kPrintError, "%s\n", lua_tostring(L, -1) );
			return spp::kE_Fail;
		case LUA_ERRERR:
			spp::printf(spp::kPrintError, "An error occurred while trying to handle an error. Yo, dawg...\n");
			return spp::kE_Fail;
		case LUA_ERRMEM:
			return spp::kE_Memory;
		default:
			return spp::kE_Unknown;
	}
}

static int _handleCompileTimeError(lua_State* L, int ret)
{
	switch(ret)
	{
		case LUA_ERRFILE:
			return spp::kE_FileNotFound;
		case LUA_ERRSYNTAX:
			// Print additional information
			spp::printf( spp::kPrintError, "%s\n", lua_tostring(L, -1) );
			return spp::kE_Fail;
		case LUA_ERRMEM:
			return spp::kE_Memory;
		default:
			return spp::kE_Unknown;
	}
}

int spp::SteamPlusPlus::runScript(const char* script, int argc, const char** argv, int* retcode)
{
	if(!m_initialized) {
		return kE_Uninitialized;
	}
	
	lua_State* L;
	int ret;
	
	// If it's the first time running this script, create a new environment for it.
	if(m_scripts.find(script) == m_scripts.end()) {
		m_scripts[script] = L = luaL_newstate();
		luaL_openlibs(L);
		
		// 		Step 1. Load file
		ret = luaL_loadfile(L, script);
		if( ret != 0 ) {
			return _handleCompileTimeError(L, ret);
		}
	
		// 		Step 2. Create global symbols and initialize other important things
		initializeScript(L);
	
		//		Step 3. Call the script so that it creates any of its global vars
		ret = lua_pcall(L, 0, 0, 0);
		if( ret != 0 ) {
			return _handleRuntimeError(L, ret);
		}
	}
	else {
		L = m_scripts[script];
	}
	
	// 		Step 4. Call the script's entry point
	// TODO: Push main() function to the stack and call it passing argc and argv!!
	
	lua_getglobal(L, "main");
	// Push argc - ok
	lua_pushnumber(L, argc);
	// Push argv as a single object - slightly less ok
	lua_newtable(L); 
	for(int i = 0; i < argc; ++i)
	{
		lua_pushnumber(L, i); // Index
		lua_pushstring(L, argv[i]); // Value
		lua_settable(L, -3); // Put value at index and pop them off the stack
	}
	// Pass two arguments (argc and the argv table) and expect one returned value (exit code)
	ret = lua_pcall(L, 2, 1, 0); 
	if( ret != 0 ) {
		return _handleRuntimeError(L, ret);
	}
	
	*retcode = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
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