# Vessel_components.hpp ###########################/
# The classes for any kind of part that makes up a Newtonian object######
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Keplerian_objects.hpp"

#ifndef vesselpartsh
#define vesselpartsh

typedef id part_id

class Vessel_component
{	public:
	static part_id Component_index
	# the id # that will be assigned to the next object of self type created
	part_id Get_new_index()
	# returns a index to assign to an object and increments the counter
	# by one
	part_id Component_id
	# the current objects id #
	part_id Get_component_id()
	static std.vector<Vessel_component*> Vessel_components
	# the global list of all objects of type Vessel component
	void New_vessel_component(Vessel_component * new_this);		
	# inserts the reference to the vessel component into the global
	# vessel component list
	virtual void Update_component(double dt, &parent_force_list)
	# the equivalent of Frame() for parts that make up vessels. parent force
	# list is passed by reference so forces contributed by parts to their
	# parent vessel can be tacked on
	virtual void Draw_component(SFML_Window * iwindow, Map_status)
	# currently does nothing..., I think self is meant to be used later
	# on for drawing individual parts, of which will opt to do nothing
	# instead if the part really shouldnt be drawn all the time
	Inertia_moment * Component_moment
	# the moment of inertia of the part, can be of various types
	# depending on the shape of the part (cylinder, box, sphere)
	
	VectorVictor.Vector2 Component_position
	# the position of the part relative to the parent vessel
	
	# long double seems like overkill here, a float or double vector
	# should be sufficient at least
	
	virtual double Get_component_mass()
	# return the total mass of the part
	virtual double Get_component_inertia(VectorVictor.Vector2 axis)
	# returns the moment of inertia of the part, the application of
	# the parallel axis theorem
	
	
	Vessel_component* Get_vessel_component_pointer()
	# returns a pointer of type vessel component, we can get the value of
	# self even from a child object
	~Vessel_component()






# Now that we have the general type for parts, define some specific types
# of parts that we will obviously need a lot in any vessel

class Resource_Tank: public Vessel_component
{	# not just fuel tanks, sort of component that can store a resource
	# having mass. Not necessarily electric charge though... hmm, could
	# use a bit more thought
	public:
	Resource_Tank(double initial_tank_resource_mass, tank_mass, inner_radius, outer_radius, tank_length, PositionVector);	
	# everything is cylinders now for some reason. We should be able to have
	# other shapes eventually though
	virtual void Update_component(double dt, &parent_force_list)
	# still virtual cause, actually self shouldnt be virtual, 
	# (or neednt be anyways...)
	
	# self needs to be assesed, easily changed
	double Tank_mass, Resource_mass
	# what it sounds like. In kilos
	bool Empty
	# Flag to check so we dont keep trying to suck fuel out of an empty tank 
	double Get_resource_mass()
	# again simple, the resource mass, the mass of the tank
	# structure
	double Get_component_mass()
	# tank and resource mass together
	double Get_component_inertia(VectorVictor.Vector2 axis);	
	# same as explained above. Total moment of inertia for the resource tank
	~Resource_Tank()


enum thruster_group{rotate_clockwise, rotate_counterclockwise, translate_forward, translate_back, translate_right, translate_left, main_engines, retro_engines, hover_engines

enum rotation_direction{clockwise, counterclockwise

class Thruster: public Vessel_component
{	public:
	# no constructor since we never create a generic thruster, its
	# derived types as defined below
	void Update_component(double dt, &parent_force_list)
	# standard update function called by the parent vessel
	double Exhaust_velocity, Maximum_flow_rate, Thruster_mass
	# common info about the given engine & what it can do. Exhaust_velocity
	# should actually be max Ve, not really a big deal, semantics.
	double Thruster_throttle
	# thruster throttle is a value between 0 and 1, 1 being full throttle
	# and 0 being completely shut off		
	
	# mix ratio is (mass oxidizer/mass fuel) for optimum exhaust velocity
	
	VectorVictor.Vector2 Thruster_direction
	# normalized along with direction at construction if necessary, its
	# gotta have a magnitude of 1
	# self is the direction that the nozzle points in, to the
	# direction of the actual force vector
	
	# again, double seems like massive overkill here
	std.vector<thruster_group> Groups
	# a list of all of the groups that
	bool Is_in_group(thruster_group group)
	# returns if self thruster is of the type passed by group
	void Throttle_down(double dt, k_throttle)
	void Throttle_up(double dt, k_throttle)
	# exactly what they sound like, moderates how fast the
	# slider moves, units of (slider units/s)	
	void Throttle_to(double dt, k_throttle, Throttle_target)
	# instead of simply moving the throttle up or down, we need to
	# move the throttle to a given setting. So we move towards that setting
	# at our max rate, if we are close enough, fine tune the k_throttle
	# back so that we dont overshoot (self is done in the function)
	bool empty_tank;		
	
	# self is completely temporary	# I swear, totally	
	# actually self would be good as a quick "precheck before continuing" sort
	# of thing # Ill at least make it uppercase
	
	# ^ No idea what that was. I think it was just a simple check to avoid
	# useless operations when the fuel tank is empty. It should be handled
	# carefully so that the engine reengages when the tank is refueled, or
	# is swapped though

	# I think I see the danger here now, area could cause problems
	# really dont like self, needs to be changed
	
	bool Is_RCS
	
	virtual long double Get_maximum_torque(double dt)
	
	double Get_component_mass()
	double Get_component_inertia(VectorVictor.Vector2 axis);		
	# same as usual, properties of the thruster
	Thruster* Get_thruster_pointer()
	# another abstraction like Vessel Component pointers
	# self case applies to any component that acts as a thruster

# on second thought, self even really need to have hover engines?
# the difference between hover and main in 2d is really non-existent


class Monopropellant_thruster: public Thruster
{	public:
	# any thruster which uses only one type of fuel 
	# (and in self case one fuel tank)
	Monopropellant_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_one)
	Monopropellant_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_one, group_two)
	Monopropellant_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_one, group_two, group_three);		
	Resource_Tank * Fuel_tank
	# pointer to the fuel tank that we drain our fuel mass away from. This is...
	# ahh... not great practice, I feel like a string based solution might
	# prove slow. At the very least, mapping of tank to thruster could at
	# least happen a layer up with the main vessel? The danger here is that
	# segfault happens when we call destructor of the fuel tank. Its not good
	# practice in general
	
	# self needs a long hard look
	
	# one solution could be for the parent vessel to map tanks to thrusters via
	# a function call before parts are updated to avoid these issues
	# or... another solution might be to have "attachment points" like between
	# ksp parts, that if the fuel tank is broken away, pointer will
	# query the now disengaged connection first
	
	# again, is an important part of the architecture, on how to
	# structure self will have consequences elsewhere in the engine
	
	long double Get_maximum_torque(double dt)
	
	void Update_component(double dt, &parent_force_list)
	# same as/see above
	~Monopropellant_thruster()


class Bipropellant_thruster: public Thruster
{	public:
	# self class is incomplete ATM, already complicated algorithm for
	# monopropellant thrusters will become even more complex for self
	Bipropellant_thruster(double thruster_mass, vexhaust, optimal_mix_ratio, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, * oxidizer_tank);	
	double Optimal_mixture_ratio
	# mix ratio is (mass oxidizer/mass fuel) for optimum exhaust velocity
	# ratios above or below self experience Ve dropoffs defined by the output
	# of Get_exhaust_velocity(double mix_ratio)
	double Get_exhaust_velocity(double mix_ratio)
	# finds the exhaust velocity based on the ratio of propellants, which
	# needs to be tuned optimally for best results
	# I think the dropoff constant should be passed through to self as well,
	# that behaviour may vary depending on the specific fuel types
	# (and engine too probably)
	Resource_Tank * Fuel_tank, Oxidizer_tank
	# Again, witchcraft, above for explanation
	
	long double Get_maximum_torque(double dt)
	
	void Update_component(double dt, &parent_force_list)
	# same as/see above
	~Bipropellant_thruster()



class Hull: public Vessel_component
{	# the basic object that any given vessel might need in order to cover its
	# complicated structure, ie, could do the delta-gliders hull as a
	# million little cubes, spheres, etc., we can just approximate its 
	# physical properties directly, the hull usually has constant
	# properties, shape, size, mass, etc.
	public:
	Hull(double hull_mass, inertia_factor, hull_length, PositionVector)
	void Update_component(double dt, &parent_force_list)
	# update is pointless here, self function always needs definition, so..
	# might factor into physics later though.
	double Hull_mass, Hull_inertia, Length;		
	# Physical properties and Length which is important at different sim levels
	
	# hull distance just assumed to be zero, its location is always at the 
	# vessels coordinate system origin, there isnt a good reason for it
	# to go anywhere else:
	double Get_hull_length()
	double Get_hull_length_squared()
	double Get_component_mass()
	double Get_component_inertia(VectorVictor.Vector2 axis)
	# just regular methods to return data mentioned above		
	~Hull()





#endif
