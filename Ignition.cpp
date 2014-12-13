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

