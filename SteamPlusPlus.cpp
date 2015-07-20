#include "stdio.h"

#include "SteamPlusPlus.h"
#ifdef _WIN32
#include "windows.h"
#endif

void setTerminalColor( int fgc )
{
	#ifdef _WIN32
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( hstdout, fgc );
	#else
	printf("\033[%dm", fgc);
	#endif
}

const char *spp::kUserInputHeader = "[Steam++] ";

int spp::printf( PrintMode printMode, const char *fmt, ... )
{
	va_list args;
	va_start(args, fmt);
	
	setTerminalColor( printMode);
	int ret =  vprintf(fmt, args);
	setTerminalColor( spp::kPrintNormal );
	
	va_end(args);
	return ret;
}