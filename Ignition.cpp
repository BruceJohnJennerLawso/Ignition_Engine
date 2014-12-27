// Ignition.cpp ////////////////////////////////////////////////////////////////
// Core Ignition utilities /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"

#include "Ignition.h"


#ifdef DEBUG
void Talkback(std::string console_output)
{	std::cout << console_output << std::endl;
}
#endif

#ifndef DEBUG
void Talkback(std::string console_output)
{
}
#endif


Ignition_handle::Ignition_handle(T * new_handle)
{	type_handle = new_handle;
}

Ignition_handle::Ignition_handle(T  &new_handle)
{	(*type_handle) = new_handle;
}

Ignition_handle::~Ignition_handle()
{	
}

std::string Convert_to_string(float num, int precision)
{	// the solution to all the problems with MS Windows not having a proper
	// to_string() call, we simply dump any value into the function, and get it
	// printed back at whatever precision we like
	
	// this should be made a template function for the type of the value to
	// be passed through
	std::stringstream s;
	s << std::setprecision(precision) << num;
	return s.str();
}	
// this is a really handy function
// so much of like
// my preciousss

std::string SI::Get_formatted_value(float value, int precision, std::string unit)
{	
}
