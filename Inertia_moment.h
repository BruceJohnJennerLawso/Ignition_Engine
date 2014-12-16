// Inertia_moment.h ////////////////////////////////////////////////////////////
// Generalized moments of inertia of simple objects ////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Source.cpp"
#include "SFML_Tools.h"
// need this because it includes the VV2

#ifndef Inertia
#define Inertia

enum moment_type{hollow_cylinder, solid_cylinder, hollow_sphere, solid_sphere, hollow_box, solid_box, complex_shape};
// this is used somewhere, which I do not recall just yet

// Inertia moment parent class /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_moment
{	// the virtual parent class for specific moments of inertia shapes
	// (the wording here is weird unfortunately)
	// common aspects of the shapes are that they have a given
	// moment of inertia, when given a specific mass
	// and also, they have a specific position in the vessels coordinate frame
	// which is used to apply the parallel axis theorem to transfer the inertia
	// moment from the parts reference frame to that of the vessel
	public:
	Inertia_moment();
	// this is bad I think. Should test and remove
	virtual double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	// our first case where the pivot point is not at the origin. Will be used
	// once dynamic CG is in effect
	virtual double Get_moment_about_pivot(double inside_mass, double outside_mass);
	// The case where we just assume the pivot point is the origin (0,0)
	VectorVictor::Vector2 Position;
	// equivalent to the position of the part in the vessels reference frame
	Inertia_moment * interior_moment;
	// the reference to the inside moment IF IT HAS ONE
	// this could be especially bad if dereferenced by a solid part...
	// needs to be changed, although it appears something did not work
	// correctly with having it inherit based on child access
	
	// I think the compiler made a hairball when I tried to do it that way
	public:
	~Inertia_moment();
};	


// Complex Inertias ////////////////////////////////////////////////////////////
// for when we just want a # ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_complex: public Inertia_moment
{	// basically inertia complex is for when we have a complex shape, and we
	// dont give a damn about internal density changes or whatnot. Primarily used
	// for vessel hulls like on the delta glider, which dont have a convenient shape
	// for finding their inertia moment, but are usually static with their mass
	// over time as well as how the mass is distributed
	public:																
	Inertia_complex(double kfactor, VectorVictor::Vector2 PositionVector);
	double k;
	// literally the inertia about the complex shapes own axis is just k*m
	// which I think is workable, so long as the rho(r) distribution doesnt
	// change with time
	
	// I = integral over the volume of rho(r) r^2 dV
	// which should just relate back to some constant retrieved due to
	// the rho part, and 1/3 r^3, which is constant anyways 
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	// the mobile origin case
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	// and assuming the origin is (0,0)
	~Inertia_complex();
};	

// Cylinders, hollow and solid /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_cylinder: public Inertia_moment
{	// the specific shape of a cylinder, but with its z axis (the one through
	// the circular faces) lying in the games xy plane. Useful for fuel tanks
	// especially
	public:
	Inertia_cylinder(double radius, double height, VectorVictor::Vector2 PositionVector);		
	// Solid cylinder, density constant over the whole thing
	Inertia_cylinder(double inner_radius, double outer_radius, double cylinder_height, VectorVictor::Vector2 PositionVector);	
	// hollow cyclinder, but not technically closed at the end, open like a
	// toilet paper roll. Kinda works well enough for most applications, since
	// the ends are usually thin, but a solution using another pair of solid
	// cylinders at the ends would help
	protected:
	bool hollow;
	// flag that determines which type of cylinder it is
	double Inner_radius, Outer_radius, height;
	// Height is the length along the z axis here
	double k;	
	// Given that dimensions should remain constant for all Inertia objects,
	// it makes sense to do that horrific part of the computation once at
	// construction. So as long as the dimensions of the shape are unchanged
	// we dont need to recalculate k each call
	
	// this should be possible to recalculate on demand eventually, maybeish?
	// not really sure
	
	// the protected access is wait, it does nothing ?
	// I think I misunderstood this, the protected access is pointless here
	// maybe not, need to be careful with this
	public:	
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	// moving parent pivot version
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	// and assuming the pivot is (0,0)
	~Inertia_cylinder();
};	

// Spheres, hollow and solid ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_sphere: public Inertia_moment
{	// probably the easiest to do because spheres are basically the same no
	// matter which angle you look at them from
	// kinda trippy if you stop to think about it
	public:
	Inertia_sphere(double radius, VectorVictor::Vector2 PositionVector);								
	// Solid sphere. Not sure what the point of it would be, but who cares
	Inertia_sphere(double inner_radius, double outer_radius, VectorVictor::Vector2 PositionVector);	
	// Sphere with an internal cavity, of radius inner_radius and a different
	// density than the outer shell. Could be a fuel tank or anything really
	// the best structure for confining a pressure is supposed to be a sphere
	// anyways
	protected:
	bool hollow;
	// which type of sphere we have
	double Inner_radius, Outer_radius;
	// physical parameters, in meters as always
	double k;	
	// the inertia constant calculated at construction
	
	// maybe the protected access is so nothing messes with the hollow state?
	// that would make things fly out the window
	public:
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	// the inertia with a moveable pivot point
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	// Inertia if the pivot point is assumed to be (0,0)
	~Inertia_sphere();
};

// Boxes, hollow and solid /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Inertia_box: public Inertia_moment
{	// an actual rectangular box sitting with its height perpendicular
	// to the xy plane
	
	// The heights dont appear to have any effect on the inertia, but
	// we keep them for some strange reason
	public:
	Inertia_box(double length, double width, double height, VectorVictor::Vector2 PositionVector);														
	// Solid box, constant density
	Inertia_box(double outer_length, double outer_width, double inner_length, double inner_width, double outer_height, double inner_height, VectorVictor::Vector2 PositionVector);	
	// Box with box-shaped cavity centered inside having a different density 
	protected:
	bool hollow;
	double Inner_length, Inner_width, Inner_height;
	double Outer_length, Outer_width, Outer_height;
	// properties of the object
	double k;	
	// inertia constant based on dimensions
	public:
	double Get_moment_about_pivot(VectorVictor::Vector2 pivot_point, double inside_mass, double outside_mass);
	// moveable origin version
	double Get_moment_about_pivot(double inside_mass, double outside_mass);
	// assuming that the pivot is (0,0)
	~Inertia_box();
};

#endif
