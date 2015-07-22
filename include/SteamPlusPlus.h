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

namespace spp
{
/** An argument that must be passed to functions like spp::printf(), determines the color and meaning of the outputted text. */
enum PrintMode {
// Since the way of handling character color varies across platforms, each mode has to be defined for both Windows and UNIX.
#ifdef _WIN32
    kPrintNormal = 0xF, /** Standard print mode, used for mundane messages. */
#else
    kPrintNormal = 0x7, /** Standard print mode, used for mundane messages. */
#endif

#ifdef _WIN32
    kPrintError = 0xC, /** Error print mode, used for error messages. */
#else
    kPrintError = 0x1, /** Error print mode, used for error messages. */
#endif

#ifdef _WIN32
    kPrintInfo = 0xE /** Info print mode, used for notifying the user about important things. */ 
#else
    kPrintInfo = 0x3 /** Info print mode, used for notifying the user about important things. */
#endif
};

/**  Error codes that will be thrown by an instance of the SteamPlusPlus class.*/
enum ErrorCodes
{
	kE_OK = 0,
	kE_Uninitialized, /** OpenSteamworks or the LUA Engine have not been initialized. */
	kE_FileNotFound /** The requested file was not found. */
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

/**This class wraps the OpenSteamworks API and the LUA interpreter together.*/
class SteamPlusPlus
{
private:
	bool m_initialized = false;
	
	/** Maps each lua_State to the name of the script it originated from. */
	std::unordered_map<const char*, lua_State*> m_scripts;
public:
	/**
	 * @brief Runs a script.
	 * @param script The relative path to the script.
	 * @param argc The number of arguments to pass to the script.
	 * @param argv The list of arguments to pass to the script. It should be of length argc and the first argument should be equivalent to its name.
	 * @return kE_Uninitialized if OpenSteamworks or the LUA Engine have not been loaded, or kE_FileNotFound if the script wasn't found.
	 */
	int runScript(const char* script, int argc, const char** argv);
	
};

}

#endif // STEAMPLUS_H