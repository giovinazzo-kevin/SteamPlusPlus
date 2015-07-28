#include <algorithm>
#include <vector>

#include "SteamPlusPlus.h"
#include "SPP_Terminal.h"
#include "Main.h"

struct LUACallback_t
{
	int m_cbID;
	lua_State* m_luaState;
	const char* m_luaFuncName;
};

std::mutex callbacks_mtx;
std::vector<LUACallback_t> g_callbacks;

int spp::registerCallback(int cbID, lua_State* L, const char* cbname)
{
	callbacks_mtx.lock();
	// Callback already registered from this script?
	for( auto it = g_callbacks.begin(); it != g_callbacks.end(); ++it )
	{
		if( it->m_cbID != cbID && it->m_luaFuncName != cbname ) {
			continue;
		}
		// The same cbID can only be bound to one function, and
		// one function can only be bound to one cbID.
		if( it->m_luaState == L ) { 
			callbacks_mtx.unlock();
			return k_EFail;
		}
	}
	
	// If not, register it
	g_callbacks.push_back( (LUACallback_t){ cbID, L, cbname } );
	callbacks_mtx.unlock();
	return k_EOK;
}

int spp::unregisterCallback(lua_State* L, const char* cbname)
{
	callbacks_mtx.lock();
	for( auto it = g_callbacks.begin(); it != g_callbacks.end(); ++it )
	{
		if( it->m_luaFuncName == cbname && it->m_luaState == L ) {
			
			// registerCallback makes sure that cbnames are exclusive for
			// each lua_State*, so the loop can break here.
			g_callbacks.erase(it);
			if( spp::hasCallbacks(L) ) {
				callbacks_mtx.unlock();
				return k_EOK;
			}
			
			// If the current lua_State* doesn't have any more callbacks
			// registered, it can die.
			spp::LuaSandbox* parent = sppClient.getGlobalSandbox()->rfindParent(L);
			if( parent == NULL ) {
				lua_pushstring(L, "The calling script was orphaned.");
				lua_error(L);
			}
			parent->killScript(L);
			callbacks_mtx.unlock();
			return k_EOK;
		}
	}
	// Callback not registered from this particular script
	callbacks_mtx.unlock();
	return k_EFail;
}

void spp::unregisterAllCallbacks(lua_State* L)
{
	for( auto it = g_callbacks.begin(); it != g_callbacks.end(); )
	{
		if( it->m_luaState == L ) {
			spp::unregisterCallback( L, it->m_luaFuncName ); 
		}
		else {
			++it;
		}
	}
}

bool spp::hasCallbacks(lua_State* L)
{
	for( auto it = g_callbacks.begin(); it != g_callbacks.end(); ++it)
	{
		if( it->m_luaState == L ) {
			return true;
		}
	}
	
	return false;
}

void spp::fireCallbacks(int cbID, int cubParam, uint8* pubParam)
{
	callbacks_mtx.lock();
	for( auto it = g_callbacks.begin(); it != g_callbacks.end(); ++it )
	{
		if( it->m_cbID != cbID ) continue;
		
		lua_getglobal(it->m_luaState, it->m_luaFuncName);
		lua_pushinteger(it->m_luaState, cubParam);
		lua_pushlightuserdata(it->m_luaState, (void *)pubParam);
		
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
		int ret = lua_pcall(it->m_luaState, 2, 1, 0);
		// Re-print the header
		setTerminalColor(spp::kPrintBoring);
		fputs(header, stdout);
		// And since the user is supposed to enter input, the color can safely remain kPrintBoring
		gets_mtx.unlock();
		
		if( ret != 0 ) {
			spp::lua::handleRuntimeError(it->m_luaState, ret); // In this case we just want _handleRuntimeError to print more info, not to break the control flow.
			spp::printf( spp::kPrintError, "[%s] An error occurred while firing the callback (%d).\n", it->m_luaFuncName, ret);
		}
	}
	callbacks_mtx.unlock();
}