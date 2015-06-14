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

typedef int integer;

integer inputInteger(std::string prompt);

std::string inputString(std::string prompt);


int main()
{	//integer hello = -1;
	//hello = inputInteger("Please Input Integer Number: ");
	//std::cout << hello << std::endl;
	
	VectorVictor::Vector2 dir1(inputInteger("pair 1 dir x: "),inputInteger("pair 1 dir y: "));
	VectorVictor::Vector2 pos1(inputInteger("pair 1 pos x: "), inputInteger("pair 1 pos y: "));
	
	VectorVictor::Vector_pair pair1(pos1, dir1);
	
	
	VectorVictor::Vector2 dir2(inputInteger("pair 2 dir x: "),inputInteger("pair 2 dir y: "));
	VectorVictor::Vector2 pos2(inputInteger("pair 2 pos x: "), inputInteger("pair 2 pos y: "));
	
	VectorVictor::Vector_pair pair2(pos2, dir2);	
	

	if(dir1.Antiparallel(dir2) == true)
	{	std::cout << "Directions are parallel" << std::endl;
	}
	else
	{	std::cout << "pair 1 pos & dir: ( " << pos1.Get_vector("u") << " " << dir1.Get_vector("u") << " ) pair 2 pos & dir: ( " << pos2.Get_vector("u") << " " << dir2.Get_vector("u") << std::endl; 
	}
	std::cout << Orientation_as_text(pair1.Orientation(pair2)) << std::endl;
	
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
