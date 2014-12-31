#include "Game_physics.hpp"

// Inertia moment parent class /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_moment::Inertia_moment()
{
}	// this should go

double Inertia_moment::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	return 0;
}	// needs an error talkback message

double Inertia_moment::Get_moment_about_pivot(double inside_mass, double outside_mass)
{	return 0;
}	// needs an error talkback message

Inertia_moment::~Inertia_moment()
{
}	// also bad maybe

// Complex Inertias ////////////////////////////////////////////////////////////
// for when we just want a # ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_complex::Inertia_complex(double  kfactor, VectorVictor::Vector2 PositionVector)
{	k = kfactor;
	Position = PositionVector;
	interior_moment = NULL;
}

double Inertia_complex::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = (inside_mass*k);
	// this should be changed, given that a moment is actually something
	// different ;)
	
	// if the pivot point was the origin of the part, we're okay
	if(pivot_point != Position)
	{	// but if not we need to apply the parallel axis theorem to find the new
		// moment of inertia around the new axis
		pivot_point = Position - pivot_point;
		// so we find the relative offset vector from the parts center where
		// our original moment of inertia was calculated, to the new axis that
		// we are calculating the moment of inertia around
		double r = pivot_point.Get_vector_magnitude();	
		// we use that info to find the relative offsets magnitude
		moment += (inside_mass*(r*r));		
		//and finally use the PAT as stated above & below
		// I(r) = Icg + (m(r^2))	
	}	return moment;
}

double Inertia_complex::Get_moment_about_pivot(double inside_mass, double outside_mass)
{	double moment = (inside_mass*k);
	// we start as usual
	double r = Position.Get_vector_magnitude();
	// and we check to see if the Position of the element is offset from the
	// center of the parent vessel
	
	// gotta remember, position is in the parent vessels frame of reference
	if(r != 0)
	{	moment += (inside_mass*(r*r));
		return moment;
		// Apply the parallel axis theorem here
		// to shift it relative to our new axis
	}
	else
	{	return moment;
		// other wise just send it as is
	}
}

Inertia_complex::~Inertia_complex()
{
}


// Cylinders, hollow and solid /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_cylinder::Inertia_cylinder(double radius, double cylinder_height, VectorVictor::Vector2 PositionVector)
{	Outer_radius = radius;
	Inner_radius = 0;
	height = cylinder_height;
	hollow = false;
	k = (height*height);
	// weird that the 1/12 isnt applied here, but rather in the function call'
	// itself. Oh well...
	Position = PositionVector;
	interior_moment = NULL;
	// simple set calls, except for this last one, not sure why it absolutely
	// has to be NULL, but no point in fixing an aint broke
	
	// I dont like this setup, this should really be done with separate objects
}

Inertia_cylinder::Inertia_cylinder(double inner_radius, double outer_radius, double cylinder_height, VectorVictor::Vector2 PositionVector)
{	Outer_radius = outer_radius;
	Inner_radius = inner_radius;
	height = cylinder_height;
	hollow = true;
	k = ((3*((Outer_radius*Outer_radius)+(Inner_radius*Inner_radius)))+(height*height));
	Position = PositionVector;
	interior_moment = new Inertia_cylinder(inner_radius, cylinder_height, PositionVector);
	// similar to above, just that we calculate k differently, and the inside
	// moment is initialized for obvious reasons
}

double Inertia_cylinder::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = 0;
	moment += ((outside_mass*k)/12);
	if(pivot_point != Position)		// Parallel axis theorem!!!
	{	pivot_point = Position - pivot_point;			// possible reference entanglement here if the point is referenced otherwise
		double r = pivot_point.Get_vector_magnitude();
		moment += (outside_mass*(r*r));							// I(r) = Icg + (m(r^2))
		if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(pivot_point, 0, inside_mass);
		}	return moment;	
	}
	else
	{	if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(pivot_point, 0, inside_mass);
		}
		return moment;
	}
}	// Used if the pivot point is not 0 for some reason. Most likely not to be used any time soon, as it usually should be (0,0) // Actually, quite helpful, since it should allow for COG shift

double Inertia_cylinder::Get_moment_about_pivot(double inside_mass, double outside_mass)
{	double moment = 0;
	moment += ((outside_mass*k)/12);
	double r = Position.Get_vector_magnitude();
	if(r != 0)
	{	moment += (outside_mass*(r*r));
		if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(0, inside_mass);
		}	return moment;
	}
	else
	{	if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(0, inside_mass);
		}	return moment;
	}
}	

Inertia_cylinder::~Inertia_cylinder()
{	delete interior_moment;
}	// Okay, inertia cylinder is finished now

// Spheres, hollow and solid ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_sphere::Inertia_sphere(double radius, VectorVictor::Vector2 PositionVector)
{	Outer_radius = radius;
	Inner_radius = 0;
	hollow = false;
	k = (Outer_radius*Outer_radius);
	Position = PositionVector;
	interior_moment = NULL;
}

Inertia_sphere::Inertia_sphere(double inner_radius, double outer_radius, VectorVictor::Vector2 PositionVector)
{	Outer_radius = outer_radius;
	Inner_radius = inner_radius;
	hollow = true;
	k = (((pow(Outer_radius, 5))-(pow(Inner_radius, 5)))/((pow(Outer_radius, 3))-(pow(Inner_radius, 3))));
	Position = PositionVector;
	interior_moment = new Inertia_sphere(inner_radius, PositionVector);
}

		// Need to fix inner/outer mass stuff
double Inertia_sphere::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = 0;
	moment += ((2*(outside_mass*k))/5);
	if(pivot_point != Position)		// Parallel axis theorem!!!
	{	pivot_point = Position - pivot_point;			// possible reference entanglement here if the point is referenced otherwise
		double r = pivot_point.Get_vector_magnitude();
		moment += (outside_mass*(r*r));							// I(r) = Icg + (m(r^2))
		if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(pivot_point, 0, inside_mass);
		}	return moment;
	}
	else
	{	if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(pivot_point, 0, inside_mass);
		}	return moment;
	}
}	// Used if the pivot point is not 0 for some reason. Most likely not to be used any time soon, as it usually should be (0,0)

double Inertia_sphere::Get_moment_about_pivot(double inside_mass, double outside_mass)
{	double moment = 0;
	moment += ((2*(outside_mass*k))/5);
	double r = Position.Get_vector_magnitude();
	if(r != 0)
	{	moment += (outside_mass*(r*r));
		if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(0, inside_mass);
		}	return moment;
	}
	else
	{	if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(0, inside_mass);
		}	return moment;
	}
}

Inertia_sphere::~Inertia_sphere()
{	delete interior_moment;
}


// Boxes, hollow and solid /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_box::Inertia_box(double length, double width, double height, VectorVictor::Vector2 PositionVector)
{	Outer_length = length;
	Outer_width = width;
	Outer_height = height;
	Inner_length = 0;
	Inner_width = 0;
	Inner_height = 0;
	hollow = false;
	k = ((Outer_length*Outer_length)+(Outer_width*Outer_width));
	Position = PositionVector;
	interior_moment = NULL;
}

Inertia_box::Inertia_box(double outer_length, double outer_width, double inner_length, double inner_width, double outer_height, double inner_height, VectorVictor::Vector2 PositionVector)
{	Outer_height = outer_height;
	Outer_length = outer_length;
	Outer_width = outer_width;
	Inner_height = inner_height;
	Inner_length = inner_length;		// It would probably make sense to scrap these since their values will be held by the interior_moment object instead.
	Inner_width = inner_width;			//  not going to do that now though. Could be a bit of trouble if dimensions are manipulated at runtime, but not to worry now.
	hollow = true;
	k = (((Outer_length*Outer_length)+(Outer_width*Outer_width))-((((pow(Inner_length, 4))*Inner_width)/((Outer_length*Outer_length)*Outer_width))+(((Inner_length*Inner_length)*(Inner_width*Inner_width*Inner_width))/(Outer_width*(Outer_length*Outer_length)))));
	Position = PositionVector;
	interior_moment = new Inertia_box(inner_length, inner_width, inner_height, PositionVector);
}

double Inertia_box::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = 0;
	moment += ((outside_mass*k)/12);
	if(pivot_point != Position)		// Parallel axis theorem!!!
	{	pivot_point = Position - pivot_point;			// possible reference entanglement here if the point is referenced otherwise
		double r = pivot_point.Get_vector_magnitude();
		moment += (outside_mass*(r*r));							// I(r) = Icg + (m(r^2))
		if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(pivot_point, 0, inside_mass);
		}	return moment;
	}
	else
	{	if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(pivot_point, 0, inside_mass);
		}	return moment;
	}
}	// Used if the pivot point is not 0 for some reason. Most likely not to be used any time soon, as it usually should be (0,0)

double Inertia_box::Get_moment_about_pivot(double inside_mass, double outside_mass)
{	double moment = 0;
	moment += ((outside_mass*k)/12);
	double r = Position.Get_vector_magnitude();
	if(r != 0)
	{	moment += (outside_mass*(r*r));	// parallel axis theorem
		if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(0, inside_mass);
		}	return moment;
	}
	else
	{	if(hollow == true)
		{	moment += interior_moment->Get_moment_about_pivot(0, inside_mass);
		}	return moment;
	}
}

Inertia_box::~Inertia_box()
{	delete interior_moment;
}
