#include <algorithm>

#include "SteamPlusPlus.h"
#include "SPP_Terminal.h"


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
	
	lua_pushcfunction(L, lua::l_print);
	lua_setfield(L, -2, "print");
	
	lua_pushcfunction(L, lua::l_printerr);
	lua_setfield(L, -2, "err");
	
	lua_pushcfunction(L, lua::l_printinfo);
	lua_setfield(L, -2, "info");
	
	lua_pushcfunction(L, lua::l_printboring);
	lua_setfield(L, -2, "boring");
	
	lua_pushcfunction(L, lua::l_registercback);
	lua_setfield(L, -2, "registerCallback");
	
	lua_pushcfunction(L, lua::l_unregistercback);
	lua_setfield(L, -2, "unregisterCallback");
	
	lua_pushcfunction(L, lua::l_parsechatmsg);
	lua_setfield(L, -2, "parseChatMsg");
	
	lua_setglobal(L, "spp");
	
	// Make sure to override print() as well or it'll break the terminal.
	lua_pushcfunction(L, lua::l_print);
	lua_setglobal(L, "print");
	
	return 0;
}

static int _handleRuntimeError(lua_State* L, int ret) 
{
	switch(ret)
	{
		case LUA_ERRRUN:
			// Print additional information
			spp::printf( spp::kPrintError, "%s\n", lua_tostring(L, -1) );
			return spp::k_EFail;
		case LUA_ERRERR:
			spp::printf(spp::kPrintError, "An error occurred while trying to handle an error. Yo, dawg...\n");
			return spp::k_EFail;
		case LUA_ERRMEM:
			return spp::k_EMemory;
		default:
			return spp::k_EUnknown;
	}
}

static int _handleCompileTimeError(lua_State* L, int ret)
{
	switch(ret)
	{
		case LUA_ERRFILE:
			return spp::k_EFileNotFound;
		case LUA_ERRSYNTAX:
			// Print additional information
			spp::printf( spp::kPrintError, "%s\n", lua_tostring(L, -1) );
			return spp::k_EFail;
		case LUA_ERRMEM:
			return spp::k_EMemory;
		default:
			return spp::k_EUnknown;
	}
}

int spp::SteamPlusPlus::runScript(const char* script, int argc, const char** argv, int* retcode)
{
	if(!m_initialized) {
		return k_EUninitialized;
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
	
	return k_EOK;
}

int spp::SteamPlusPlus::killScript(const char* script)
{
	if(!m_initialized) {
		return k_EUninitialized;
	}
	
	// If the passed script exists
	if( m_scripts.find(script) != m_scripts.end() ) {
		// Iterate the callbacks map
		for(auto p = m_callbacks.begin(); p != m_callbacks.end();)
		{
			auto value = p->second;
			// And for each registered callback, iterate its vector of subscribers
			for(auto it = value.begin(); it != value.end(); ++it)
			{
				// If the current subscriber equals the script we're killing, erase the <script, callback> pair
				if( it->first == m_scripts[script] ) {
					p = m_callbacks.erase(p);
				} 
				else {
					p++;
				}
			}
			
		}
		
		// And then erase the script
		m_scripts.erase(script);
		// Finally, tell LUA to close the lua_State
		lua_close( m_scripts[script] );
		
		return k_EOK;
	}
	else {
		return k_EFileNotFound;
	}
}

int spp::SteamPlusPlus::registerCallback(int cbID, lua_State* L, const char* cbname)
{
	// Callback already registered from this script.
	if( m_callbacks.find(cbID) != m_callbacks.end() ) {
		auto *v = &m_callbacks[cbID];
		for(auto it = v->begin(); it != v->end(); ++it)
		{
			if(it->first == L) {
				return k_EFail;
			}
		}
	}

	m_callbacks[cbID].push_back( std::pair<lua_State*, std::string>(L, cbname) );
	return k_EOK;
}

int spp::SteamPlusPlus::unregisterCallback(int cbID, lua_State* L)
{
	// Callback not registered from any script
	if( m_callbacks.find(cbID) == m_callbacks.end() ) { 
		return k_EFail;
	}
	
	auto *v = &m_callbacks[cbID];
	for(auto it = v->begin(); it != v->end(); ++it)
	{
		if( it->first == L ) {
			v->erase(it);
			return k_EOK;
		}
	}
	
	// Callback not registered from this particular script
	return k_EFail;
}

void spp::SteamPlusPlus::fireCallbacks(int cbID, int cubParam, uint8* pubParam)
{
	auto *v = &m_callbacks[cbID];
	for(auto it = v->begin(); it != v->end(); ++it)
	{
		auto L = it->first;
		
		lua_getglobal(L, it->second.c_str());
		lua_pushinteger(L, cubParam);
		lua_pushlightuserdata(L, (void *)pubParam);
		
		gets_mtx.lock();
		// Delete the [Steam++] header.
		if( headerDisplaying ) {
			int hlen = strlen(header);
			while(--hlen)
			{
				fputs("\b\b ", stdout);
			}
			fputc('\b', stdout);
		}
		// Let the callback fire and print whatever it wants
		int ret = lua_pcall(L, 2, 1, 0);
		// Re-print the header
		setTerminalColor(spp::kPrintBoring);
		fputs(header, stdout);
		// And since the user is supposed to enter input, the color can remain kPrintBoring
		gets_mtx.unlock();
		
		if( ret != 0 ) {
			_handleRuntimeError(L, ret);
			spp::printf( spp::kPrintError, "[%s] An error occurred while firing the callback (%d).\n", it->second.c_str(), ret);
		}
	}
}