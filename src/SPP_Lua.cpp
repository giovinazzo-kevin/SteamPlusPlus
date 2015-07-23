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
	
	int ret = sppClient.registerCallback(cbID, L, cbName);
	
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
	
	int ret = sppClient.unregisterCallback(cbID, L);
	
	lua_pushnumber(L, ret);
	
	return 1;
}

// virtual int GetFriendMessage(CSteamID steamIDFriend, int iMessageID, void *pvData, int cubData, EChatEntryType *peChatEntryType) = 0;
// LUA signature: spp.parseChatMsg(pubParam) -> steamID, senderSteamID, entryType, message;
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
	
	char msgBuf[2048];
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