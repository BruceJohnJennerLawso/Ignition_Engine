#include <math.h>
#include "Game_physics.hpp"

# Inertia moment parent class ########################/
########################################

Inertia_moment.Inertia_moment()
}	# self should go

def Get_moment_about_pivot(self, pivot_point, inside_mass, outside_mass):
{	return 0
}	# needs an error talkback message


def Is_hollow(self):
{	Talkback("Bad call to Inertia_moment.Is_hollow()")
	return False


Inertia_moment.~Inertia_moment()
}	# also bad maybe

# Complex Inertias ##############################
# for when we just want a # #########################/
########################################

Inertia_complex.Inertia_complex(double kfactor)
{	K = kfactor


def Get_moment_about_pivot(self, pivot_point, inside_mass, outside_mass):
{	moment = (inside_mass*K)
	# self should be changed, that a moment is actually something
	# different ;)

	if pivot_point.Get_vector_magnitude_squared() != 0:
	{	
		r = pivot_point.Get_vector_magnitude();	
		# we use that info to find the relative offsets magnitude
		moment += (inside_mass*(r*r));		
		#and finally use the PAT as stated above & below
		# I(r) = Icg + (m(r^2))	
	}	
	return moment


def Is_hollow(self):
{	return False



# old function, it for now

Inertia_complex.~Inertia_complex()



# Solid Cylinder ###############################
# Rho constant the whole way through the cylinder ##############/
########################################

Solid_cylinder.Solid_cylinder()
{	self.Set_values(1, 1)


def Set_values(self, radius, height):
{	Radius = radius
	Height = height
	K = ((Height*Height)/12)


Solid_cylinder.Solid_cylinder(double radius, cylinder_height)
{	self.Set_values(radius, cylinder_height)


def Get_moment_about_pivot(self, pivot_point, inside_mass, outside_mass):
{	moment = 0
	moment += (inside_mass*K)
	if pivot_point.Get_vector_magnitude_squared() != 0:
	{	r = pivot_point.Get_vector_magnitude()
		moment += (inside_mass*(r*r));							
		# I(r) = Icg + (m(r^2))	

	return moment


def Is_hollow(self):
{	return False


Solid_cylinder.~Solid_cylinder()


# Hollow Cylinder ##############################/
# A hollow cylinder, at both ends, a paper towel roll #######/
########################################


Hollow_cylinder.Hollow_cylinder(double inner_radius, outer_radius, cylinder_height)
{	Inner_radius = inner_radius
	Outer_radius = outer_radius
	Height = cylinder_height
	K = ((3*((outer_radius*outer_radius)+(inner_radius*inner_radius))) + (Height*Height))/12
	# whew, should be good enough for now
	Interior.Set_values(inner_radius, cylinder_height)


def Get_moment_about_pivot(self, pivot_point, inside_mass, outside_mass):
{	moment = 0
	moment += (outside_mass*K)
	# self is under some review right now, definitions might not have
	# been accurate
	if pivot_point.Get_vector_magnitude_squared() != 0:
	{	r = pivot_point.Get_vector_magnitude()
		moment += (outside_mass*(r*r));							
		# I(r) = Icg + (m(r^2))	

	moment += Interior.Get_moment_about_pivot(pivot_point, inside_mass, 0);	
	# get the moment of inertia of the inside part
	return moment


def Is_hollow(self):
{	return True


Hollow_cylinder.~Hollow_cylinder()



# Solid sphere, Rho throughout ###################/
########################################

Solid_sphere.Solid_sphere()


Solid_sphere.Solid_sphere(double radius)
{	self.Set_values(radius)


def Set_values(self, radius):
{	Radius = radius
	K = (2/5)*(radius*radius)
}	

def Get_moment_about_pivot(self, pivot_point, inside_mass, outside_mass):
{	moment = 0
	moment += (inside_mass*K)
	# self is under some review right now, definitions might not have
	# been accurate
	if pivot_point.Get_vector_magnitude_squared() != 0:
	{	r = pivot_point.Get_vector_magnitude()
		moment += (inside_mass*(r*r));							
		# I(r) = Icg + (m(r^2))	

	return moment


def Is_hollow(self):
{	return False


Solid_sphere.~Solid_sphere()


# Hollow sphere, a different density inside ###############
########################################

Hollow_sphere.Hollow_sphere(double inner_radius, outer_radius)
{	Inner_radius = inner_radius
	Outer_radius = outer_radius
	K = (2/5)*(((pow(outer_radius, 5))-(pow(inner_radius, 5)))/((pow(outer_radius, 3))-(pow(inner_radius, 3))))
	# ugly aint it



def Get_moment_about_pivot(self, pivot_point, inside_mass, outside_mass):
{	moment = 0
	moment += (outside_mass*K)
	# self is under some review right now, definitions might not have
	# been accurate
	if pivot_point.Get_vector_magnitude_squared() != 0:
	{	r = pivot_point.Get_vector_magnitude()
		moment += (outside_mass*(r*r));							
		# I(r) = Icg + (m(r^2))	

	moment += Interior.Get_moment_about_pivot(pivot_point, inside_mass, 0);	
	# get the moment of inertia of the inside part
	return moment


def Is_hollow(self):
{	return True


Hollow_sphere.~Hollow_sphere()

