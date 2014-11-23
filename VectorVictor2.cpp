//#include "VectorVictor2.h"
#include <iostream>
#include <math.h>	// Roger, Roger.
//#include "Inertia_moment.h"
#include <sstream>
#include <iomanip>

std::string Convert_to_string(float num, int precision)
{	std::stringstream s;
	s << std::setprecision(precision) << num;
	return s.str();
}	
// this is a really handy function
// so much of like

// The VectorVictor namespace //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// VV2 Constructors ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

VectorVictor::Vector2::Vector2 ()
{	Set_values(0,0);	
}

VectorVictor::Vector2::Vector2(long double ix, long double iy)
{	Set_values(ix, iy);
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

long double VectorVictor::Vector2::Get_x()
{	return x;
}

long double VectorVictor::Vector2::Get_y()
{	return y;
}

std::string VectorVictor::Vector2::Get_vector(std::string unit)	// this would make a lot of sense as a template function for the cast type
{	std::string vector = "(";
	int exponent = 0;
	long double x_output = Get_x();
	if((x_output >= 1000.0000)||(x_output <= -1000.00000))
	{	while((x_output >= 1000.0000)||(x_output <= -1000.00000))
		{	x_output /= 1000.00000;
			exponent++;
		}
	}
	//vector.append(std::to_string((short)x_output));
	vector.append(Convert_to_string((float)x_output , 4));
	if(exponent != 0)
	{	switch(exponent)
		{	case 1:
			vector.append("k");		// kilo
			break;
			case 2:
			vector.append("M"); 	// Mega
			break;
			case 3:
			vector.append("G");	// Giga
			break;		
			case 4:
			vector.append("T");		// Tera	// (actual usefulness pretty much ends here)
			break;
			case 5:
			vector.append("P");	// Peta
			break;								
			case 6:
			vector.append("E");	// Exa
			break;
			case 7:
			vector.append("Z");		// Zetta
			break;						
		}
		if(exponent > 7)
		{	while(exponent > 7)
			{	x_output *= 1000.00;
			}
		}
	}
	vector.append(unit);
	vector.append(" , ");
	exponent = 0;
	long double y_output = Get_y();	
	if((y_output >= 1000.0000)||(y_output <= -1000.00000))
	{	while((y_output >= 1000.0000)||(y_output <= -1000.00000))
		{	y_output /= 1000.00000;
			exponent++;
		}
	}
	//vector.append(std::to_string((short)y_output));
	vector.append(Convert_to_string((float)y_output , 4));	
	if(exponent != 0)
	{	switch(exponent)
		{	case 1:
			vector.append("k");		// kilo
			break;
			case 2:
			vector.append("M"); 	// Mega
			break;
			case 3:
			vector.append("G");	// Giga
			break;		
			case 4:
			vector.append("T");		// Tera
			break;
			case 5:
			vector.append("P");	// Peta
			break;								
			case 6:
			vector.append("E");	// Exa		// (actual usefulness pretty much ends here)
			break;
			case 7:
			vector.append("Z");		// Zetta
			break;						
		}
		if(exponent > 7)
		{	while(exponent > 7)
			{	y_output *= 1000.00;
			}
		}
	}
	vector.append(unit);	
	vector.append(")");
	return vector;
}	// well that escalated quickly

// Standard Class Operators ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

VectorVictor::Vector2  VectorVictor::Vector2::operator+ (VectorVictor::Vector2 pplus)
{	VectorVictor::Vector2 tplus(this->Get_x(), this->Get_y());
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
{	VectorVictor::Vector2 tminus;
	tminus.x -= pminus.x;
	tminus.y -= pminus.y;
	return (tminus);		
}

VectorVictor::Vector2  VectorVictor::Vector2::operator-= (VectorVictor::Vector2 pminus)
{	x -= pminus.x;
	y -= pminus.y;
	return (*this);		
}

VectorVictor::Vector2 VectorVictor::Vector2::operator= (const VectorVictor::Vector2 v)
{	x = v.x;
	y = v.y;
	return (*this);		
}

VectorVictor::Vector2 VectorVictor::Vector2::operator *= (const double c)
{	x *= c;
	y *= c;
	return (*this);		
}

bool VectorVictor::Vector2::operator != (const VectorVictor::Vector2 v)
{	if(this->x != v.x)
	{	if(this->y != v.y)
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

bool VectorVictor::Vector2::operator == (const VectorVictor::Vector2 v)
{	if(this->x == v.x)
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

// Extended Vector Operators ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void VectorVictor::Vector2::Normalize()
{	long double vector_magnitude = this->Get_vector_magnitude();
	if(vector_magnitude > 0)
	{	Set_x(x/(vector_magnitude));
		Set_y(y/(vector_magnitude));
	}
}

VectorVictor::Vector2 VectorVictor::Vector2::Vector_normal()
{	VectorVictor::Vector2 Output;
	long double vector_magnitude = this->Get_vector_magnitude();	std::cout << vector_magnitude << std::endl;
	Output.Set_x(x/(vector_magnitude));	std::cout << this->Get_x() << std::endl;
	Output.Set_y(y/(vector_magnitude));	std::cout << this->Get_y() << std::endl;
	return Output;										
}

void VectorVictor::Vector2::Rotate_vector(long double rotation)
{	rotation = ((rotation/360)*2*Pi);				
	x = ( (x*(cos(rotation)))+(y*(sin(rotation))) );	
	y = ( (x*((-1)*sin(rotation)))+(y*(cos(rotation))) );															 	
}	// Rotation is counter-clockwise, around the origin.
	// After my conversation with Prof Mann on 11/07/13, I later realized that I wasnt multiplying the vectors properly.
	
VectorVictor::Vector2 VectorVictor::Vector2::Get_rotated_vector(long double rotation)
{	VectorVictor::Vector2 V_rotated(0, 0);
	rotation = ((rotation/360)*2*Pi);				
	V_rotated.x = ( (x*(cos(rotation)))+(y*(sin(rotation))) );	
	V_rotated.y = ( (x*((-1)*sin(rotation)))+(y*(cos(rotation))) );
	return V_rotated;															 	
}	// Rotation is counter-clockwise, around the origin.
	// After my conversation with Prof Mann on 11/07/13, I later realized that I wasnt multiplying the vectors properly.

long double VectorVictor::Vector2::Get_vector_magnitude()
{	long double output, vx, vy;
	vx = pow(x, 2);
	vy = pow(y, 2);	vy += vx;
	output = sqrt(vy);
	return output;							
}

long double VectorVictor::Vector2::Get_vector_magnitude_squared()
{	long double output, vx, vy;
	vx = pow(x, 2);
	vy = pow(y, 2);
	output = (vx+vy);
	return output;				
}

// Destructor //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

VectorVictor::Vector2::~Vector2(void)
{								
}


// Extended non-member Vector Operators ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

long double VectorVictor::Get_dot_product(long double x1, long double y1, long double x2, long double y2)
{	long double output = ((x1*x2)+(y1*y2));
	return output;
}

long double VectorVictor::Get_dot_product(VectorVictor::Vector2 First_vector, VectorVictor::Vector2 Second_vector)
{	long double x1 = First_vector.Get_x();
	long double y1 = First_vector.Get_y();
	long double x2 = Second_vector.Get_x();
	long double y2 = Second_vector.Get_y();
	long double output = ((x1*x2)+(y1*y2));
	return output;
}

long double VectorVictor::Get_vector_angle(VectorVictor::Vector2 First_vector, VectorVictor::Vector2 Second_vector)
{	long double theta, cos_theta;
	long double dot_product, Magnitude1, Magnitude2;
	dot_product = VectorVictor::Get_dot_product(First_vector, Second_vector);
	Magnitude1 = First_vector.Get_vector_magnitude();
	Magnitude2 = Second_vector.Get_vector_magnitude();
	cos_theta = ((dot_product)/(Magnitude1*Magnitude2));
	theta = acos(cos_theta);
	return theta;
}	// cos(theta) = (a dot b)/(Maga*Magb);

long double VectorVictor::Get_cross_product(Vector2 First_vector, Vector2 Second_vector)
{	long double x1, y1, x2, y2;
	x1 = First_vector.Get_x();
	y1 = First_vector.Get_y();
	x2 = Second_vector.Get_x();
	y2 = Second_vector.Get_y();
	long double Output = ((x1*y2)-(x2*y1));
	return Output;
}	// Sorry for the sketchiness, assumption goes that the vectors are really (x,y,0), this returns the z value of their cross products

long double VectorVictor::Get_vector_separation(Vector2 First_vector, Vector2 Second_vector)
{	long double dx = (First_vector.x - Second_vector.x);
	long double dy = (First_vector.y - Second_vector.y);
	
	long double rad = sqrt((dx*dx)+(dy*dy));
	return rad;
}
