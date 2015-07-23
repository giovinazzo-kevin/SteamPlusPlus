#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "SteamPlusPlus.h"
#include "SPP_Terminal.h"

std::mutex spp::gets_mtx; 
bool spp::headerDisplaying = false;
const char* spp::header = "[Steam++]# ";

void spp::setTerminalColor(int fgc)
{
#ifdef _WIN32
	// On Windows, use the provided functions for setting text attributes.
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, fgc);
#else
	// On UNIX, use the ANSI character sequence for coloring foreground text.
    printf("\033[%dm", fgc);
#endif
}

int spp::vprintf(PrintMode printMode, const char* fmt, va_list args)
{
	FILE* stream = stdout;
	
    setTerminalColor(printMode);
	
	switch(printMode)
	{
		case kPrintError:
			::printf("ERROR: ");
			stream = stderr;
		break;
		default:
		break;
	}
	
    int ret = ::vfprintf(stream, fmt, args);
	setTerminalColor(spp::kPrintNormal);
	
    return ret;
}

int spp::printf(PrintMode printMode, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = spp::vprintf(printMode, fmt, args);
    va_end(args);
    return ret;
}

char* spp::gets(char* str, size_t n, bool displayHeader)
{
	setTerminalColor(spp::kPrintBoring);
	if( displayHeader ) {
		gets_mtx.lock();
		headerDisplaying = true;
		fputs(header, stdout);
		gets_mtx.unlock();
	}
	
    fgets(str, n, stdin);
	
	gets_mtx.lock();
	headerDisplaying = false;
	gets_mtx.unlock();
	
    // Remove trailing newline, or don't if fgets returned because n was reached.
    int pos = strlen(str) - 1;
    if(str[pos] == '\n') {
		str[pos] = '\0';
	}
	setTerminalColor(spp::kPrintNormal);
    return str;
}