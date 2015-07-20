#ifndef STEAMPLUS_H
#define STEAMPLUS_H

#include <cstdarg> //va_list
#include <stdio.h>
#include <string.h>

namespace spp
{
extern const char* kUserInputHeader;

enum PrintMode {
#ifdef _WIN32
    kPrintNormal = 0xF,
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

// Both return the number of characters written.
int printf(PrintMode printMode, const char* fmt, ...);
int vprintf(PrintMode printMode, const char* fmt, va_list args);

// Reads a line, returning back str. If displayHeader == true, preprends the line with kUserInputHeader.
char* gets(char* str, size_t n, int displayHeader = true);

// Runs a script, returning its sid, or fails and returns -1.
int runscript(const char* script, int argc, const char** argv);
}

#endif // STEAMPLUS_H