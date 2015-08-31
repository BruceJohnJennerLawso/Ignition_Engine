# VectorVictor2.h ##############################/
# A smallish 2d vector math library used for the ignition engine#######/
########################################
##include <std_files>
##include "Headers.h"
##include "Source.cpp"
#include "Ignition.hpp"
#include <vector>

#ifndef VectorVictor2
#define VectorVictor2

##define VECTORVICTOR_DEBUG

#define Pi 	3.14159265358979323846
#define Tau 6.283185308								
# Hopefully self doesnt anger the anti-tau-ans. ooh wait, antautansnot  Or
# maybe just anti-tauntans 
# anti-tauntans: smell even better on the insidenot 
#define Pau 4.712388980

# The VectorVictor namespace #########################
########################################

namespace VectorVictor
{	class Vector2	
	{	# core entity of VectorVictor for now
		public:
		Vector2 ()
		# Init the vector as (0,0)
		Vector2 (long double ix, double iy)
		# Init the vector with the supplied values
		
		#private:	# it really shouldnt be like self, my patience is unbelievably low right now. The calls that are directly accessing the position need to be rewritten to use the functions at a later date
		long double x, y
		# the actual data itself, as a long double
		public:
		void Set_x (long double ix)
		void Set_y (long double iy)
		void Set_values(long double ix, double iy)
		# exactly what it sounds like
		long double Get_x()
		long double Get_y()
		# return the values of each element in the vector	
		std.string Get_vector(std.string unit)
		# returns a string nicely formatted with units and si prefixes
		# helpful for data retrieval
		Vector2 Vector_normal();	
		# returns the normalized version of the vector (magnitude of 1)
		void Normalize();	
		# Normalizes the vector itself														
		void Rotate_vector(long double rotation)
		# Rotate the vector clockwise by rotation in degrees					
		Vector2 Get_rotated_vector(long double rotation)
		# returns a copy of the rotated vector		
		long double Get_vector_magnitude();	
		# returns the length of the vector						
		long double Get_vector_magnitude_squared()
		# returns the length of the vector, squared so we dont need to
		# run an expensive call to sqrt()			
		Vector2 operator + ( Vector2);							
		Vector2 operator += ( Vector2)
		# addition operators, what they sound like
		# components are added along each axis							
		Vector2 operator - ( Vector2);							
		Vector2 operator -= ( Vector2);	
		# subtraction operators, behaviour as 						
		operator = ( Vector2 v);	
		# copy operator						
		
		Vector2 operator * ( double c);	
		Vector2 operator *= ( double c)
		# multiply each element by some value						
		bool operator != ( Vector2 v);							
		bool operator == ( Vector2 v);	
		# comparison operators, == assumes all elements are the same, and
		# != that one or more are required				
		bool Is_nan()
		# try to compare each element of the vector against itself, return
		# True if comparing it against itself fails to indicate that shit
		# went out the window (apparently self is a fundamental thing about
		# nan in C)	
		#ifdef VECTORVICTOR_DEBUG
		bool Nan_state
		# if we arent debugging actively, dont particularly care much about
		# flagging stuff like self, Nan_state and any calls that try to
		# flag where stuff is going nan are only defined if VECTORVICTOR_DEBUG
		# is defined as well. Performances are obviously quite important here
		# in vector maths, we dont wanna be wasteful if we can avoid it
		#endif
		bool Flag_nan(std.string message, & initial_state);	
		~Vector2(void);						
	}

	long double Get_dot_product(long double x1, double y1, double x2, double y2)
	# the dot product of the two values, by accepting parameters
	# independently
	long double Get_dot_product(Vector2 First_vector, Second_vector)
	# and the same thing, done with the objects passed through
	long double Get_vector_angle(Vector2 First_vector, Second_vector)
	# returns the angle in radians, odd, degrees are better for return
	# types
	long double Get_cross_product(Vector2 First_vector, Second_vector)
	# obviously we cant do a cross product in two space, self actually
	# does is pretend the vectors are 3D vectors lying flat in the xy plane,
	# then calculate the z component of their cross and return it. This is
	# needed to calculate the magnitude of the Torque vector for a given force
	# by getting the cross of the Force with the radial vector, definition
	# of torque
	long double Get_vector_separation(Vector2 First_vector, Second_vector)
	# Not sure self was ever even used, I dont think it can hurt anything..
}

long double Absolute_value(long double value)


long double Minimum(long double value1, double value2)
long double Maximum(long double value1, double value2)
bool In_range(long double value1, double value2, double target_value)

long double Smallest_value(std.vector<long double> &list)

#endif
