#include <stdio.h>
#include <vector>
#include <thread>

#include "SteamPlusPlus.h"
#include "Main.h"

const int kInputBufferSize = 512;
const char* kScriptsFolderPath = "scripts/";

//! Handles user input and is responsible for running scripts
void runInputLoop(spp::SteamPlusPlus& client)
{
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
		
		// Prepend the scripts folder's location to the first argument
		char pathBuf[ strlen(argList[0]) + strlen(kScriptsFolderPath) ];
		sprintf(pathBuf, "%s%s", kScriptsFolderPath, argList[0]);
		
		// Try to run the script, passing it the arguments
		// Please note that arguments are passed exactly like in C and C++, with the name of the executable being the first argument.
		int ret = client.runScript(pathBuf, argc, &argList[0]);
		if(ret != spp::kE_OK) {
			spp::printf(spp::kPrintNormal, "(%d) ", ret);
			switch(ret)
			{
				case spp::kE_Uninitialized:
					spp::printf(spp::kPrintError, "The SteamPlusPlus object is uninitialized.\n");
				break;
				case spp::kE_FileNotFound:
					spp::printf(spp::kPrintError, "Script \"%s\" does not exist.\n", argList[0]);
				break;
				case spp::kE_Memory:
					spp::printf(spp::kPrintError, "Script \"%s\" could not be allocated in memory.\n", argList[0]);
				break;
				case spp::kE_Unknown:
					spp::printf(spp::kPrintError, "Unknown error.\n");
				break;
				// Any other error should speak for itself through an error handler. Should...
				default:
					spp::printf(spp::kPrintError, "Aborting due to previous error.\n");
				break;
			}
		}
    }
}

//! Iterates each script and calls their callbacks whenever appropriate
void runScriptCallbackLoop(spp::SteamPlusPlus& client)
{
	
}

int main(int argc, char** argv)
{
	spp::SteamPlusPlus client;
    spp::printf(spp::kPrintInfo, "Welcome to Steam++!\n");
	
	//std::thread sclThread(runScriptCallbackLoop, client);
	runInputLoop(client);
	//sclThread.join();
    return 0;
}
