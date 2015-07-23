#ifndef SPP_TERMINAL_H
#define SPP_TERMINAL_H
#include <mutex>

namespace spp
{
extern std::mutex gets_mtx; 
extern bool headerDisplaying;
extern const char* header;

void setTerminalColor(int fgc);
}

#endif