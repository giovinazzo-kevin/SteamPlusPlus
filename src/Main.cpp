#include <stdio.h>
#include <vector>

#include "Main.h"

spp::SteamPlusPlus sppClient;
std::recursive_mutex sppClient_mtx;

//! Handles user input and is responsible for running scripts
static void runInputThread()
{
	char inbuf[BUFSIZ];
	while( sppClient.isRunning() ) { // isRunning is false when the sppClient is uninitialized so it's safe to call
		// Ask the user to input something
		spp::gets(inbuf, BUFSIZ, true);

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
		int exitCode;
		int ret = sppClient.createSandbox(argList[0], argc, &argList[0], &exitCode);
		if(ret != spp::k_EOK) {
			sppClient.destroySandbox(argList[0]);
			switch(ret)
			{
				case spp::k_EUninitialized:
					spp::printf(spp::kPrintError, "The SteamPlusPlus object is uninitialized.\n");
				break;
				case spp::k_EFileNotFound:
					spp::printf(spp::kPrintError, "Script \"%s\" does not exist.\n", argList[0]);
				break;
				case spp::k_EMemory:
					spp::printf(spp::kPrintError, "Script \"%s\" could not be allocated in memory.\n", argList[0]);
				break;
				case spp::k_EUnknown:
					spp::printf(spp::kPrintError, "Unknown error.\n");
				break;
				// Any other error should speak for itself through an error handler. Should...
				default:
					spp::printf(spp::kPrintError, "Aborting due to previous error. (%d)\n", ret);
				break;
			}
		} else {
			spp::printf(spp::kPrintBoring, "Script quit with exit code %d.\n", exitCode);
		}
	}
}


static void runCallbackThread() 
{
	CallbackMsg_t msg;
	while( sppClient.isRunning() && Steam_BGetCallback( sppClient.getSteamPipe(), &msg) ) // BGetCallback is (B)locking
	{
		/* The '0' callback has no meaning, and I'm replacing it to give it a new one.
		 * Any script that registers it, will have it called every tick. Like an infinite loop.
		 * This serves the purpose of keeping alive scripts which depend on their state, and also
		 * provides a way of creating non-blocking infinite loops.
		 * 
		 * If a script wants to commit sudoku, all it has to do is unregister all of its callbacks.
		 */
		if( msg.m_iCallback != 0 ) {
			spp::fireCallbacks( msg.m_iCallback, msg.m_pubParam );
		}
		
		Steam_FreeLastCallback(sppClient.getSteamPipe());
	}
}

//! Since Steam_BGetCallback is blocking, this thread fires the special callback '0' to all the scripts
static void runTickThread()
{
	while( sppClient.isRunning()  ) 
	{
		if ( spp::fireCallbacks( 0, 0 ) == 0 ) {
			// If no callbacks were fired, let the CPU sleep for a while.
			sleep(16); 
		}
	}
}

int main(int argc, char** argv)
{
	spp::printf(spp::kPrintNormal, "Initializing Steamworks...\n");
	if( sppClient.initSteamworks() != spp::k_EOK ) {
		return 1;
	}
	spp::printf(spp::kPrintNormal, "\nWelcome to Steam++!\n");
	
	std::thread cbthread(runCallbackThread);
	std::thread tickthread(runTickThread);
	runInputThread();
	
	tickthread.join();
	cbthread.join();
	
	return 0;
}