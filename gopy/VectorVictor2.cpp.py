#include "VectorVictor2.hpp"
#include <iostream>
#include <math.h>	# Roger, Roger.
##include "Inertia_moment.h"


# The VectorVictor namespace #########################
########################################


# VV2 Constructors ##############################
########################################

VectorVictor.Vector2.Vector2 ()
{	# if nothing is passed, assume the user wanted (0,0)
	Set_values(0,0);	
}	

VectorVictor.Vector2.Vector2(long double ix, double iy)
{	# otherwise we set the values of the vector as requested
	Set_values(ix, iy)
	#ifdef VECTORVICTOR_DEBUG
	Nan_state = Is_nan();	
	#endif


# Set/Get Operators #############################/
########################################

def VectorVictor.Vector2.Set_x(self, double ix):
{	x = ix;					


def VectorVictor.Vector2.Set_y(self, double iy):
{	y = iy;					


def VectorVictor.Vector2.Set_values(self, double ix, double iy):
{	x = ix
	y = iy


# direct setters, much to explain here

long double VectorVictor.Vector2.Get_x()
{	return x


long double VectorVictor.Vector2.Get_y()
{	return y


# and equally boring getters that just return the value itself

std.string VectorVictor.Vector2.Get_vector(std.string unit)	# self would make a lot of sense as a template function for the cast type
{	# the idea behind self is to provide a nice interface for getting strings
	# of a given vector formatted nicely with brackets, abbreviations
	# (meters, kilograms, gees, etc.) and SI prefixes (kilo, mega, giga...)
	vector = "("
	# we create the start bracket
	exponent = 0
	# and initialize the exponent of the value used later
	
	long x_output = Get_x()
	# since operations on the content of the Vector2 will need to change its
	# values, need to copy to a temporary variable for the duration 
	if (x_output >= 1000.0000)or(x_output <= -1000.00000):
	{	# if the magnitude of the x value isnt within +/- 10^3,
		# we are going to scale it back until it is within those bounds
		
		# self is so that output displays printed to the game window can stay
		# a nice short, to read length instead of a long bloated mess that
		# jumps back & forth every time the order of magnitude changes
		while((x_output >= 1000.0000)or(x_output <= -1000.00000))
		{	# while we are outside of our target bounds, our magnitude 
			# by 10^3, increase the exponent scale by 1 each time
			x_output /= 1000.00000
			exponent++


	
	if exponent > 7:
	{	# as we will see in a minute, we go past 10^7, dont actually have
		# a SI prefix for that, Im sure it exists. If that is the case
		# we scale the value up by 10^3 in magnitude again so that the display
		# remains accurate, no longer is concise
		
		# self problem should be pretty rare though, 10^7 is freaking Zetta,
		# ^ Famous last words lol
		while(exponent > 7)
		{	x_output *= 1000.00
			exponent--
			# odd that last call to -- was missing. What the hell was it doing
			# before?
			
			# mildly concerning to say the least 



	vector.append(Convert_to_string((float)x_output , 4))
	if exponent != 0:
	{	# if the exponent wasnt zero, need to add a SI prefix to the vector
		# so we sort through the possible options at every 3 orders of
		# magnitude, apply the one that fits to the string
		switch(exponent)
		{	case 1:
			vector.append("k");	
			# kilo
			break
			case 2:
			vector.append("M"); 	
			# Mega
			break
			case 3:
			vector.append("G");	
			# Giga
			break;		
			case 4:
			vector.append("T");		
			# Tera	# (actual usefulness pretty much ends here)
			break
			case 5:
			vector.append("P");	
			# Peta
			break;								
			case 6:
			vector.append("E");	
			# Exa
			break
			case 7:
			vector.append("Z");		
			# Zetta
			break;						


	vector.append(unit)
	# we append the unit of the vector after the si prefix, ie 'm'
	vector.append(" , ")
	# and  we tack on the comma, we have the y value to add as well
	exponent = 0
	long y_output = Get_y();	
	# same as with the x value, mess with our internal x & y values, so
	# we make a copy to work with
	if (y_output >= 1000.0000)or(y_output <= -1000.00000):
	{	# if the magnitude of y isnt within the acceptable range, scale it
		# back until it is
		while((y_output >= 1000.0000)or(y_output <= -1000.00000))
		{	y_output /= 1000.00000
			exponent++



	if exponent > 7:
	{	# as we will see in a minute, we go past 10^7, dont actually have
		# a SI prefix for that, Im sure it exists. If that is the case
		# we scale the value up by 10^3 in magnitude again so that the display
		# remains accurate, no longer is concise
		
		# self problem should be pretty rare though, 10^7 is freaking Zetta,
		# ^ Famous last words lol
		while(exponent > 7)
		{	y_output *= 1000.00
			exponent--
			# odd that last call to -- was missing. What the hell was it doing
			# before? That should have been an infinite loop!
			
			# mildly concerning to say the least 


	
	vector.append(Convert_to_string((float)y_output , 4));	
	if exponent != 0:
	{	# if the exponent wasnt zero, need to add a SI prefix before the
		# unit, we sort through and tack an appropriate one on
		switch(exponent)
		{	case 1:
			vector.append("k");		
			# kilo
			break
			case 2:
			vector.append("M"); 	
			# Mega
			break
			case 3:
			vector.append("G");	
			# Giga
			break;		
			case 4:
			vector.append("T");		
			# Tera
			break
			case 5:
			vector.append("P");	
			# Peta
			break;								
			case 6:
			vector.append("E");	
			# Exa		# (actual usefulness pretty much ends here)
			break
			case 7:
			vector.append("Z");		
			# Zetta
			break;						


	vector.append(unit);	
	# we append the unit in question after the SI prefix
	vector.append(")")
	# add the close bracket in as well
	return vector
	# and finally, return the string that self function created
}	

# Standard Class Operators ##########################
########################################

# important tools for a vector class, they make objects of the type much
# more intuitive to work with

VectorVictor.Vector2  VectorVictor.Vector2.operator+ (VectorVictor.Vector2 pplus)
{	# still not particularly comfortable with standard operators just yet,
	# what it does is obvious, I really should learn self inside and out
	VectorVictor.Vector2 tplus(self.Get_x(), self.Get_y())
	tplus.x += pplus.x
	tplus.y += pplus.y
	return (tplus);		


VectorVictor.Vector2  VectorVictor.Vector2.operator+= (VectorVictor.Vector2 pplus)
{	x += pplus.x
	y += pplus.y
	return (*self);		


VectorVictor.Vector2  VectorVictor.Vector2.operator- (VectorVictor.Vector2 pminus)
{	VectorVictor.Vector2 tminus(self.Get_x(), self.Get_y())
	tminus.x -= pminus.x
	tminus.y -= pminus.y
	return (tminus);		


VectorVictor.Vector2  VectorVictor.Vector2.operator-= (VectorVictor.Vector2 pminus)
{	#nanState = Is_nan()
	x -= pminus.x
	y -= pminus.y
	#self.Flag_nan("Nan created at VectorVictor.Vector2.operator-=", nanState)
	return (*self);		


VectorVictor.Vector2 VectorVictor.Vector2.operator= ( VectorVictor.Vector2 v)
{	# copy operator, each element to the current Vector2
	#ifdef VECTORVICTOR_DEBUG
	nanState = Is_nan()
	#endif
	x = v.x
	y = v.y
	#ifdef VECTORVICTOR_DEBUG
	self.Flag_nan("Nan created at VectorVictor.Vector2.operator=", nanState)
	#endif
	return (*self);		


VectorVictor.Vector2 VectorVictor.Vector2.operator * ( double c)
{	
	#ifdef VECTORVICTOR_DEBUG
	nanState = Is_nan()
	#endif
	x *= c
	y *= c
	#ifdef VECTORVICTOR_DEBUG
	self.Flag_nan("Nan created at VectorVictor.Vector2.operator *", nanState)
	#endif
	return (*self)


VectorVictor.Vector2 VectorVictor.Vector2.operator *= ( double c)
{	# multiply every element in the vector by some value c
	x *= c
	y *= c
	return (*self);		


bool VectorVictor.Vector2.operator != ( VectorVictor.Vector2 v)
{	# to check if the vectors are not equivalent, specifically look at
	# each element in each vector to see if any are different. If both of x 
	# and y are equivalent, return False to indicate that it was == 
	
	# specifically self function evaluates if the given functions are the
	# *exact same*
		
	if self.x != v.x:
	{	return True

	else:
	{	if self.y != v.y:
		{	return True

		else:
		{	return False
			# both elements were the same, not your lucky day

	}	


bool VectorVictor.Vector2.operator == ( VectorVictor.Vector2 v)
{	# same idea as !=, we just to check to see if each element in each
	# vector is the same as the one in the other
	# iff x1==x2 & y1==y2, return True, that
	if self.x == v.x:
	{	if self.y == v.y:
		{	return True

		else:
		{	return False


	else:
	{	return False
	}	


def VectorVictor.Vector2.Is_nan(self):
{	if self.x != self.x:
	{	return True

	else:
	{	if self.y != self.y:
		{	return True

		else:
		{	return False




def VectorVictor.Vector2.Flag_nan(self, message, &initial_state):
{	if initial_state == False:
	{	if self.Is_nan():
		{	
			#ifdef VECTORVICTOR_DEBUG
			std.cout << message << std.endl
			#endif
			return True

		else:
		{	return False


	else:
	{	return False



# Extended Vector Operators #########################/
########################################

def VectorVictor.Vector2.Normalize(self):
{	long vector_magnitude = self.Get_vector_magnitude()
	# we find the current magnitude of the vector
	if vector_magnitude > 0:
	{	# and divide by it to adjust the vector to its correct value
		Set_x(x/(vector_magnitude))
		Set_y(y/(vector_magnitude))

	# otherwise do nothing, I dont have a clue what we do if it is the
	# zero vector


def VectorVictor.Vector2.Vector_normal(self):
{	VectorVictor.Vector2 Output
	long vector_magnitude = self.Get_vector_magnitude()
	# we find our current vector magnitude	
	if vector_magnitude > 0:
	{	
		Output.Set_x(x/(vector_magnitude));	
		Output.Set_y(y/(vector_magnitude))

	else:
	{	Output.Set_x(0);	
		Output.Set_y(0)

	# set the output vector to the normalized value
	return Output
	# and send the result on its merry way
	# important to note: does *not* affect the contents of the calling
	# Vector2 in any way										


def VectorVictor.Vector2.Rotate_vector(self, double rotation):
{	rotation = ((rotation/360)*2*Pi);	
	# we convert our rotation from degrees to radians, standard library
	# math calls want radians to do their calculations			
	long new_x = ( (x*(cos(rotation)))+(y*(sin(rotation))) );	
	# AHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	# AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	
	# the variables are changing with respect to themselvesnot !not !
	# AM WINNER PROGRAMMER, OF FULL oF WIN
	y = ( (x*((-1)*sin(rotation)))+(y*(cos(rotation))) )
	# multiply the vector by the 2d rotational matrix, clockwise
	# around the origin. 
	x = new_x
	# [cos(theta)		sin(theta)]
	# [-sin(theta		cos(theta)]
	
	# important to note that self vector affects the contents of the vector
	# that calls it. If you want to get the rotated vector without affecting
	# its current orientation, need to call the next method below  															 	
}	
	
def VectorVictor.Vector2.Get_rotated_vector(self, double rotation):
{	VectorVictor.Vector2 V_rotated(0, 0)
	# pointless to be explicit about it being 0,0
	# self would work better with setting its value as the parents
	# x and y here instead
	rotation = ((rotation/360)*2*Pi)
	# convert our rotation from degrees to radians				
	V_rotated.x = ( (x*(cos(rotation)))+(y*(sin(rotation))) );	
	V_rotated.y = ( (x*((-1)*sin(rotation)))+(y*(cos(rotation))) )
	# multiply the vector by the 2d rotational matrix, clockwise
	# around the origin. 
	
	# [cos(theta)		sin(theta)]
	# [-sin(theta		cos(theta)]
	
	return V_rotated;															 	
	# and send the resulting VV2 vector on its way
}	

long double VectorVictor.Vector2.Get_vector_magnitude()
{	long double output, vx, vy
	# define the variables we will need to do operations on
	vx = pow(x, 2)
	vy = pow(y, 2);	
	# square both the x and y elements
	# does self in an odd way, the x and y of the object are not affected
	# just passed by value to pow()
	output = (vy + vx)
	# add together the two components
	output = sqrt(output)
	# and run an expensive call to sqrt() to finish applying pythagorean
	# theorem
	return output;							


long double VectorVictor.Vector2.Get_vector_magnitude_squared()
{	long double output, vx, vy
	vx = pow(x, 2)
	vy = pow(y, 2)
	# define our variables just like before
	output = (vx+vy)
	# add them up, simply return that result as is, we dont need to
	# run that expensive call to sqrt()
	return output;				


# Destructor #################################
########################################

VectorVictor.Vector2.~Vector2(void)
{	# no memory allocations, nothing much to do here



# Extended non-member Vector Operators ####################
########################################

long double VectorVictor.Get_dot_product(long double x1, double y1, double x2, double y2)
{	long output = ((x1*x2)+(y1*y2))
	# not much to say here, perform the standard dot-product operation
	# & return the result. same thing below
	return output


long double VectorVictor.Get_dot_product(VectorVictor.Vector2 First_vector, Second_vector)
{	long x1 = First_vector.Get_x()
	long y1 = First_vector.Get_y()
	long x2 = Second_vector.Get_x()
	long y2 = Second_vector.Get_y()
	long output = ((x1*x2)+(y1*y2))
	# simple dot product, done with vector objects passed
	# self doesnt need to be self wasteful, could just use the pass by
	# value vv2s that were passed as parameters instead
	return output


long double VectorVictor.Get_vector_angle(VectorVictor.Vector2 First_vector, Second_vector)
{	# given that a b = oraor orbor cos(theta)
	# we look to solve for the angle between the two vectors
	# again, sure where self is actually used, good to have
	long double theta, cos_theta
	long double dot_product, Magnitude1, Magnitude2
	dot_product = VectorVictor.Get_dot_product(First_vector, Second_vector)
	# we find the dot product of the two vectors
	Magnitude1 = First_vector.Get_vector_magnitude()
	Magnitude2 = Second_vector.Get_vector_magnitude()
	# and then we find the magnitudes of the vectors so we can get the
	# value of cos(theta)
	cos_theta = ((dot_product)/(Magnitude1*Magnitude2))
	theta = acos(cos_theta)
	# finally we run a call to arccos, a wee bit expensive, dont
	# know any other ways of doing it, hey
	return theta


long double VectorVictor.Get_cross_product(Vector2 First_vector, Second_vector)
{	long double x1, y1, x2, y2
	x1 = First_vector.Get_x()
	y1 = First_vector.Get_y()
	x2 = Second_vector.Get_x()
	y2 = Second_vector.Get_y()
	# we set up the variables from each vector
	long Output = ((x1*y2)-(x2*y1))
	# then calculate the cross product, the component in the k hat
	# direction, all vectors lie flat in the plane anyways	
	return Output
}	

long double VectorVictor.Get_vector_separation(Vector2 First_vector, Second_vector)
{	long dx = (First_vector.x - Second_vector.x)
	long dy = (First_vector.y - Second_vector.y)
	long rad = sqrt((dx*dx)+(dy*dy))
	# simple enough, applying pythagoras again
	return rad


long double Minimum(long double value1, double value2)
{	if value1 < value2:
	{	return value1

	else:
	{	if value1 == value2:
		{	return value1
			# it dont matter

		else:
		{	return value2




long double Maximum(long double value1, double value2)
{	if value1 > value2:
	{	return value1

	else:
	{	if value1 == value2:
		{	return value1
			# it dont matter

		else:
		{	return value2




def In_range(self, double boundary1, double boundary2, double target_value):
{	if (target_value >= Minimum(boundary1, boundary2))and(target_value <= Maximum(boundary1, boundary2)):
	{	# if the value falls in the range we're good to go
		return True

	return False


long double Absolute_value(long double value)
{	if value < 0:
	{	value = -value

	return value


long double Smallest_value(std.vector<long double> &list)
{	long smallest = list.at(0)
	for(std.vector<long double>it = list.begin(); it != list.end(); ++it)
	{	if (*it) < smallest:
		{	smallest = (*it)


	return smallest

