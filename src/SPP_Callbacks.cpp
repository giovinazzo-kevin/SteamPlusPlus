#include <algorithm>

#include "SteamPlusPlus.h"
#include "SPP_Terminal.h"

/** Maps each callback id to a vector of pairs of LuaSandboxes and strings which represent the LUA function to be called. */
static std::unordered_map<int, std::vector<std::pair<lua_State*, std::string>>> g_callbacks;

int spp::registerCallback(int cbID, lua_State* L, const char* cbname)
{
	// Callback already registered from this script.
	if( g_callbacks.find(cbID) != g_callbacks.end() ) {
		auto *v = &g_callbacks[cbID];
		for(auto it = v->begin(); it != v->end(); ++it)
		{
			if(it->first == L) {
				return k_EFail;
			}
		}
	}

	g_callbacks[cbID].push_back( std::pair<lua_State*, std::string>(L, cbname) );
	return k_EOK;
}

int spp::unregisterCallback(int cbID, lua_State* L)
{
	// Callback not registered from any script
	if( g_callbacks.find(cbID) == g_callbacks.end() ) { 
		return k_EFail;
	}
	
	auto *v = &g_callbacks[cbID];
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

void spp::unregisterAllCallbacks(lua_State* L)
{
	for(auto p = g_callbacks.begin(); p != g_callbacks.end();)
	{
		// And unregister any callback that was bound to this script.
		spp::unregisterCallback( p->first, L ); 
	}
}

void spp::fireCallbacks(int cbID, int cubParam, uint8* pubParam)
{
	auto *v = &g_callbacks[cbID];
	for(auto it = v->begin(); it != v->end(); ++it)
	{
		lua_State* L = it->first; 
		
		lua_getglobal(L, it->second.c_str());
		lua_pushinteger(L, cubParam);
		lua_pushlightuserdata(L, (void *)pubParam);
		
		gets_mtx.lock();
		// Delete the [Steam++] header that is printed by spp::gets()
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
		// And since the user is supposed to enter input, the color can safely remain kPrintBoring
		gets_mtx.unlock();
		
		if( ret != 0 ) {
			spp::lua::handleRuntimeError(L, ret); // In this case we just want _handleRuntimeError to print more info, not to break the control flow.
			spp::printf( spp::kPrintError, "[%s] An error occurred while firing the callback (%d).\n", it->second.c_str(), ret);
		}
	}
}