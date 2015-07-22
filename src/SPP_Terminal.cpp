#include <stdio.h>
#include <mutex>
#ifdef _WIN32
#include <windows.h>
#endif

#include "SteamPlusPlus.h"

static void setTerminalColor(int fgc)
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

static const char* header = "[Steam++]# ";
static bool isHeaderShowing = false;
static std::mutex headerShowing_mtx;


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
	
	// If the [Steam++] header is showing, but a script wants to print, we have to delete it, let the script print, and rewrite it.
	headerShowing_mtx.lock();
	if( isHeaderShowing ) {
		int len = strlen(header);
		while(len--) 
		{
			fputc('\b', stdout);
		}
	}
	headerShowing_mtx.unlock();
	
    int ret = ::vfprintf(stream, fmt, args);
    setTerminalColor(spp::kPrintNormal);
	
	headerShowing_mtx.lock();
	if( isHeaderShowing ) {
		setTerminalColor(spp::kPrintBoring);
		fputs(header, stdout);
	}
	headerShowing_mtx.unlock();
	
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
    if(displayHeader) {
		headerShowing_mtx.lock();
		fputs(header, stdout);
		isHeaderShowing = true;
		headerShowing_mtx.unlock();
	}
    fgets(str, n, stdin);
    // Remove trailing newline, or don't if fgets returned because n was reached.
    int pos = strlen(str) - 1;
    if(str[pos] == '\n') {
		str[pos] = '\0';
	}
	setTerminalColor(spp::kPrintNormal);
	headerShowing_mtx.lock();
	isHeaderShowing = false;
	headerShowing_mtx.unlock();
    return str;
}