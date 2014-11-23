// Ignition.h /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Just switchboard for the Ignition Engine ////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#define DEBUG

#ifdef DEBUG
	#define SPILLBACK 
#endif

bool spill_frame = false;

#ifdef _WIN64
	#define WINDOWS
#elif _WIN32
	#define WINDOWS
#elif __APPLE__
	// this is unsupported for the moment
#elif __linux
	#define LINUX
#endif



#define STAR_WARS
//#define STAR_TREK
//#define LUNAR_LANDER

void Talkback(std::string console_output);
bool Spillback(std::string console_output);
bool Spillback(std::string prefix, long double variable_output);

#ifdef DEBUG
void Talkback(std::string console_output)
{	std::cout << console_output << std::endl;
}
	
#ifdef SPILLBACK

bool Spillback(std::string console_output)
{	if(spill_frame == true)
	{	std::cout << console_output << std::endl;
		return true;
	}
	else
	{	return false;
	}
}
bool Spillback(std::string prefix, long double variable_output)
{	std::cout.precision(20);	// measured precision of a long double I think
	if(spill_frame == true)
	{	std::cout << prefix << variable_output << std::endl;
		return true;
	}
	else
	{	return false;
	}
}
#endif
	
#endif

#ifndef DEBUG
void Talkback(std::string console_output)
{
}
#endif

#ifndef SPILLBACK
bool Talkback(std::string console_output)
{	return false;
}
bool Spillback(std::string console_output)
{	return false;
}
#endif

// A bit of explanation here. The Talkback system is designed for debugging (no way! you say). It only couts messages when the engine is compiled with the DEBUG flag uncommented
// The Spillback is a bit more complicated. It is designed to act as a debugging tool by couting desired outputs when the spill_frame variable is true. This is so that I can take a look at the
// guts of an individual frame in a coherent way instead of being indundated with outputs 60 times every second. Currently this is tied to the F key in SW::Starfighter 


