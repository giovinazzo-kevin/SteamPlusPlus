#include "SteamPlusPlus.h"

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

int spp::lua::l_ls(lua_State* L)
{
	
	return 0;
}