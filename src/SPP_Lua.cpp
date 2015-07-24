#include "Main.h"

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

int spp::lua::initializeScript(lua_State* L)
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
	
	lua_pushcfunction(L, spp::lua::l_printboring);
	lua_setfield(L, -2, "boring");
	
	lua_pushcfunction(L, spp::lua::l_registercback);
	lua_setfield(L, -2, "registerCallback");
	
	lua_pushcfunction(L, spp::lua::l_unregistercback);
	lua_setfield(L, -2, "unregisterCallback");
	
	lua_pushcfunction(L, spp::lua::l_parsechatmsg);
	lua_setfield(L, -2, "parseChatMsg");
	
	lua_setglobal(L, "spp");
	
	// Let's override print() as well to make it more future-proof.
	lua_pushcfunction(L, spp::lua::l_print);
	lua_setglobal(L, "print");
	
	return 0;
}

int spp::lua::handleRuntimeError(lua_State* L, int ret) 
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

int spp::lua::handleCompileTimeError(lua_State* L, int ret)
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

int spp::lua::l_printboring(lua_State* L)
{
	const char* message = lua_tostring(L, 1);
	spp::printf(spp::kPrintBoring, message);
	return 0;
}

int spp::lua::l_registercback(lua_State* L)
{
	if( !lua_isinteger(L, 1) ) {
		lua_pushstring(L, "The first argument must be an integer, the Callback ID.");
		lua_error(L);
	}
	
	if( !lua_isstring(L, 2)  ) {
		lua_pushstring(L, "The second argument must be a string, the name of the Callback function.");
		lua_error(L);
	}
	
	int cbID = lua_tointeger(L, 1);
	const char* cbName = lua_tostring(L, 2);
	
	int ret = spp::registerCallback(cbID, L, cbName);
	
	lua_pushinteger(L, ret);
	
	return 1;
}

int spp::lua::l_unregistercback(lua_State* L)
{
	if( !lua_isinteger(L, 1) ) {
		lua_pushstring(L, "The first argument must be an integer, the Callback ID.");
		lua_error(L);
	}
	
	int cbID = lua_tointeger(L, 1);
	
	int ret = spp::unregisterCallback(cbID, L);
	
	lua_pushnumber(L, ret);
	
	return 1;
}

int spp::lua::l_runscript(lua_State* L)
{
	if( !lua_isstring(L, 1) ) {
		lua_pushstring(L, "The first argument must be a string, the name of the script to call.");
		lua_error(L);
	}
	
	
	
}

int spp::lua::l_parsechatmsg(lua_State* L)
{
	if( !lua_islightuserdata(L, 1) ) {
		lua_pushstring(L, "The first argument must be a pointer, the pubParam.");
		lua_error(L);
	}
	
	uint8* pubParam = static_cast<uint8*>( lua_touserdata(L,1) );
	if( pubParam == NULL ) {
		lua_pushstring(L, "The passed pointer isn't a valid FriendChatMsg_t structure.");
		lua_error(L);
	}
	
	FriendChatMsg_t* msgInfo = reinterpret_cast<FriendChatMsg_t*>(pubParam);
	EChatEntryType entryType = k_EChatEntryTypeChatMsg;
	
	char msgBuf[2048]; // There's no way of knowing a message's length beforehand, and messages have a limit of 2048 characters.
	sppClient.getISteamFriends()->GetFriendMessage( msgInfo->m_ulFriendID, msgInfo->m_iChatID, msgBuf, sizeof(msgBuf), &entryType );
	
	lua_pushinteger(L, msgInfo->m_ulFriendID.ConvertToUint64());
	lua_pushinteger(L, msgInfo->m_ulSenderID.ConvertToUint64());
	
	char etBuf[24];
	switch( entryType )
	{
		case k_EChatEntryTypeChatMsg:
			strncpy(etBuf, "ChatMsg", sizeof( etBuf ));
		break;
		case k_EChatEntryTypeEmote:
			strncpy(etBuf, "Emote", sizeof( etBuf ));
		break;
		case k_EChatEntryTypeInviteGame:
			strncpy(etBuf, "InviteGame", sizeof( etBuf ));
		break;
		case k_EChatEntryTypeLeftConversation:
			strncpy(etBuf, "LeftConversation", sizeof( etBuf ));
		break;
		case k_EChatEntryTypeLobbyGameStart:
			strncpy(etBuf, "LobbyGameStart", sizeof( etBuf ));
		break;
		case k_EChatEntryTypeTyping:
			strncpy(etBuf, "Typing", sizeof( etBuf ));
		break;
		case k_EChatEntryTypeInvalid:
		default:
			strncpy(etBuf, "Invalid", sizeof( etBuf ));
		break;
	}
	
	lua_pushstring(L, etBuf);
	lua_pushstring(L, msgBuf);

	return 4;
}