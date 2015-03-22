#include "VectorVictor2.hpp"
#include <iostream>
#include <math.h>	
// Roger, Roger.

//#include "Inertia_moment.h"


std::string Orientation_as_text(vector_orientation input)
{	std::string output = "";
	switch(input)
	{	case inwards:
		output = "inwards";
		break;
		
		case outwards:
		output = "outwards";
		break;
		
		case parallel:
		output = "parallel";
		break;
		
		case non_parallel:
		output = "non parallel";
		break;
	}
	return output;
}

// The VectorVictor namespace //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// VV2 Constructors ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

VectorVictor::Vector2::Vector2 ()
{	// if nothing is passed, we assume the user wanted (0,0)
	Set_values(0,0);	
}	

VectorVictor::Vector2::Vector2(long double ix, long double iy)
{	// otherwise we set the values of the vector as requested
	Set_values(ix, iy);
	#ifdef VECTORVICTOR_DEBUG
	Nan_state = Is_nan();	
	#endif
}

// Set/Get Operators ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void VectorVictor::Vector2::Set_x(long double ix)
{	x = ix;					
}

void VectorVictor::Vector2::Set_y(long double iy)
{	y = iy;					
}

void VectorVictor::Vector2::Set_values(long double ix, long double iy)
{	x = ix;
	y = iy;
}

// direct setters, nothing much to explain here

long double VectorVictor::Vector2::Get_x()
{	return x;
}

long double VectorVictor::Vector2::Get_y()
{	return y;
}

// and equally boring getters that just return the value itself

std::string VectorVictor::Vector2::Get_vector(std::string unit)	// this would make a lot of sense as a template function for the cast type
{	// the idea behind this is to provide a nice interface for getting strings
	// of a given vector formatted nicely with brackets, unit abbreviations
	// (meters, kilograms, gees, etc.) and SI prefixes (kilo, mega, giga...)
	std::string vector = "(";
	// we create the start bracket
	int exponent = 0;
	// and initialize the exponent of the value used later
	
	long double x_output = Get_x();
	// since operations on the content of the Vector2 will need to change its
	// values, we need to copy to a temporary variable for the duration 
	if((x_output >= 1000.0000)||(x_output <= -1000.00000))
	{	// if the magnitude of the x value isnt within +/- 10^3,
		// we are going to scale it back until it is within those bounds
		
		// this is so that output displays printed to the game window can stay
		// a nice short, easy to read length instead of a long bloated mess that
		// jumps back & forth every time the order of magnitude changes
		while((x_output >= 1000.0000)||(x_output <= -1000.00000))
		{	// while we are outside of our target bounds, divide our magnitude 
			// by 10^3, and increase the exponent scale by 1 each time
			x_output /= 1000.00000;
			exponent++;
		}
	}
	
	if(exponent > 7)
	{	// as we will see in a minute, if we go past 10^7, I dont actually have
		// a SI prefix for that, although Im sure it exists. If that is the case
		// we scale the value up by 10^3 in magnitude again so that the display
		// remains accurate, but no longer is concise
		
		// this problem should be pretty rare though, as 10^7 is freaking Zetta,
		// ^ Famous last words lol
		while(exponent > 7)
		{	x_output *= 1000.00;
			exponent--;
			// odd that last call to -- was missing. What the hell was it doing
			// before?
			
			// mildly concerning to say the least 
		}
	}

	vector.append(Convert_to_string((float)x_output , 4));
	if(exponent != 0)
	{	// if the exponent wasnt zero, we need to add a SI prefix to the vector
		// so we sort through the possible options at every 3 orders of
		// magnitude, and apply the one that fits to the string
		switch(exponent)
		{	case 1:
			vector.append("k");	
			// kilo
			break;
			case 2:
			vector.append("M"); 	
			// Mega
			break;
			case 3:
			vector.append("G");	
			// Giga
			break;		
			case 4:
			vector.append("T");		
			// Tera	// (actual usefulness pretty much ends here)
			break;
			case 5:
			vector.append("P");	
			// Peta
			break;								
			case 6:
			vector.append("E");	
			// Exa
			break;
			case 7:
			vector.append("Z");		
			// Zetta
			break;						
		}
	}
	vector.append(unit);
	// we append the unit of the vector after the si prefix, ie 'm'
	vector.append(" , ");
	// and  we tack on the comma, since we have the y value to add as well
	exponent = 0;
	long double y_output = Get_y();	
	// same as with the x value, cant mess with our internal x & y values, so
	// we make a copy to work with
	if((y_output >= 1000.0000)||(y_output <= -1000.00000))
	{	// if the magnitude of y isnt within the acceptable range, we scale it
		// back until it is
		while((y_output >= 1000.0000)||(y_output <= -1000.00000))
		{	y_output /= 1000.00000;
			exponent++;
		}
	}

	if(exponent > 7)
	{	// as we will see in a minute, if we go past 10^7, I dont actually have
		// a SI prefix for that, although Im sure it exists. If that is the case
		// we scale the value up by 10^3 in magnitude again so that the display
		// remains accurate, but no longer is concise
		
		// this problem should be pretty rare though, as 10^7 is freaking Zetta,
		// ^ Famous last words lol
		while(exponent > 7)
		{	y_output *= 1000.00;
			exponent--;
			// odd that last call to -- was missing. What the hell was it doing
			// before? That should have been an infinite loop!
			
			// mildly concerning to say the least 
		}
	}
	
	vector.append(Convert_to_string((float)y_output , 4));	
	if(exponent != 0)
	{	// if the exponent wasnt zero, we need to add a SI prefix before the
		// unit, so we sort through and tack an appropriate one on
		switch(exponent)
		{	case 1:
			vector.append("k");		
			// kilo
			break;
			case 2:
			vector.append("M"); 	
			// Mega
			break;
			case 3:
			vector.append("G");	
			// Giga
			break;		
			case 4:
			vector.append("T");		
			// Tera
			break;
			case 5:
			vector.append("P");	
			// Peta
			break;								
			case 6:
			vector.append("E");	
			// Exa		// (actual usefulness pretty much ends here)
			break;
			case 7:
			vector.append("Z");		
			// Zetta
			break;						
		}
	}
	vector.append(unit);	
	// we append the unit in question after the SI prefix
	vector.append(")");
	// add the close bracket in as well
	return vector;
	// and finally, we return the string that this function created
}	

// Standard Class Operators ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// important tools for a vector class, since they make objects of the type much
// more intuitive to work with

VectorVictor::Vector2  VectorVictor::Vector2::operator+ (VectorVictor::Vector2 pplus)
{	// still not particularly comfortable with standard operators just yet,
	// what it does is obvious, but I really should learn this inside and out
	VectorVictor::Vector2 tplus(this->Get_x(), this->Get_y());
	tplus.x += pplus.x;
	tplus.y += pplus.y;
	return (tplus);		
}

VectorVictor::Vector2  VectorVictor::Vector2::operator+= (VectorVictor::Vector2 pplus)
{	x += pplus.x;
	y += pplus.y;
	return (*this);		
}

VectorVictor::Vector2  VectorVictor::Vector2::operator- (VectorVictor::Vector2 pminus)
{	VectorVictor::Vector2 tminus(this->Get_x(), this->Get_y());
	tminus.x -= pminus.x;
	tminus.y -= pminus.y;
	return (tminus);		
}

VectorVictor::Vector2  VectorVictor::Vector2::operator-= (VectorVictor::Vector2 pminus)
{	//bool nanState = Is_nan();
	x -= pminus.x;
	y -= pminus.y;
	//this->Flag_nan("Nan created at VectorVictor::Vector2::operator-=", nanState);
	return (*this);		
}

VectorVictor::Vector2 VectorVictor::Vector2::operator= (const VectorVictor::Vector2 v)
{	// copy operator, copies each element to the current Vector2
	#ifdef VECTORVICTOR_DEBUG
	bool nanState = Is_nan();
	#endif
	x = v.x;
	y = v.y;
	#ifdef VECTORVICTOR_DEBUG
	this->Flag_nan("Nan created at VectorVictor::Vector2::operator=", nanState);
	#endif
	return (*this);		
}

VectorVictor::Vector2 VectorVictor::Vector2::operator * (const double c)
{	
	#ifdef VECTORVICTOR_DEBUG
	bool nanState = Is_nan();
	#endif
	x *= c;
	y *= c;
	#ifdef VECTORVICTOR_DEBUG
	this->Flag_nan("Nan created at VectorVictor::Vector2::operator *", nanState);
	#endif
	return (*this);
}

VectorVictor::Vector2 VectorVictor::Vector2::operator *= (const double c)
{	// multiply every element in the vector by some value c
	x *= c;
	y *= c;
	return (*this);		
}

bool VectorVictor::Vector2::operator != (const VectorVictor::Vector2 v)
{	// to check if the vectors are not equivalent, we specifically look at
	// each element in each vector to see if any are different. If both of x 
	// and y are equivalent, we return false to indicate that it was == 
	
	// specifically this function evaluates if the given functions are the
	// *exact same*
		
	if(this->x != v.x)
	{	return true;
	}
	else
	{	if(this->y != v.y)
		{	return true;
		}
		else
		{	return false;
			// both elements were the same, just not your lucky day
		}
	}	
}

bool VectorVictor::Vector2::operator == (const VectorVictor::Vector2 v)
{	// same idea as !=, now we just to check to see if each element in each
	// vector is the same as the one in the other
	// iff x1==x2 & y1==y2, we return true, indicating that
	if(this->x == v.x)
	{	if(this->y == v.y)
		{	return true;
		}
		else
		{	return false;
		}
	}
	else
	{	return false;
	}	
}

bool VectorVictor::Vector2::Is_nan()
{	if(this->x != this->x)
	{	return true;
	}
	else
	{	if(this->y != this->y)
		{	return true;
		}
		else
		{	return false;
		}
	}
}

bool VectorVictor::Vector2::Flag_nan(std::string message, bool &initial_state)
{	if(initial_state == false)
	{	if(this->Is_nan())
		{	
			#ifdef VECTORVICTOR_DEBUG
			std::cout << message << std::endl;
			#endif
			return true;
		}
		else
		{	return false;
		}
	}
	else
	{	return false;
	}
}

// Extended Vector Operators ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void VectorVictor::Vector2::Normalize()
{	long double vector_magnitude = this->Get_vector_magnitude();
	// we find the current magnitude of the vector
	if(vector_magnitude > 0)
	{	// and divide by it to adjust the vector to its correct value
		Set_x(x/(vector_magnitude));
		Set_y(y/(vector_magnitude));
	}
	// otherwise do nothing, cause I dont have a clue what we do if it is the
	// zero vector
}

VectorVictor::Vector2 VectorVictor::Vector2::Vector_normal()
{	VectorVictor::Vector2 Output;
	long double vector_magnitude = this->Get_vector_magnitude();
	// we find our current vector magnitude	
	if(vector_magnitude > 0)
	{	
		Output.Set_x(x/(vector_magnitude));	
		Output.Set_y(y/(vector_magnitude));
	}
	else
	{	Output.Set_x(0);	
		Output.Set_y(0);
	}
	// set the output vector to the normalized value
	return Output;
	// and send the result on its merry way
	// important to note: does *not* affect the contents of the calling
	// Vector2 in any way										
}

void VectorVictor::Vector2::Rotate_vector(long double rotation)
{	rotation = ((rotation/360)*2*Pi);	
	// we convert our rotation from degrees to radians, since standard library
	// math calls want radians to do their calculations			
	long double new_x = ( (x*(cos(rotation)))+(y*(sin(rotation))) );	
	// AHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	
	// the variables are changing with respect to themselves!!!!
	// AM WINNER PROGRAMMER, SUCH OF FULL oF WIN
	y = ( (x*((-1)*sin(rotation)))+(y*(cos(rotation))) );
	// multiply the vector by the 2d rotational matrix, rotating clockwise
	// around the origin. 
	x = new_x;
	// [cos(theta)		sin(theta)]
	// [-sin(theta		cos(theta)]
	
	// important to note that this vector affects the contents of the vector
	// that calls it. If you want to get the rotated vector without affecting
	// its current orientation, you need to call the next method below  															 	
}	
	
VectorVictor::Vector2 VectorVictor::Vector2::Get_rotated_vector(long double rotation)
{	VectorVictor::Vector2 V_rotated(0, 0);
	// pointless to be explicit about it being 0,0
	// this would work better with setting its value as the parents
	// x and y here instead
	rotation = ((rotation/360)*2*Pi);
	// convert our rotation from degrees to radians				
	V_rotated.x = ( (x*(cos(rotation)))+(y*(sin(rotation))) );	
	V_rotated.y = ( (x*((-1)*sin(rotation)))+(y*(cos(rotation))) );
	// multiply the vector by the 2d rotational matrix, rotating clockwise
	// around the origin. 
	
	// [cos(theta)		sin(theta)]
	// [-sin(theta		cos(theta)]
	
	return V_rotated;															 	
	// and send the resulting VV2 vector on its way
}	

long double VectorVictor::Vector2::Get_vector_magnitude()
{	long double output, vx, vy;
	// define the variables we will need to do operations on
	vx = pow(x, 2);
	vy = pow(y, 2);	
	// square both the x and y elements
	// does this in an odd way, but the x and y of the object are not affected
	// just passed by value to pow()
	output = (vy + vx);
	// add together the two components
	output = sqrt(output);
	// and run an expensive call to sqrt() to finish applying pythagorean
	// theorem
	return output;							
}

long double VectorVictor::Vector2::Get_vector_magnitude_squared()
{	long double output, vx, vy;
	vx = pow(x, 2);
	vy = pow(y, 2);
	// define our variables just like before
	output = (vx+vy);
	// add them up, but simply return that result as is, since we dont need to
	// run that expensive call to sqrt()
	return output;				
}


long double VectorVictor::Vector2::Dot(VectorVictor::Vector2 &vector)
{	long double output = ((this->x*vector.x)+(this->y*vector.y));
	return output;
}

long double VectorVictor::Vector2::Cross(VectorVictor::Vector2 &vector)
{	long double output = ((this->x*vector.y)-(vector.x*this->y));
	return output;
}

VectorVictor::Vector2 VectorVictor::Vector2::Projection(VectorVictor::Vector2 &onto)
{	VectorVictor::Vector2 output(onto.x, onto.y);
	// create a copy of the vector we are projecting onto
	output *= ((this->Dot(onto))/(onto.Dot(onto)));
	// multiply it by (V1 dot V2)/(V2 dot V2)
	return output;
}

bool VectorVictor::Vector2::Parallel(VectorVictor::Vector2 &vector)
{	long double ratio = (this->x/vector.x);
	if(ratio == (this->y/vector.y))
	{	return true;
	}
	return false;
}

bool VectorVictor::Vector2::Antiparallel(VectorVictor::Vector2 &vector)
{	long double ratio = (this->x/vector.x);
	if(ratio == (this->y/vector.y))
	{	if(ratio < 0)
		{	return true;
		}
	}
	return false;	
}


// Destructor //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

VectorVictor::Vector2::~Vector2(void)
{	// no memory allocations, so nothing much to do here
}


// Extended non-member Vector Operators ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

long double VectorVictor::Get_dot_product(long double x1, long double y1, long double x2, long double y2)
{	long double output = ((x1*x2)+(y1*y2));
	// not much to say here, just perform the standard dot-product operation
	// & return the result. same thing below
	return output;
}



long double VectorVictor::Get_dot_product(VectorVictor::Vector2 First_vector, VectorVictor::Vector2 Second_vector)
{	long double x1 = First_vector.Get_x();
	long double y1 = First_vector.Get_y();
	long double x2 = Second_vector.Get_x();
	long double y2 = Second_vector.Get_y();
	long double output = ((x1*x2)+(y1*y2));
	// simple dot product, now done with vector objects passed
	// this doesnt need to be this wasteful, we could just use the pass by
	// value vv2s that were passed as parameters instead
	return output;
}

long double VectorVictor::Get_vector_angle(VectorVictor::Vector2 First_vector, VectorVictor::Vector2 Second_vector)
{	// given that a dot b = ||a|| ||b|| cos(theta)
	// we look to solve for the angle between the two vectors
	// again, not sure where this is actually used, but good to have
	long double theta, cos_theta;
	long double dot_product, Magnitude1, Magnitude2;
	dot_product = VectorVictor::Get_dot_product(First_vector, Second_vector);
	// we find the dot product of the two vectors
	Magnitude1 = First_vector.Get_vector_magnitude();
	Magnitude2 = Second_vector.Get_vector_magnitude();
	// and then we find the magnitudes of the vectors so we can get the
	// value of cos(theta)
	cos_theta = ((dot_product)/(Magnitude1*Magnitude2));
	theta = acos(cos_theta);
	// finally we run a call to arccos, perhaps a wee bit expensive, but dont
	// know any other ways of doing it, so hey
	return theta;
}

long double VectorVictor::Get_cross_product(Vector2 First_vector, Vector2 Second_vector)
{	long double x1, y1, x2, y2;
	x1 = First_vector.Get_x();
	y1 = First_vector.Get_y();
	x2 = Second_vector.Get_x();
	y2 = Second_vector.Get_y();
	// we set up the variables from each vector
	long double Output = ((x1*y2)-(x2*y1));
	// then calculate the cross product, specifically the component in the k hat
	// direction, since all vectors lie flat in the plane anyways	
	return Output;
}	

long double VectorVictor::Get_vector_separation(Vector2 First_vector, Vector2 Second_vector)
{	long double dx = (First_vector.x - Second_vector.x);
	long double dy = (First_vector.y - Second_vector.y);
	long double rad = sqrt((dx*dx)+(dy*dy));
	// simple enough, just applying pythagoras again
	return rad;
}

long double Minimum(long double value1, long double value2)
{	if(value1 < value2)
	{	return value1;
	}
	else
	{	if(value1 == value2)
		{	return value1;
			// it dont matter
		}
		else
		{	return value2;
		}
	}
}

long double Maximum(long double value1, long double value2)
{	if(value1 > value2)
	{	return value1;
	}
	else
	{	if(value1 == value2)
		{	return value1;
			// it dont matter
		}
		else
		{	return value2;
		}
	}
}

bool In_range(long double boundary1, long double boundary2, long double target_value)
{	if((target_value >= Minimum(boundary1, boundary2))&&(target_value <= Maximum(boundary1, boundary2)))
	{	// if the value falls in the range we're good to go
		return true;
	}
	return false;
}

long double Absolute_value(long double value)
{	if(value < 0)
	{	value = -value;
	}
	return value;
}

long double Smallest_value(std::vector<long double> &list)
{	long double smallest = list.at(0);
	for(std::vector<long double>::iterator it = list.begin(); it != list.end(); ++it)
	{	if((*it) < smallest)
		{	smallest = (*it);
		}
	}
	return smallest;
}

// Circle //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Circle::Circle()
{	Position.Set_values(0,0);
	Radius = -1;
	// flags it as not really constructed yet
}

Circle::Circle(VectorVictor::Vector2 position, long double radius)
{	this->Set_values(position, radius);
}

void Circle::Set_values(VectorVictor::Vector2 &position, long double radius)
{	Position = position;
	Radius = radius;
}

bool Circle::Intersection(Circle &target_circle, VectorVictor::Vector2 &offset)
{	offset = (target_circle.Position - this->Position);
	//get the relative offset from the *this Circle to the target circle
	long double separation = offset.Get_vector_magnitude_squared();
	if(separation < Squared(target_circle.Radius + this->Radius))
	{	// compared the relative squares, & found an intersection
		return true;
	}
	return false;
}

Circle::~Circle()
{
}

long double Squared(long double value)
{	value *= value;
	return value;
}



// Vector_pair experiment //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

VectorVictor::Vector_pair::Vector_pair(VectorVictor::Vector2 position, VectorVictor::Vector2 direction)
{	Position = position;
	Direction = direction;
}

vector_orientation VectorVictor::Vector_pair::Orientation(Vector_pair &vector)
{	if(!this->Direction.Parallel(vector.Direction))
	{	VectorVictor::Vector2 offset(0,0);
		offset = (this->Position - vector.Position);
		if((offset.Parallel(this->Direction))&&(offset.Parallel(vector.Direction)))
		{	// we checked and both direction vectors are at least pointing in
			// the line between their positions, otherwise non_parallel
			
			// and now we need to check the exact details of how they are
			// oriented in 2d space, relating to the exact value of that c value
			// where V1 = cV2
			
			// but we might be able to tiptoe around that using some logic
			if(this->Direction.Antiparallel(vector.Direction) == false)
			{	// if we aint antiparallel, we must be parallel ya know
				return parallel;
			}
			else if(vector.Direction.Antiparallel(offset) == true)
			{	// see the diagram for the explain on this one
				
				// its based on the exact direction of offset starting at the
				// tip of vector.position and pointing towards the tip of
				// this->position
				return outwards;
			}
			else
			{	// having covered every other case, we are good to go
				return inwards;
			}
		}
		
	}
	return non_parallel;
		
}

VectorVictor::Vector_pair::~Vector_pair()
{
}







