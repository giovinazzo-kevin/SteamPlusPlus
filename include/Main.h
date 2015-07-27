#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <thread>
#include <mutex>

#include "SteamPlusPlus.h"

extern spp::SteamPlusPlus sppClient;
extern std::recursive_mutex sppClient_mtx;
#endif // MAINLOOP_H