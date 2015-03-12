#include <math.h>
#include "Game_physics.hpp"

// Inertia moment parent class /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_moment::Inertia_moment()
{
}	// this should go

double Inertia_moment::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	return 0;
}	// needs an error talkback message


bool Inertia_moment::Is_hollow()
{	Talkback("Bad call to Inertia_moment::Is_hollow()");
	return false;
}

Inertia_moment::~Inertia_moment()
{
}	// also bad maybe

// Complex Inertias ////////////////////////////////////////////////////////////
// for when we just want a # ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Inertia_complex::Inertia_complex(double kfactor)
{	K = kfactor;
}

double Inertia_complex::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = (inside_mass*K);
	// this should be changed, given that a moment is actually something
	// different ;)

	if(pivot_point.Get_vector_magnitude_squared() != 0)
	{	
		double r = pivot_point.Get_vector_magnitude();	
		// we use that info to find the relative offsets magnitude
		moment += (inside_mass*(r*r));		
		//and finally use the PAT as stated above & below
		// I(r) = Icg + (m(r^2))	
	}	
	return moment;
}

bool Inertia_complex::Is_hollow()
{	return false;
}


// old function, ignore it for now

Inertia_complex::~Inertia_complex()
{
}


// Solid Cylinder //////////////////////////////////////////////////////////////
// Rho constant the whole way through the cylinder /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Solid_cylinder::Solid_cylinder()
{	this->Set_values(1, 1);
}

void Solid_cylinder::Set_values(double radius, double height)
{	Radius = radius;
	Height = height;
	K = ((Height*Height)/12);
}

Solid_cylinder::Solid_cylinder(double radius, double cylinder_height)
{	this->Set_values(radius, cylinder_height);
}

double Solid_cylinder::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = 0;
	moment += (inside_mass*K);
	if(pivot_point.Get_vector_magnitude_squared() != 0)
	{	double r = pivot_point.Get_vector_magnitude();
		moment += (inside_mass*(r*r));							
		// I(r) = Icg + (m(r^2))	
	}
	return moment;
}

bool Solid_cylinder::Is_hollow()
{	return false;
}

Solid_cylinder::~Solid_cylinder()
{
}

// Hollow Cylinder /////////////////////////////////////////////////////////////
// A hollow cylinder, open at both ends, like a paper towel roll ///////////////
////////////////////////////////////////////////////////////////////////////////


Hollow_cylinder::Hollow_cylinder(double inner_radius, double outer_radius, double cylinder_height)
{	Inner_radius = inner_radius;
	Outer_radius = outer_radius;
	Height = cylinder_height;
	K = ((3*((outer_radius*outer_radius)+(inner_radius*inner_radius))) + (Height*Height))/12;
	// whew, that should be good enough for now
	Interior.Set_values(inner_radius, cylinder_height);
}

double Hollow_cylinder::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = 0;
	moment += (outside_mass*K);
	// this is under some review right now, these definitions might not have
	// been accurate
	if(pivot_point.Get_vector_magnitude_squared() != 0)
	{	double r = pivot_point.Get_vector_magnitude();
		moment += (outside_mass*(r*r));							
		// I(r) = Icg + (m(r^2))	
	}
	moment += Interior.Get_moment_about_pivot(pivot_point, inside_mass, 0);	
	// get the moment of inertia of the inside part
	return moment;
}

bool Hollow_cylinder::Is_hollow()
{	return true;
}

Hollow_cylinder::~Hollow_cylinder()
{
}


// Solid sphere, constant Rho throughout ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Solid_sphere::Solid_sphere()
{
}

Solid_sphere::Solid_sphere(double radius)
{	this->Set_values(radius);
}

void Solid_sphere::Set_values(double radius)
{	Radius = radius;
	K = (2/5)*(radius*radius);
}	

double Solid_sphere::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = 0;
	moment += (inside_mass*K);
	// this is under some review right now, these definitions might not have
	// been accurate
	if(pivot_point.Get_vector_magnitude_squared() != 0)
	{	double r = pivot_point.Get_vector_magnitude();
		moment += (inside_mass*(r*r));							
		// I(r) = Icg + (m(r^2))	
	}
	return moment;
}

bool Solid_sphere::Is_hollow()
{	return false;
}

Solid_sphere::~Solid_sphere()
{
}

// Hollow sphere, with a different density inside //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Hollow_sphere::Hollow_sphere(double inner_radius, double outer_radius)
{	Inner_radius = inner_radius;
	Outer_radius = outer_radius;
	K = (2/5)*(((pow(outer_radius, 5))-(pow(inner_radius, 5)))/((pow(outer_radius, 3))-(pow(inner_radius, 3))));
	// ugly aint it
}


double Hollow_sphere::Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass)
{	double moment = 0;
	moment += (outside_mass*K);
	// this is under some review right now, these definitions might not have
	// been accurate
	if(pivot_point.Get_vector_magnitude_squared() != 0)
	{	double r = pivot_point.Get_vector_magnitude();
		moment += (outside_mass*(r*r));							
		// I(r) = Icg + (m(r^2))	
	}
	moment += Interior.Get_moment_about_pivot(pivot_point, inside_mass, 0);	
	// get the moment of inertia of the inside part
	return moment;
}

bool Hollow_sphere::Is_hollow()
{	return true;
}

Hollow_sphere::~Hollow_sphere()
{
}
