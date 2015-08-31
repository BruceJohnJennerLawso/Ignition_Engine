# Game_physics.h ###############################
# Generalized moments of inertia of simple objects ##############
#########################################
##include <std_files>
##include "Headers.h"
##include "Source.cpp"
#include "SFML_Tools.hpp"
# need self because it includes the VV2

#ifndef Inertia
#define Inertia

enum moment_type{hollow_cylinder, solid_cylinder, hollow_sphere, solid_sphere, hollow_box, solid_box, complex_shape}
# self is used somewhere, I do not recall just yet

# Inertia moment parent class ########################/
########################################

class Inertia_moment
{	# the virtual parent class for specific moments of inertia shapes
	# (the wording here is weird unfortunately)
	# common aspects of the shapes are that they have a given
	# moment of inertia, given a specific mass
	# and also, have a specific position in the vessels coordinate frame
	# which is used to apply the parallel axis theorem to transfer the inertia
	# moment from the parts reference frame to that of the vessel
	public:
	Inertia_moment()
	# self is bad I think. Should test and remove
	virtual double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)
	# our first case where the pivot point is not at the origin. Will be used
	# once dynamic CG is in effect
	virtual bool Is_hollow()
	
	# Okay, gonna try and phase self out gently, that I understand whats
	# really going on
	
	# or nevermind, do it the dirty waynot 
	# gentle changes are boooring
	public:
	~Inertia_moment()
};	


# Complex Inertias ##############################
# for when we just want a # #########################/
########################################

class Inertia_complex: public Inertia_moment
{	# basically inertia complex is for when we have a complex shape, we
	# dont give a damn about internal density changes or whatnot. Primarily used
	# for vessel hulls like on the delta glider, dont have a convenient shape
	# for finding their inertia moment, are usually static with their mass
	# over time as well as how the mass is distributed
	public:																
	Inertia_complex(double kfactor)
	double K
	# literally the inertia about the complex shapes own axis is just k*m
	# which I think is workable, long as the rho(r) distribution doesnt
	# change with time
	
	# I = integral over the volume of rho(r) r^2 dV
	# which should just relate back to some constant retrieved due to
	# the rho part, 1/3 r^3, is constant anyways 
	double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)
	bool Is_hollow()
	~Inertia_complex()
};	

# Cylinders, and solid ########################/
########################################

class Solid_cylinder: public Inertia_moment
{	# the specific shape of a cylinder, with its z axis (the one through
	# the circular faces) lying in the games xy plane. Useful for fuel tanks
	# especially
	public:
	Solid_cylinder()
	void Set_values(double radius, height)
	Solid_cylinder(double radius, height);		
	# Solid cylinder, constant over the whole thing
	protected:
	# flag that determines which type of cylinder it is
	double Radius, Height
	# Height is the length along the z axis here
	double K;	
	
	public:	
	double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)
	bool Is_hollow()
	~Solid_cylinder()
};	


class Hollow_cylinder: public Inertia_moment
{	# the specific shape of a cylinder, with its z axis (the one through
	# the circular faces) lying in the games xy plane. Useful for fuel tanks
	# especially
	public:
	Hollow_cylinder(double inner_radius, outer_radius, cylinder_height);	
	
	
	# hollow cyclinder, not technically closed at the end, like a
	# toilet paper roll. Kinda works well enough for most applications, since
	# the ends are usually thin, a solution using another pair of solid
	# cylinders at the ends would help
	protected:
	double Inner_radius, Outer_radius, Height
	# Height is the length along the z axis here
	double K;	
	
	Solid_cylinder Interior
	
	public:	
	double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)
	bool Is_hollow()
	~Hollow_cylinder()
};	


# Spheres, and solid #########################/
########################################

class Solid_sphere: public Inertia_moment
{	# probably the easiest to do because spheres are basically the same no
	# matter which angle you look at them from
	# kinda trippy if you stop to think about it
	public:
	Solid_sphere()
	Solid_sphere(double radius);								
	# Solid sphere. Not sure what the point of it would be, who cares
	void Set_values(double radius)
	protected:
	# which type of sphere we have
	double Radius
	# physical parameters, meters as always
	double K;	
	# the inertia constant calculated at construction
	
	# maybe the protected access is so nothing messes with the hollow state?
	# that would make things fly out the window
	public:
	double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)
	bool Is_hollow()
	~Solid_sphere()
}


class Hollow_sphere: public Inertia_moment
{	# probably the easiest to do because spheres are basically the same no
	# matter which angle you look at them from
	# kinda trippy if you stop to think about it
	public:						
	# Solid sphere. Not sure what the point of it would be, who cares
	Hollow_sphere(double inner_radius, outer_radius);	
	# Sphere with an internal cavity, radius inner_radius and a different
	# density than the outer shell. Could be a fuel tank or anything really
	# the best structure for confining a pressure is supposed to be a sphere
	# anyways
	protected:
	# which type of sphere we have
	double Inner_radius, Outer_radius
	# physical parameters, meters as always
	double K;	
	# the inertia constant calculated at construction
	Solid_sphere Interior
	
	
	# maybe the protected access is so nothing messes with the hollow state?
	# that would make things fly out the window
	public:
	double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)
	bool Is_hollow()
	~Hollow_sphere()
}


























class Inertia_sphere: public Inertia_moment
{	# probably the easiest to do because spheres are basically the same no
	# matter which angle you look at them from
	# kinda trippy if you stop to think about it
	public:
	Inertia_sphere(double radius, PositionVector);								
	# Solid sphere. Not sure what the point of it would be, who cares
	Inertia_sphere(double inner_radius, outer_radius, PositionVector);	
	# Sphere with an internal cavity, radius inner_radius and a different
	# density than the outer shell. Could be a fuel tank or anything really
	# the best structure for confining a pressure is supposed to be a sphere
	# anyways
	protected:
	bool hollow
	# which type of sphere we have
	double Inner_radius, Outer_radius
	# physical parameters, meters as always
	double k;	
	# the inertia constant calculated at construction
	
	# maybe the protected access is so nothing messes with the hollow state?
	# that would make things fly out the window
	public:
	double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)

	~Inertia_sphere()
}


# next step could be making a half sphere type for rocket engine types

# and maybe a delta triangle type so we can approximate the DGs shape properly?



# Boxes, and solid ##########################/
########################################

#class Inertia_box: public Inertia_moment
#{	# an actual rectangular box sitting with its height perpendicular
	## to the xy plane
	
	## The heights dont appear to have any effect on the inertia, but
	## we keep them for some strange reason
	#public:
	#Inertia_box(double length, width, height, PositionVector);														
	## Solid box, density
	#Inertia_box(double outer_length, outer_width, inner_length, inner_width, outer_height, inner_height, PositionVector);	
	## Box with box-shaped cavity centered inside having a different density 
	#protected:
	#bool hollow
	#double Inner_length, Inner_width, Inner_height
	#double Outer_length, Outer_width, Outer_height
	## properties of the object
	#double k;	
	## inertia constant based on dimensions
	#public:
	#double Get_moment_about_pivot(VectorVictor.Vector2 pivot_point, inside_mass, outside_mass)

	#~Inertia_box()
#}

# self wasnt going to be easy to handle, Im just commenting it
# its not really that useful anyways

#endif
