#include <cstdlib> // malloc
#include <mutex>
#include "SteamPlusPlus.h"

const char* kScriptsFolderPath = "scripts/";
static std::mutex sandbox_mtx;

char* _buildPath(const char* script)
{
	// Prepend the scripts folder's location to the first argument
	char* pathBuf = (char*)malloc(strlen(script) + strlen(kScriptsFolderPath) + 5); // + 5 = Additional 4 bytes in case ".lua" needs to be appended and terminator.
	sprintf(pathBuf, "%s%s", kScriptsFolderPath, script);
	// If the entered path doesn't end in .lua, add it
	if( strstr(pathBuf, ".lua") != pathBuf + strlen(pathBuf) - 4 ) {
		strcat(pathBuf, ".lua");
	}
	return pathBuf;
}

int spp::LuaSandbox::runScript(const char* script, int argc, const char** argv, int* retcode)
{
	lua_State* L;
	int ret;
	
	// Try to get a working path from the script name
	script = _buildPath(script);
	
	// We don't want m_scripts or any lua_State* to be altered right now!
	sandbox_mtx.lock();
	// If it's the first time running this script, create a new environment for it.
	if(m_scripts.find(script) == m_scripts.end()) {
		m_scripts[script] = new LuaSandbox();
		m_scripts[script]->m_innerState = L = luaL_newstate();
		luaL_openlibs(L);
		
		// 		Step 1. Load file
		ret = luaL_loadfile(L, script);
		if( ret != 0 ) {
			delete script;
			sandbox_mtx.unlock();
			return spp::lua::handleCompileTimeError(L, ret);
		}
	
		// 		Step 2. Create global symbols and initialize other important things
		spp::lua::initializeScript(L);
	
		//		Step 3. Call the script so that it creates any of its global vars
		ret = lua_pcall(L, 0, 0, 0);
		if( ret != 0 ) {
			delete script;
			sandbox_mtx.unlock();
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
		killScript(script);
		delete script;
		sandbox_mtx.unlock();
		return spp::lua::handleRuntimeError(L, ret);
	}
	
	*retcode = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	sandbox_mtx.unlock();
	// If the script hasn't registered any callbacks, RIP
	if( !spp::hasCallbacks(L) ) {
		killScript(script);
	}
	
	delete script;
	return k_EOK;
}

int spp::LuaSandbox::killScript(const char* script)
{
	// Try to get a working path from the script name
	script = _buildPath(script);
	
	// Likewise, we don't want anything to change while we're killing a script
	sandbox_mtx.lock();
	// If the passed script exists
	if( m_scripts.find(script) != m_scripts.end() ) {
		lua_State* L = m_scripts[script]->m_innerState;
		// Unregister its callbacks
		spp::unregisterAllCallbacks(L);
		// Tell LUA to close the lua_State
		lua_close(L);
		// And then erase the script from the list of things this sandbox gives a fuck about
		delete m_scripts[script];
		m_scripts.erase(script);
		
		delete script;
		sandbox_mtx.unlock();
		return k_EOK;
	}
	else {
		delete script;
		sandbox_mtx.unlock();
		return k_EFileNotFound;
	}
}


spp::LuaSandbox* spp::LuaSandbox::rfindParent(lua_State* L)
{
	sandbox_mtx.lock();
	if( this->m_innerState == L ) {
		sandbox_mtx.unlock();
		return this;
	}
	
	for( auto it = m_scripts.begin(); it != m_scripts.end(); ++it)
	{
		LuaSandbox* ret = it->second->rfindParent(L);
		if( ret != NULL ) {
			sandbox_mtx.unlock();
			return ret;
		}
	}
	
	sandbox_mtx.unlock();
	return NULL;
}

const char* spp::LuaSandbox::findName(lua_State* L) const
{
	sandbox_mtx.lock();
	for( auto it = m_scripts.begin(); it != m_scripts.end(); ++it)
	{
		if( it->second->m_innerState == L ) {
			sandbox_mtx.unlock();
			return it->first.c_str();
		}
		
	}
	
	sandbox_mtx.unlock();
	return NULL;
}

spp::LuaSandbox::~LuaSandbox()
{
	for( auto it = m_scripts.begin(); it != m_scripts.end(); ++it )
	{
		killScript(it->first.c_str());
	}
}