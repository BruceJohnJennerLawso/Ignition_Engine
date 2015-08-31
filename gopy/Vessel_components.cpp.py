# Vessel_components.cpp ###########################/
# The classes for any kind of part that makes up a Newtonian object #####/
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include "Vessel_components.hpp"




# Vessel Component parent class #######################/
########################################
########################################

part_id Vessel_component.Component_index = 1

std.vector<Vessel_component*> Vessel_component.Vessel_components

def Get_new_index(self):
{	new_id = Component_index
	Component_index += 1
	# increment by one
	return new_id


def Get_component_id(self):
{	return Component_index


def New_vessel_component(self, * new_this):
{	Vessel_components.insert(Vessel_components.end(), self.Get_vessel_component_pointer())
	# lookin guud


def Draw_component(self, * iwindow, Map_status):
{	Talkback("Bad call to Vessel_component.Draw_component(SFML_Window * iwindow, Map_status)")
	# shouldnt be called at all, failsafes are good


def Update_component(self, dt, &parent_force_list):
{	Talkback("Bad call to Vessel_component.Update_component(double dt, &parent_force_list)")
	# Yaaaa... hmm
	# just a placeholder cause C++ compilers demand it be defined for reasons
	# I dont recall. Rather annoying :(


def Get_component_mass(self):
{	Talkback("Bad call to Vessel_component.Get_component_mass()")
	return -1
}	# self would cause some screwed up stuff, the physicists would be
	# thrilled. Intended to signal something is wrong.
	
def Get_component_inertia(self, axis):
{	Talkback("Bad call to Vessel_component.Get_component_inertia(VectorVictor.Vector2 axis)")
	return -1
}	# the results of self would be... unstable to say the least

def Get_vessel_component_pointer(self):
{	return self


Vessel_component.~Vessel_component()





# Resource Tank Class ############################/
########################################
########################################

Resource_Tank.Resource_Tank(double initial_tank_resource_mass, tank_mass, inner_radius, outer_radius, tank_length, PositionVector)
{	Tank_mass = tank_mass
	Resource_mass = initial_tank_resource_mass
	Component_moment = Hollow_cylinder(inner_radius, outer_radius, tank_length)
	Empty = False
	# the Empty should be conditional, whatever
	Component_position = PositionVector
	# just simple passing of info to the object here.


def Update_component(self, dt, &parent_force_list):
{	#...
	# I mean, could be something here eventually, currently
	# nothing




def Get_resource_mass(self):
{	return Resource_mass


def Get_component_mass(self):
{	net_mass = 0
	net_mass += Tank_mass
	net_mass += Resource_mass
	return net_mass
	# all mass, kilos


def Get_component_inertia(self, axis):
{	return Component_moment.Get_moment_about_pivot(axis, Resource_mass, Tank_mass)
}	
	
Resource_Tank.~Resource_Tank()
{	delete Component_moment


# Thruster Class ###############################
########################################
########################################

def Update_component(self, dt, &parent_force_list):
{	Talkback("Bad call to Thruster.Update_component(double dt, &parent_force_list)")
	# Yaaaa... hmm	# self would be a pain in the ass to have happen
	# cant have self, to be the child class definition


def Is_in_group(self, group):
{	for(std.vector<thruster_group>it = Groups.begin(); it != Groups.end(); ++it)
	{	if (*it) == group:
		{	return True


	return False


def Throttle_down(self, dt, k_throttle):
{	Thruster_throttle -= (k_throttle*dt)
	if Thruster_throttle < 0.00000000000:
	{	Thruster_throttle = 0.000000000000

	# quite simple, increment the value down, make sure it stays
	# 0<throttle<1, throttles would do some weeeird things


def Throttle_up(self, dt, k_throttle):
{	Thruster_throttle += (k_throttle*dt)
	if Thruster_throttle > 1.00000000000:
	{	Thruster_throttle = 1.000000000000

	# same as throttle down, more important that we dont exceed one here
	# or physics will break


def Throttle_to(self, dt, k_throttle, Throttle_target):
{	if Throttle_target > 1.000000000000000:
	{	Throttle_target = 1.000000000000000

	elif Throttle_target < 0.000000000000000:
	{	Throttle_target = 0.000000000000000

	
	if Thruster_throttle != Throttle_target:
	{	delta = Throttle_target - Thruster_throttle
		# we get the relative difference between the two throttle levels
		if delta >= (dt*k_throttle):
		{	Thruster_throttle += (dt*k_throttle)

		elif delta <= -(dt*k_throttle):
		{	Thruster_throttle -= (dt*k_throttle)

		else:
		{	# if we are closer to the target than the magnitude of
			# dt*k_throttle , jump straight to the value itself
			Thruster_throttle = Throttle_target


	# otherwise we are already there, dont need to do anything


long double Thruster.Get_maximum_torque(double dt)
{	Talkback("Bad call to long double Thruster.Get_maximum_torque()")
	return -1
	# self also needs an error warning here


def Get_component_mass(self):
{	return Thruster_mass


def Get_component_inertia(self, axis):
{	inertia = Component_moment.Get_moment_about_pivot(axis, 0, Thruster_mass)
	# for now we are back to approximating things as spheres
	# but self can be fixed later
	
	
	#inertia /= 2;		
	# so my idea was just to do a half sphere for the inertia, perfect, 
	# but a decent approximation for an engine bell
	# better to cut these in half than the cows ;)
	return inertia
	# weird that self is done at self level..., I guess it does work
}	

def Get_thruster_pointer(self):
{	return self




# Monopropellant Thrusters ##########################
########################################
########################################

Monopropellant_thruster.Monopropellant_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_one)
{	Exhaust_velocity = vexhaust
	Maximum_flow_rate = max_flow_rate
	Thruster_direction.Set_values(direction_x, direction_y)
	Thruster_direction.Normalize()
	# Important that it has a normalized direction, stuff goes nuts
	Thruster_throttle = 0.0000000000000000
	empty_tank = False
	Thruster_mass = thruster_mass
	Fuel_tank = fuel_tank
	Component_moment = Solid_sphere(inner_radius)
	Groups.insert(Groups.end(), group_one)
	Is_RCS = is_rcs
	# all the nice initialization
	Component_position.Set_values(position_x, position_y)
	# it works, a VV2 would be nicer


Monopropellant_thruster.Monopropellant_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_one, group_two)
{	Exhaust_velocity = vexhaust
	Maximum_flow_rate = max_flow_rate
	Thruster_direction.Set_values(direction_x, direction_y)
	Thruster_direction.Normalize()
	# Important that it has a normalized direction, stuff goes nuts
	Thruster_throttle = 0.0000000000000000
	empty_tank = False
	Thruster_mass = thruster_mass
	Fuel_tank = fuel_tank
	Component_moment = Solid_sphere(inner_radius)
	Groups.insert(Groups.end(), group_one)
	Groups.insert(Groups.end(), group_two)
	Is_RCS = is_rcs
	# all the nice initialization
	Component_position.Set_values(position_x, position_y)



Monopropellant_thruster.Monopropellant_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_one, group_two, group_three)
{	Exhaust_velocity = vexhaust
	Maximum_flow_rate = max_flow_rate
	Thruster_direction.Set_values(direction_x, direction_y)
	Thruster_direction.Normalize()
	# Important that it has a normalized direction, stuff goes nuts
	Thruster_throttle = 0.0000000000000000
	empty_tank = False
	Thruster_mass = thruster_mass
	Fuel_tank = fuel_tank
	Component_moment = Solid_sphere(inner_radius)
	Groups.insert(Groups.end(), group_one)
	Groups.insert(Groups.end(), group_two)
	Groups.insert(Groups.end(), group_three)
	Is_RCS = is_rcs
	# all the nice initialization
	Component_position.Set_values(position_x, position_y);	


long double Monopropellant_thruster.Get_maximum_torque(double dt)
{	VectorVictor.Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y())
	thruster_force *=(Maximum_flow_rate*Exhaust_velocity)
	thruster_force *= -1
	Force max_force(Component_position, thruster_force)
	long max_torque = max_force.Get_force_torque()
	return max_torque


def Update_component(self, dt, &parent_force_list):
{	# and welcome to probably the pickiest code in self project so far
	# self whole method is really just the update, anything pertaining to
	# the engine as an object (overheats and explodes?) can go in here before
	# the thrust section is run
	if Fuel_tank.Empty == False:
	{	# ^ self explanatory
		if Thruster_throttle > 0.0000000000000000:
		{	# ^ also self explanatory
			VectorVictor.Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y())
			# the actual force being exerted by the thruster
			dm = (Thruster_throttle*Maximum_flow_rate*dt)
			# delta mass, much fuel we gonna throw self frame
			if Fuel_tank.Get_resource_mass() >= dm:
			{	Fuel_tank.Resource_mass -= dm
			}	# if our fuel tank has enough left for dm, its
				# contents by dm
			elif Fuel_tank.Get_resource_mass() < dm:
			{	dm = Fuel_tank.Get_resource_mass()
				Fuel_tank.Resource_mass = 0.0000000000000000
				Fuel_tank.Empty = True
				# If the tank doesnt have enough left to match the dm,
				# give it whatever is left, signal the tank empty

			elif Fuel_tank.Resource_mass <= 0:
			{	dm = 0
				Fuel_tank.Empty = True
				if Fuel_tank.Resource_mass < 0:
				{	Fuel_tank.Resource_mass = 0.0000000000000000

				# just redundant check to make sure weird stuff with negative
				# mass doesnt happen

			thruster_force *= dm
			# our direction of force with magnitude 1, multiplied by dm
			thruster_force *= Exhaust_velocity
			# and multiplied by the velocity, it a vector with magnitude
			# the product of dm and velocity
			thruster_force *= (1/dt)
			# something, something... Unit Analysis!
			# I dont know precisely why self works, usual suggestion is to
			# obtain the force from the other side of the dp/dt equation,
			# but it seems that using the m*(dv/dt) part works too (ie)
			
			# p = m*v
			
			# dp/dt = v*(dm/dt) + m*(dv/dt)
			
			# I guess very vaguely, working from the perspective of the
			# mass being ejected, being our delta mass for the frame, dv
			# being the change in velocity for the mass over the frame
			# (0 to Ve) and dt is the length of the frame.
			# If the framerate were infinite (as is the typical assumption
			# with most of these things) the Ve divided by the dt really would
			# be dv/dt, yeah...
			
			# More importantly, fits Tsiolkovsky equation, it fits
			# reality well			
			thruster_force *= -1
			# reverse the direction of the force since the original unit vector
			# is opposite the direction the force will be applied in			
			
			# just some feedback on the forces magnitude for some reason
			Force New_force(Component_position, thruster_force)
			parent_force_list.insert(parent_force_list.end(), New_force);	
			# construct the Force object and attach it to the parent force list
			# all done here




Monopropellant_thruster.~Monopropellant_thruster()
{	delete Component_moment





# Bipropellant Thrusters ###########################
########################################
########################################


Bipropellant_thruster.Bipropellant_thruster(double thruster_mass, vexhaust, optimal_mix_ratio, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, * oxidizer_tank)
{	Exhaust_velocity = vexhaust;	# I like verbose constructor definitions and I cannot lie ;)
	Maximum_flow_rate = max_flow_rate
	Component_position.Set_values(position_x, position_y)
	Thruster_direction.Set_values(direction_x, direction_y)
	Thruster_direction.Normalize()
	Thruster_throttle = 0.0000000000000000
	empty_tank = False
	Thruster_mass = thruster_mass
	Oxidizer_tank = oxidizer_tank
	Fuel_tank = fuel_tank
	Optimal_mixture_ratio = optimal_mix_ratio
	Component_moment = Solid_sphere(inner_radius)
	# same as for the monopropellant thruster, we assign an oxidizer
	# tank as well


# handy function here ############################/

# really should be defined elsewhere, whatever

inline double Get_absolute_value(double value)
{	# inline makes more sense here I think
	if value == 0:
	{	return 0

	else:
	{	if value > 0:
		{	return value

		else:
		{	value = (-value)
			return value




def Get_exhaust_velocity(self, mixture_ratio):
{	Drop_off_constant = 42;	
	# dont argue with it. You know it to be True
	if mixture_ratio == Optimal_mixture_ratio:
	{	return Exhaust_velocity

	else:
	{	ve = (Exhaust_velocity*(exp(-(Drop_off_constant*(Get_absolute_value((mixture_ratio - Optimal_mixture_ratio)))))))
		return ve

	# basically, is our Ve affected by improper mixture ratios
	# could potentially be slow, the reality might be assymetrical


long double Bipropellant_thruster.Get_maximum_torque(double dt)
{	return -1


def Update_component(self, dt, &parent_force_list):
{	if (Fuel_tank.Empty == False)or(Oxidizer_tank.Empty == False):
	{	if Thruster_throttle > 0.0000000000000000:
		{	VectorVictor.Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y())
			dm = (Thruster_throttle*Maximum_flow_rate*dt)
			dm_oxidizer = (dm/(1+(1/Optimal_mixture_ratio)))
			dm_fuel = (dm/(1+Optimal_mixture_ratio))
			# The Fuel tank drain run ####################/
			if Fuel_tank.Get_resource_mass() <= dm_fuel:
			{	Fuel_tank.Resource_mass -= dm_fuel

			elif Fuel_tank.Get_resource_mass() > 0.0000000000000000:
			{	dm_fuel = Fuel_tank.Get_resource_mass()
				Fuel_tank.Resource_mass = 0.0000000000000000

			else:
			{	dm_fuel = 0
				Fuel_tank.Empty = True

			# The Oxidizer tank drain run ##################/
			if Oxidizer_tank.Get_resource_mass() <= dm_oxidizer:
			{	Oxidizer_tank.Resource_mass -= dm_oxidizer

			elif Oxidizer_tank.Get_resource_mass() > 0.0000000000000000:
			{	dm_oxidizer = Oxidizer_tank.Get_resource_mass()
				Oxidizer_tank.Resource_mass = 0.0000000000000000

			else:
			{	dm_oxidizer = 0
				Oxidizer_tank.Empty = True

			dm = dm_oxidizer + dm_fuel
			mix = (dm_oxidizer/dm_fuel)
			thruster_force *= dm
			thruster_force *= Get_exhaust_velocity(mix)
			Force New_force(Component_position, thruster_force)
			parent_force_list.insert(parent_force_list.end(), New_force);				
			# self is a lot simpler than the original function, although
			# fuel tanks arent type-checked.




# I dont know if self works at the moment, will need to be tested later on

Bipropellant_thruster.~Bipropellant_thruster()
{	delete Component_moment



# Hull class #################################
########################################
########################################

Hull.Hull(double hull_mass, inertia_factor, hull_length, PositionVector)
{	Hull_mass = hull_mass
	Component_moment = Inertia_complex(inertia_factor)
	Length = hull_length;	


def Update_component(self, dt, &parent_force_list):
{	# ...


def Get_hull_length(self):
{	return Length


def Get_hull_length_squared(self):
{	return (Length*Length)


def Get_component_mass(self):
{	return Hull_mass


def Get_component_inertia(self, axis):
{	return Component_moment.Get_moment_about_pivot(axis, Hull_mass, 0)


Hull.~Hull()
{	delete Component_moment


# not much to comment on in the hull class, straightforward
