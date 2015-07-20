#include <stdio.h>
#include <vector>

#include "SteamPlusPlus.h"
#include "MainLoop.h"

const int kInputBufferSize = 512;

int main(int argc, char** argv)
{
    spp::printf(spp::kPrintInfo, "Welcome to Steam++!\n");

    char inbuf[kInputBufferSize];
    while(1) {
		// Ask the user to input something
		fflush(stdin);
		spp::gets(inbuf, kInputBufferSize, true);

		// Split the entered string into tokens, or arguments
		std::vector<const char*> argList;
		char* arg = strtok(inbuf, " ");
		while(arg != NULL) {
			argList.push_back(arg);
			arg = strtok(NULL, " ");
		}

		int argc = argList.size();
		// argc == 0 when no tokens were extracted, i.e. the command line was empty
		if(argc == 0) {
			continue;
		}
		
		// Try to run the script, passing it the arguments
		// Please note that arguments are passed exactly like in C and C++, with the name of the executable being the first argument.
		int ret = spp::runscript(argList[0], argc, &argList[0]);
		if(ret == -1) {
			spp::printf(spp::kPrintError, "Script \"%s\" does not exist.\n", argList[0]);
		}
    }

    return 0;
}
