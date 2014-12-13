// Ignition.h //////////////////////////////////////////////////////////////////
// Just switchboard for the Ignition Engine ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#ifndef Ignitionh
#define Ignitionh
// header guard cant use Ignition since thats the method name later on

#define DEBUG


#ifdef _WIN64
	#define WINDOWS
#elif _WIN32
	#define WINDOWS
#elif __APPLE__
	// this is unsupported for the moment
#elif __linux
	#define LINUX
#endif


void Talkback(std::string console_output);
//bool Spillback(std::string console_output);
//bool Spillback(std::string prefix, long double variable_output);

// A bit of explanation here. The Talkback system is designed for debugging (no way! you say). It only couts messages when the engine is compiled with the DEBUG flag uncommented
// The Spillback is a bit more complicated. It is designed to act as a debugging tool by couting desired outputs when the spill_frame variable is true. This is so that I can take a look at the
// guts of an individual frame in a coherent way instead of being indundated with outputs 60 times every second. Currently this is tied to the F key in SW::Starfighter 

#endif
