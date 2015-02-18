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

#define Max_map_scale 16
// the maximum scale of the map view from the base window size, ie if the base
// window has a width of 1024 pixels, its maximum zoom scale will encompass
// 1024*(10^16) meters in universe. Thats about 100 light years or so if I
// recall correctly, or 1000? The scale might only go up to 10^15 or something
// in actual practice

// I should look that up

#define Min_map_scale 1

#define Max_cam_scale 100000.00
#define Min_cam_scale 0.01

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
