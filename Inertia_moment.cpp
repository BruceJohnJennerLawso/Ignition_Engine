#include "Inertia_moment.h"

// Inertia moment parent class /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_moment::Inertia_moment()
{
}

double Inertia_moment::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	return 0;
}

double Inertia_moment::Get_moment_about_pivot(double inside_mass, double outside_mass)
{	return 0;
}

Inertia_moment::~Inertia_moment()
{
}

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
	if(pivot_point != Position)
	{	pivot_point = Position - pivot_point;			// possible reference entanglement here if the point is referenced otherwise... not sure what that actually meant...
		double r = pivot_point.Get_vector_magnitude();	// get the offset vector from the pivot point, and find its magnitude for PAT
		moment += (inside_mass*(r*r));							// I(r) = Icg + (m(r^2))	
	}	return moment;
}

double Inertia_complex::Get_moment_about_pivot(double inside_mass, double outside_mass)
{	double moment = (inside_mass*k);
	double r = Position.Get_vector_magnitude();
	if(r != 0)
	{	moment += (inside_mass*(r*r));		// Apply the parallel axis theorem here
		return moment;
	}
	else
	{	return moment;
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
	Position = PositionVector;
	interior_moment = NULL;
}

Inertia_cylinder::Inertia_cylinder(double inner_radius, double outer_radius, double cylinder_height, VectorVictor::Vector2 PositionVector)
{	Outer_radius = outer_radius;
	Inner_radius = inner_radius;
	height = cylinder_height;
	hollow = true;
	k = ((3*((Outer_radius*Outer_radius)+(Inner_radius*Inner_radius)))+(height*height));
	Position = PositionVector;
	interior_moment = new Inertia_cylinder(inner_radius, cylinder_height, PositionVector);
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
