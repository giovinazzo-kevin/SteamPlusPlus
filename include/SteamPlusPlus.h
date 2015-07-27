#ifndef STEAMPLUS_H
#define STEAMPLUS_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <cstdarg> //va_list
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <vector>

#include "Steamworks.h"

#ifdef _WIN32
#define sleep(x) Sleep( (x) )
#else
#define sleep(x) usleep( (x) * 1000 )
#endif

namespace spp
{
/** An argument that must be passed to functions like spp::printf(), determines the color and meaning of the outputted text. */
enum PrintMode {
// Since the way of handling character color varies across platforms, each mode has to be defined for both Windows and UNIX.
#ifdef _WIN32
    kPrintNormal = 0xF, /** Standard print mode, used for mundane messages. */
#else
    kPrintNormal = 97, /** Standard print mode, used for mundane messages. */
#endif

#ifdef _WIN32
    kPrintError = 0xC, /** Error print mode, used for error messages. */
#else
    kPrintError = 91, /** Error print mode, used for error messages. */
#endif

#ifdef _WIN32
    kPrintBoring = 0x7, /** Prints information in a non-flashy color. */
#else
    kPrintBoring = 37, /** Prints information in a non-flashy color. */
#endif

#ifdef _WIN32
    kPrintInfo = 0xE /** Info print mode, used for notifying the user about important things. */ 
#else
    kPrintInfo = 93 /** Info print mode, used for notifying the user about important things. */
#endif
};

/**  Error codes that will be thrown by an instance of the SteamPlusPlus class.*/
enum ErrorCodes
{
	k_EOK = 0,           /** Everything went according to plan. Odd... */
	k_EFail,             /** Something really bad happened. */
	k_EUninitialized,    /** The SteamPlusPlus object has not been initialized. */
	k_EFileNotFound,     /** The requested file was not found. */
	k_EMemory,           /** For errors relative to memory allocation and deallocation. */
	k_EUnknown           /** Something that should be used sparingly. */
};

/**
 * @brief Wrapper of printf() that supports PrintModes.
 * @param printMode Enumeration defining the type of message to send.
 * @param fmt The classic "printf" format string.
 * @return The number of characters read.
 */
int printf(PrintMode printMode, const char* fmt, ...);
/**
 * @brief Wrapper of printf that supports PrintModes.
 * @param printMode Enumeration defining the type of message to send.
 * @param fmt The classic "printf" format string.
 * @param args A va_list of arguments to pass to vprintf().
 * @return The number of characters read.
 */
int vprintf(PrintMode printMode, const char* fmt, va_list args);

/**
 * @brief Wrapper of fgets() used to read lines.
 * @param str A pointer to a character array that will be filled with the user input.
 * @param n The maximum number of characters to read.
 * @param displayHeader If true, preprends the line with kUserInputHeader.
 * @return str.
 */
char* gets(char* str, size_t n, bool displayHeader = true);

/** This class isolates a lua_State from the others, safely allowing it to spawn and ~~murder~~ kill children. */
class LuaSandbox
{
	private:
	/** The lua_State being sandboxed. */
	lua_State* m_innerState;
	
	/** A sandbox for each child script... to play in. */
	std::unordered_map<std::string, LuaSandbox*> m_scripts;
	
	public:
	~LuaSandbox();
	
	/**
	 * @brief Runs a script.
	 * @param script The relative path to the script.
	 * @param argc The number of arguments to pass to the script.
	 * @param argv The list of arguments to pass to the script. It should be of length argc and the first argument should be equivalent to its name.
	 * @param retcode The return code of the script.
	 * @return kE_OK 				on success,
	 *         kE_Fail 				on generic failures (which are documented by an error handler),
	 *         kE_FileNotFound		if the script wasn't found, 
	 *         kE_Memory			if lua_loadfile failed due to memory allocation problems,
	 *         kE_Unknown			welp.
	 */
	int runScript(const char* script, int argc, const char** argv, int* retcode);
	
	/**
	 * @brief Kills a script and detaches any hooks it originally had created.
	 * @param script The name of the script to kill. 
	 *        IMPORTANT: This is the name that was used to load the script, so if it was moved don't pass its new location.
	 * @return kE_OK				on success,
	 *         kE_FileNotFound		if an invalid path was passed,
	 *         kE_Unknown			welp.
	 */
	int killScript(const char* script);
	/**
	 * @brief Kills a script and detaches any hooks it originally had created.
	 * @param L The lua_State* of the script to kill. 
	 * @return kE_OK				on success,
	 *         kE_FileNotFound		if an invalid lua_Script was passed,
	 *         kE_Unknown			welp.
	 */
	inline int killScript(lua_State* L) 
	{
		return killScript( findName(L) );
	}
	
	/**
	 * @brief Returns a pointer to the sandbox associated to the given lua_State*, or NULL.
	 * This function is recursive.
	 */
	LuaSandbox* rfindParent(lua_State* L);
	/**
	 * @brief Returns the name of the script that matches the given lua_State
	 */
	const char* findName(lua_State* L);
	
	/**
	 * @brief Returns a pointer to the lua_State being sandboxed.
	 */
	lua_State* getLuaState() { return m_innerState; }
};

/** This class wraps the OpenSteamworks API and the LUA interpreter together. */
class SteamPlusPlus
{
	private:
	bool m_initialized = false;
	bool m_running = true;
	
	HSteamPipe m_hPipe;
	HSteamUser m_hUser;
	CSteamAPILoader		m_steamLoader;
	ISteamClient017 * 	m_pSteamClient;
	ISteamUser017* 		m_pSteamUser;
	ISteamFriends015* 	m_pSteamFriends;
	
	/** The sandbox in which scripts ran by SteamPlusPlus itself are executed. */
	LuaSandbox m_globalSandbox;
	
	public:
	~SteamPlusPlus();
	
	bool isRunning();

	int initSteamworks();
	void cleanupSteamworks();
	
	HSteamPipe getSteamPipe() { return m_hPipe; }
	HSteamUser getSteamUser() { return m_hUser; }
	ISteamClient017*  getISteamClient()  { return m_pSteamClient; }
	ISteamUser017*    getISteamUser()    { return m_pSteamUser; }
	ISteamFriends015* getISteamFriends() { return m_pSteamFriends; }
	LuaSandbox* getGlobalSandbox() { return &m_globalSandbox; }
	
	/**
	 * @brief Basically just a wrapper of LuaSandbox::runScript, which instead has an appropriate description.
	 * @return This function can additionally return k_EUninitialized if Steamworks hasn't been initialized yet.
	 */
	int createSandbox(const char* script, int argc, const char** argv, int* retcode);
	
	/**
	 * @brief Basically just a wrapper of LuaSandbox::killScript, which instead has an appropriate description.
	 * @return This function can additionally return k_EUninitialized if Steamworks hasn't been initialized yet.
	 */
	int destroySandbox(const char* script);
};

/**
 * @brief Registers a LUA callback that fires whenever the given event is intercepted.
 * @param cbID The event to intercept.
 * @param L The lua_State for which the callback is registered.
 * @param cbname The name of the LUA function that defines the callback.
 * @return k_EOK on success, k_EFail on failure (callback already registered?).
 */
int registerCallback(int cbID, lua_State* L, const char* cbname);

/**
 * @brief Unregisters a previously registered LUA callback.
 * @param L The lua_State for which the callback is unregistered.
 * @param cbname The function to unregister.
 * @return k_EOK on success, k_EFail on failure (callback not registered?).
 */
int unregisterCallback(lua_State* L, const char* cbname);

/**
 * @brief Unregisters any previously registered LUA callback.
 * @param L The lua_State for which the callbacks are unregistered.
 */
void unregisterAllCallbacks(lua_State* L);

/**
 * @brief Returns true if the lua_State has registered at least one callback.
 * @param L The lua_State for which the callbacks are checked.
 */
bool hasCallbacks(lua_State* L);

/**
 * @brief Fires any callback associated with the given cbID.
 */
void fireCallbacks(int cbID, int cubParam, uint8* pubParam);

	namespace lua
	{
	int handleRuntimeError(lua_State* L, int ret);	
	int handleCompileTimeError(lua_State* L, int ret);	
	int initializeScript(lua_State* L);
		
	/**
	 * @brief Prints a string in kPrintNormal mode.
	 */
	int l_print(lua_State* L);
	/**
	 * @brief Prints a string in kPrintError mode.
	 */
	int l_printerr(lua_State* L);
	/**
	 * @brief Prints a string in kPrintInfo mode.
	 */
	int l_printinfo(lua_State* L);
	/**
	 * @brief Prints a string in kPrintBoring mode.
	 */
	int l_printboring(lua_State* L);
	/**
	 * @brief Registers a callback that fires when the passed message is intercepted.
	 */
	int l_registercback(lua_State* L);
	/**
	 * @brief Unregisters a previously registered callback.
	 */
	int l_unregistercback(lua_State* L);
	/**
	 * @brief Calls a LUA script, returning its exit code.
	 */
	int l_runscript(lua_State* L);
	/**
	 * @brief Kills a LUA script that was previously called.
	 * Only a script that was created by the calling script can be killed.
	 */
	int l_killscript(lua_State* L);

	/* OPENSTEAMWORKS BINDINGS BELOW */

	/**
	 * @brief Parses a FriendChatMsg_t pointer.
	 */
	int l_parsechatmsg(lua_State* L);
	}
}

#endif // STEAMPLUS_H