// ctest.cpp ///////////////////////////////////////////////////////////////////
// test basic functions of VV2 geometry stuff via the command //////////////////
// line on centos //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"

//#include "Source.c"

//#include "Headers.hpp"
//#include "Source.cpp"
#include "VectorVictor2.hpp"
#include <iostream>
#include <string>

typedef unsigned int integer;

integer inputInteger(std::string prompt);

std::string inputString(std::string prompt);


int main()
{	integer hello = -1;
	hello = inputInteger("Please Input Integer Number: ");
	std::cout << hello << std::endl;
	return 0;
}


integer inputInteger(std::string prompt)
{
	while(true)
	{	std::cout << prompt;
		integer data;
		std::cin >> data;
		if(!std::cin.fail())
		{	return data;
		}
		else
		{	while(std::cin.fail())
			{	std::cin.clear();
			}
		}
		std::cin.ignore();
		std::cout << "\n" << std::endl;
	}
}


std::string Input_string(std::string prompt)
{
	std::string data = "";
	while(true)
	{	std::cout << prompt;
		while(std::getline(std::cin, data))
		{	if(data.size() == 0)
			{	continue;
			}
			else
			{	return data;
			}
		}
	}
}
