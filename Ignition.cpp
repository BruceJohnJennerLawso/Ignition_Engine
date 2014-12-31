// Ignition.cpp ////////////////////////////////////////////////////////////////
// Core Ignition utilities /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"

#include "Ignition.hpp"


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


template <class T> Ignition_handle<T>::Ignition_handle(T * new_handle)
{	type_handle = new_handle;
}

template <class T> Ignition_handle<T>::Ignition_handle(T  &new_handle)
{	(*type_handle) = new_handle;
}

template <class T> Ignition_handle<T>::~Ignition_handle()
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

std::string Convert_to_string(long int num)
{	// the solution to all the problems with MS Windows not having a proper
	// to_string() call, we simply dump any value into the function, and get it
	// printed back at whatever precision we like
	
	// this should be made a template function for the type of the value to
	// be passed through
	std::stringstream s;
	s << std::to_string((long long)num);
	return s.str();
}	

std::string SI::Get_formatted_value(float value, int precision, std::string unit)
{	std::string formatted_value = Convert_to_string(value, precision);
	std::string space = " ";
	formatted_value.append(space);
	formatted_value.append(unit);
	return formatted_value;
}

std::string SI::Get_formatted_value(std::string prefix, float value, int precision, std::string unit)
{	std::string formatted_value = Convert_to_string(value, precision);
	std::string space = " ";
	prefix.append(space);
	prefix.append(formatted_value);
	prefix.append(space);
	prefix.append(unit);
	return prefix;
}

std::string SI::Get_formatted_value(long int value, std::string unit)
{	std::string formatted_value = Convert_to_string(value);
	std::string space = " ";
	formatted_value.append(space);
	formatted_value.append(unit);
	return formatted_value;
}

std::string SI::Get_formatted_value(std::string prefix, long int value, std::string unit)
{	std::string formatted_value = Convert_to_string(value);
	std::string space = " ";
	prefix.append(space);
	prefix.append(formatted_value);
	prefix.append(space);
	prefix.append(unit);
	return prefix;
}
