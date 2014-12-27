// Ignition.h //////////////////////////////////////////////////////////////////
// Just switchboard for the Ignition Engine ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

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

template <class T> class Ignition_handle
{	public:
	Ignition_handle(T * new_handle);
	Ignition_handle(T &new_handle);
	T * type_handle;
	~Ignition_handle();
};

// this is just archived for now

void Talkback(std::string console_output);
// a quick function that can be used for printing debugging messages to the
// console. This really should have a few more overloads for long doubles & the
// like though

std::string Convert_to_string(float num, int precision);
std::string Convert_to_string(long int num);

namespace SI
{	std::string Get_formatted_value(float value, int precision, std::string unit);
	std::string Get_formatted_value(std::string prefix, float value, int precision, std::string unit);
	
	std::string Get_formatted_value(long int value, std::string unit);
	std::string Get_formatted_value(std::string prefix, long int value, std::string unit);
}

#endif
