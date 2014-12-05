// Inertia_moment.h ////////////////////////////////////////////////////////////
// A standard template header file for my projects /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Source.cpp"
#include "SFML_Tools.h"

enum moment_type{hollow_cylinder, solid_cylinder, hollow_sphere, solid_sphere, hollow_box, solid_box, complex_shape};

// Inertia moment parent class /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_moment
{	public:
	Inertia_moment();
	virtual double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	virtual double Get_moment_about_pivot(double inside_mass, double outside_mass);
	VectorVictor::Vector2 Position;
	//private:
	Inertia_moment * interior_moment;
	public:
	~Inertia_moment();
};	// Why is this class here? Damn this is not good...
// Oh of course, its the polymorphic parent for the inertia contributors to each 
// vessel from each part (box, sphere, etc...)


// Complex Inertias ////////////////////////////////////////////////////////////
// for when we just want a # ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_complex: public Inertia_moment
{	// public inheritance avoids the interior moment pointer from inheriting.
	// It could be used in this case, but probably wouldnt be very useful
	public:																
	Inertia_complex(double kfactor, VectorVictor::Vector2 PositionVector);
	double k;
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	~Inertia_complex();
};	// basically inertia complex is for when we have a complex shape, and we
// dont give a damn about internal density changes or whatnot. Primarily used
// for vessel hulls like on the delta glider, which dont have a convenient shape
// for finding their inertia moment

// Cylinders, hollow and solid /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_cylinder: public Inertia_moment
{	public:
	Inertia_cylinder(double radius, double height, VectorVictor::Vector2 PositionVector);		// Solid cylinder
	Inertia_cylinder(double inner_radius, double outer_radius, double cylinder_height, VectorVictor::Vector2 PositionVector);	// Open-ended cylinder, like a toilet paper roll
	protected:
	bool hollow;
	double Inner_radius, Outer_radius, height;
	double k;	// Given that dimensions should remain constant for all Inertia objects, it makes sense to do that horrific part of the computation once at construction
	public:		// or maybe on demand too
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	~Inertia_cylinder();
};	// cylinders that lie flat in the plane like a box, but actually have a
	// different inertia

// Spheres, hollow and solid ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_sphere: public Inertia_moment
{	public:
	Inertia_sphere(double radius, VectorVictor::Vector2 PositionVector);								// Solid balls of inertia
	Inertia_sphere(double inner_radius, double outer_radius, VectorVictor::Vector2 PositionVector);	// Sphere with an internal cavity, of radius inner_radius
	protected:
	bool hollow;
	double Inner_radius, Outer_radius;
	double k;	// Given that dimensions should remain constant for all Inertia objects, it makes sense to do that horrific part of the computation once at construction
	public:
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	~Inertia_sphere();
};

// Boxes, hollow and solid /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_box: public Inertia_moment
{	public:
	Inertia_box(double length, double width, double height, VectorVictor::Vector2 PositionVector);														// Solid box
	Inertia_box(double outer_length, double outer_width, double inner_length, double inner_width, double outer_height, double inner_height, VectorVictor::Vector2 PositionVector);	// Box with box-shaped cavity centered inside 
	protected:
	bool hollow;
	double Inner_length, Inner_width, Inner_height;
	double Outer_length, Outer_width, Outer_height;
	double k;	
	// Given that dimensions should remain constant for all Inertia objects, 
	// it makes sense to do that horrific part of the computation once at 
	// construction. We can always redo it later if needed
	public:
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	~Inertia_box();
};

