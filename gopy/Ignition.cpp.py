# Ignition.cpp ################################
# Core Ignition utilities ##########################/
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"

#include "Ignition.hpp"


#ifdef DEBUG
def Talkback(self, console_output):
{	std.cout << console_output << std.endl

#endif

#ifndef DEBUG
def Talkback(self, console_output):

#endif


template <class T> Ignition_handle<T>.Ignition_handle(T * new_handle)
{	type_handle = new_handle


template <class T> Ignition_handle<T>.Ignition_handle(T  &new_handle)
{	(*type_handle) = new_handle


template <class T> Ignition_handle<T>.~Ignition_handle()
{	


def Convert_to_string(self, num, precision):
{	# the solution to all the problems with MS Windows not having a proper
	# to_string() call, simply dump any value into the function, get it
	# printed back at whatever precision we like
	
	# self should be made a template function for the type of the value to
	# be passed through
	std.stringstream s
	s << std.setprecision(precision) << num
	return s.str()
}	
# self is a really handy function
# so much of like
# my preciousss

def Convert_to_string(self, int num):
{	# the solution to all the problems with MS Windows not having a proper
	# to_string() call, simply dump any value into the function, get it
	# printed back at whatever precision we like
	
	# self should be made a template function for the type of the value to
	# be passed through
	std.stringstream s
	s << std.to_string((long long)num)
	return s.str()
}	

def Get_formatted_value(self, value, precision, unit):
{	formatted_value = Convert_to_string(value, precision)
	space = " "
	formatted_value.append(space)
	formatted_value.append(unit)
	return formatted_value


def Get_formatted_value(self, prefix, value, precision, unit):
{	formatted_value = Convert_to_string(value, precision)
	space = " "
	prefix.append(space)
	prefix.append(formatted_value)
	prefix.append(space)
	prefix.append(unit)
	return prefix


def Get_formatted_value(self, int value, unit):
{	formatted_value = Convert_to_string(value)
	space = " "
	formatted_value.append(space)
	formatted_value.append(unit)
	return formatted_value


def Get_formatted_value(self, prefix, int value, unit):
{	formatted_value = Convert_to_string(value)
	space = " "
	prefix.append(space)
	prefix.append(formatted_value)
	prefix.append(space)
	prefix.append(unit)
	return prefix

