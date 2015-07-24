#include "SteamPlusPlus.h"

int spp::LuaSandbox::runScript(const char* script, int argc, const char** argv, int* retcode)
{
	lua_State* L;
	int ret;
	
	// If it's the first time running this script, create a new environment for it.
	if(m_scripts.find(script) == m_scripts.end()) {
		m_scripts[script] = new LuaSandbox();
		m_scripts[script]->m_innerState = L = luaL_newstate();
		luaL_openlibs(L);
		
		// 		Step 1. Load file
		ret = luaL_loadfile(L, script);
		if( ret != 0 ) {
			return spp::lua::handleCompileTimeError(L, ret);
		}
	
		// 		Step 2. Create global symbols and initialize other important things
		spp::lua::initializeScript(L);
	
		//		Step 3. Call the script so that it creates any of its global vars
		ret = lua_pcall(L, 0, 0, 0);
		if( ret != 0 ) {
			return spp::lua::handleRuntimeError(L, ret);
		}
	}
	else {
		L = m_scripts[script]->m_innerState;
	}
	
	// 		Step 4. Call the script's entry point
	
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
		return spp::lua::handleRuntimeError(L, ret);
	}
	
	*retcode = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	return k_EOK;
}

int spp::LuaSandbox::killScript(const char* script)
{
	// If the passed script exists
	if( m_scripts.find(script) != m_scripts.end() ) {
		lua_State* s = m_scripts[script]->m_innerState;
		// Unregister its callbacks
		spp::unregisterAllCallbacks(s);
		// Tell LUA to close the lua_State
		lua_close(s);
		// And then erase the script from the list of things this sandbox gives a fuck about
		delete m_scripts[script];
		m_scripts.erase(script);
		
		return k_EOK;
	}
	else {
		return k_EFileNotFound;
	}
}

lua_State* spp::LuaSandbox::getLuaState()
{
	return m_innerState;
}

spp::LuaSandbox::~LuaSandbox()
{
	for( auto it = m_scripts.begin(); it != m_scripts.end(); ++it )
	{
		killScript(it->first.c_str());
	}
}