#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <thread>
#include <mutex>

#include "SteamPlusPlus.h"

extern const char* kScriptsFolderPath; //! The path to the folder where scripts are stored by default.

extern spp::SteamPlusPlus sppClient;
extern std::mutex sppClient_mtx;
#endif // MAINLOOP_H