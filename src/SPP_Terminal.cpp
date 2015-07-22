#include <stdio.h>
#include "SteamPlusPlus.h"

#ifdef _WIN32
#include <windows.h>
#endif

void setTerminalColor(int fgc)
{
#ifdef _WIN32
	// On Windows, use the provided functions for setting text attributes.
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hstdout, fgc);
#else
	// On UNIX, use the ANSI character sequence for coloring foreground text.
    printf("\033[3%dm", fgc);
#endif
}

int spp::vprintf(PrintMode printMode, const char* fmt, va_list args)
{
    setTerminalColor(printMode);
	switch(printMode)
	{
		case kPrintError:
			::printf("ERROR: ");
		break;
		case kPrintInfo:
		case kPrintNormal:
		break;
	}
    int ret = ::vprintf(fmt, args);
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
    if(displayHeader) {
		fputs("[Steam++]# ", stdout);
	}
    fgets(str, n, stdin);
    // Remove trailing newline, or don't if fgets returned because n was reached.
    int pos = strlen(str) - 1;
    if(str[pos] == '\n') {
		str[pos] = '\0';
	}
    return str;
}