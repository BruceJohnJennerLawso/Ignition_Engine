// Ignition.h //////////////////////////////////////////////////////////////////
// Just switchboard for the Ignition Engine ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

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
// a quick function that can be used for printing debugging messages to the
// console. This really should have a few more overloads for long doubles & the
// like though

std::string Convert_to_string(float num, int precision);

namespace SI
{	std::string Get_formatted_value(float value, int precision, std::string unit);
}

#endif
