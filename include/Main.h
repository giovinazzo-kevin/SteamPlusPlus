#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "SteamPlusPlus.h"

extern const int kInputBufferSize; //! The maximum number of characters to read from the command line. Any excess is discarded.
extern const char* kScriptsFolderPath; //! The path to the folder where scripts are stored by default.

	
/**
 * @brief Calls registerCallback on the active SteamPlusPlus instance.
 * @param cbID The event to intercept.
 * @param L The lua_State that will get its callback called.
 * @param cbname The name of the LUA function that defines the callback.
 * @return k_EOK on success, k_EFail on failure (callback already registered?).
 */
int registerCallbackProxy(int cbID, lua_State* L, const char* cbname);

/**
 * @brief Calls unregisterCallback on the active SteamPlusPlus instance.
 * @param cbID The event to unregister.
 * @param L The lua_State.
 * @return k_EOK on success, k_EFail on failure (callback not registered?).
 */
int unregisterCallbackProxy(int cbID, lua_State* L);

#endif // MAINLOOP_H