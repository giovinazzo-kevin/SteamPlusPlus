#include "Main.h"

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

int spp::lua::l_registercback(lua_State* L)
{
	if( !lua_isnumber(L, 1) ) {
		lua_pushstring(L, "The first argument must be a number, the Callback ID.");
		lua_error(L);
	}
	
	if( !lua_isstring(L, 2)  ) {
		lua_pushstring(L, "The second argument must be a string, the name of the Callback function.");
		lua_error(L);
	}
	
	int cbID = lua_tonumber(L, 1);
	const char* cbName = lua_tostring(L, 2);
	
	int ret = registerCallbackProxy(cbID, L, cbName);
	lua_pushnumber(L, ret);
	
	return 1;
}

int spp::lua::l_unregistercback(lua_State* L)
{
	if( !lua_isnumber(L, 1) ) {
		lua_pushstring(L, "The first argument must be a number, the Callback ID.");
		lua_error(L);
	}
	
	int cbID = lua_tonumber(L, 1);
	
	int ret = unregisterCallbackProxy(cbID, L);
	lua_pushnumber(L, ret);
	
	return 1;
}