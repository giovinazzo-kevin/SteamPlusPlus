#ifndef STEAMPLUS_H
#define STEAMPLUS_H

#include <cstdarg>

namespace spp
{
	extern const char *kUserInputHeader;
	
	enum PrintMode
	{
		#ifdef _WIN32
		kPrintNormal = 0x7,
		#else
		kPrintNormal = 0x7,
		#endif
		
		#ifdef _WIN32
		kPrintError = 0xC,
		#else
		kPrintError = 0x1,
		#endif
		
		#ifdef _WIN32
		kPrintInfo = 0xE
		#else
		kPrintInfo = 0x3
		#endif
	};
	
	/// Returns the number of characters written
	int printf(PrintMode printMode, const char *fmt, ...);
}

#endif //STEAMPLUS_H