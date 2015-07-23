#include <stdio.h>
#include <vector>

#include "Main.h"

const char* kScriptsFolderPath = "scripts/";

spp::SteamPlusPlus sppClient;
std::mutex sppClient_mtx;

//! Handles user input and is responsible for running scripts
void runInputLoop()
{
	char inbuf[BUFSIZ];
	while( sppClient.isRunning() ) { // isRunning is false when the sppClient is uninitialized so it's safe to call
		// Ask the user to input something
		spp::gets(inbuf, BUFSIZ, true);
		
		// Flush the input buffer. fflush(stdin) yields undefined behaviour on input streams and shouldn't be used.
		// char c_; while((c_ = getchar()) != '\n' && c_ != EOF); 

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
		char pathBuf[ strlen(argList[0]) + strlen(kScriptsFolderPath) + 5]; // + 5 = Additional 4 bytes in case ".lua" needs to be appended and terminator.
		sprintf(pathBuf, "%s%s", kScriptsFolderPath, argList[0]);
		// If the entered path doesn't end in .lua, add it
		if( strstr(pathBuf, ".lua") != pathBuf + strlen(pathBuf) - 4 ) {
			strcat(pathBuf, ".lua");
		}
		
		// Try to run the script, passing it the arguments
		// Please note that arguments are passed exactly like in C and C++, with the name of the executable being the first argument.
		int exitCode;
		sppClient_mtx.lock();
		int ret = sppClient.runScript(pathBuf, argc, &argList[0], &exitCode);
		sppClient_mtx.unlock();
		if(ret != spp::k_EOK) {
			sppClient_mtx.lock();
			sppClient.killScript(pathBuf);
			sppClient_mtx.unlock();
			switch(ret)
			{
				case spp::k_EUninitialized:
					spp::printf(spp::kPrintError, "The SteamPlusPlus object is uninitialized.\n");
				break;
				case spp::k_EFileNotFound:
					spp::printf(spp::kPrintError, "Script \"%s\" does not exist.\n", pathBuf);
				break;
				case spp::k_EMemory:
					spp::printf(spp::kPrintError, "Script \"%s\" could not be allocated in memory.\n", pathBuf);
				break;
				case spp::k_EUnknown:
					spp::printf(spp::kPrintError, "Unknown error.\n");
				break;
				// Any other error should speak for itself through an error handler. Should...
				default:
					spp::printf(spp::kPrintError, "Aborting due to previous error (%d).\n", ret);
				break;
			}
		} else {
			spp::printf(spp::kPrintBoring, "Script quit with exit code (%d).\n", exitCode);
		}
	}
}


void runCallbackLoop() 
{
	CallbackMsg_t msg;
	
	while( sppClient.isRunning() ) // isRunning is false when the sppClient is uninitialized so it's safe to call
	{
		while( Steam_BGetCallback( sppClient.getSteamPipe(), &msg) )
		{
			sppClient_mtx.lock();
			sppClient.fireCallbacks( msg.m_iCallback, msg.m_cubParam, msg.m_pubParam );
			sppClient_mtx.unlock();
			Steam_FreeLastCallback(sppClient.getSteamPipe());
		}
		
		sleep(100);
	}
}

int main(int argc, char** argv)
{
	spp::printf(spp::kPrintNormal, "Initializing Steamworks...\n");
	if( sppClient.initSteamworks() != spp::k_EOK ) {
		return 1;
	}
	spp::printf(spp::kPrintNormal, "\nWelcome to Steam++!\n");
	
	std::thread cbthread(runCallbackLoop);
	runInputLoop();
	cbthread.join();
	
    return 0;
}