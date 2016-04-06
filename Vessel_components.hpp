// Vessel_components.hpp ///////////////////////////////////////////////////////
// The classes for any kind of part that makes up a Newtonian object////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Keplerian_objects.hpp"

#ifndef vesselpartsh
#define vesselpartsh

typedef id part_id;

class Vessel_component
{	public:
	
	Vessel_component(VectorVictor::Vector2 component_position);
	
	static part_id Component_index;
	// the id # that will be assigned to the next object of this type created
	part_id Get_new_index();
	// returns a new index to assign to an object and increments the counter
	// by one
	part_id Component_id;
	// the current objects id #
	part_id Get_component_id();
	// the global list of all objects of type Vessel component
	void New_vessel_component(Vessel_component * new_this);		
	// inserts the reference to the new vessel component into the global
	// vessel component list
	virtual void Update_component(double dt, std::vector<Force> &parent_force_list);
	// the equivalent of Frame() for parts that make up vessels. parent force
	// list is passed by reference so forces contributed by parts to their
	// parent vessel can be tacked on
	virtual void Draw_component(SFML_Window * iwindow, bool Map_status);
	// currently does nothing..., but I think this is meant to be used later
	// on for drawing individual parts, some of which will opt to do nothing
	// instead if the part really shouldnt be drawn all the time
	Inertia_moment * Component_moment;
	// the moment of inertia of the part, which can be of various types
	// depending on the shape of the part (cylinder, box, sphere)
	
	VectorVictor::Vector2 Component_position;
	// the position of the part relative to the parent vessel
	
	// long double seems like overkill here, maybe a float or double vector
	// should be sufficient at least
	
	virtual double Get_component_mass();
	// return the total mass of the part
	virtual double Get_component_inertia(VectorVictor::Vector2 axis);
	// returns the moment of inertia of the part, including the application of
	// the parallel axis theorem
	
	
	long double Radius;
	long double Get_radius();
	// ie we treat the part as a simple circle lying in the plane of the
	// game world and it has some radius
	
	
	Vessel_component* Get_vessel_component_pointer();
	// returns a pointer of type vessel component, so we can get the value of
	// this even from a child object
	~Vessel_component();

};




// Now that we have the general type for parts, we define some specific types
// of parts that we will obviously need a lot in any vessel

class Resource_Tank: public Vessel_component
{	// not just fuel tanks, any sort of component that can store a resource
	// having mass. Not necessarily electric charge though... hmm, that could
	// use a bit more thought
	public:
	Resource_Tank(double initial_tank_resource_mass, double tank_mass, double inner_radius, double outer_radius, double tank_length, VectorVictor::Vector2 PositionVector);	
	// everything is cylinders now for some reason. We should be able to have
	// other shapes eventually though
	virtual void Update_component(double dt, std::vector<Force> &parent_force_list);
	// still virtual cause, um actually this shouldnt be virtual, 
	// (or neednt be anyways...)
	
	// this needs to be assesed, probably easily changed
	double Tank_mass, Resource_mass;
	// what it sounds like. In kilos
	bool Empty;
	// Flag to check so we dont keep trying to suck fuel out of an empty tank 
	double Get_resource_mass();
	// again simple, just the resource mass, ignoring the mass of the tank
	// structure
	double Get_component_mass();
	// tank and resource mass together
	double Get_component_inertia(VectorVictor::Vector2 axis);	
	// same as explained above. Total moment of inertia for the resource tank
	~Resource_Tank();
};

enum thruster_group{rotate_clockwise, rotate_counterclockwise, translate_forward, translate_back, translate_right, translate_left, main_engines, retro_engines, hover_engines};

enum rotation_direction{clockwise, counterclockwise};

class Thruster: public Vessel_component
{	public:
	// no constructor since we never create a generic thruster, only its
	// derived types as defined below
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	// standard update function called by the parent vessel
	double Exhaust_velocity, Maximum_flow_rate, Thruster_mass;
	// common info about the given engine & what it can do. Exhaust_velocity
	// should actually be max Ve, but not really a big deal, just semantics.
	double Thruster_throttle;
	// thruster throttle is a value between 0 and 1, with 1 being full throttle
	// and 0 being completely shut off		
	
	// mix ratio is (mass oxidizer/mass fuel) for optimum exhaust velocity
	
	VectorVictor::Vector2 Thruster_direction;
	// normalized along with direction at construction if necessary, since its
	// gotta have a magnitude of 1
	// this is the direction that the nozzle points in, opposite to the
	// direction of the actual force vector
	
	// again, long double seems like massive overkill here
	std::vector<thruster_group> Groups;
	// a list of all of the groups that
	bool Is_in_group(thruster_group group);
	// returns if this thruster is of the type passed by group
	void Throttle_down(double dt, double k_throttle);
	void Throttle_up(double dt, double k_throttle);
	// exactly what they sound like, k_throttle moderates how fast the
	// slider moves, has units of (slider units/s)	
	void Throttle_to(double dt, double k_throttle, double Throttle_target);
	// instead of simply moving the throttle up or down, maybe we need to
	// move the throttle to a given setting. So we move towards that setting
	// at our max rate, and if we are close enough, we fine tune the k_throttle
	// back so that we dont overshoot (this is done in the function)
	bool empty_tank;		
	
	// this is completely temporary	// I swear, totally	
	// actually this would be good as a quick "precheck before continuing" sort
	// of thing // Ill at least make it uppercase
	
	// ^ No idea what that was. I think it was just a simple check to avoid
	// useless operations when the fuel tank is empty. It should be handled
	// carefully so that the engine reengages when the tank is refueled, or
	// is swapped though

	// I think I see the danger here now, this area could cause problems
	// really dont like this, it needs to be changed
	
	bool Is_RCS;
	
	virtual long double Get_maximum_torque(double dt);
	
	double Get_component_mass();
	double Get_component_inertia(VectorVictor::Vector2 axis);		
	// same as usual, physical properties of the thruster
	Thruster* Get_thruster_pointer();
	// another abstraction like Vessel Component pointers
	// this case applies to any component that acts as a thruster
};
// on second thought, does this even really need to have hover engines?
// the difference between hover and main in 2d is really non-existent


class Monopropellant_thruster: public Thruster
{	public:
	// any thruster which uses only one type of fuel 
	// (and in this case one fuel tank)
	Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one);
	Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one, thruster_group group_two);
	Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one, thruster_group group_two, thruster_group group_three);		
	Resource_Tank * Fuel_tank;
	// pointer to the fuel tank that we drain our fuel mass away from. This is...
	// ahh... not great practice, but I feel like a string based solution might
	// prove slow. At the very least, the mapping of tank to thruster could at
	// least happen a layer up with the main vessel? The danger here is that
	// segfault happens when we call destructor of the fuel tank. Its not good
	// practice in general
	
	// this needs a long hard look
	
	// one solution could be for the parent vessel to map tanks to thrusters via
	// a function call before parts are updated to avoid these issues
	// or... another solution might be to have "attachment points" like between
	// ksp parts, so that if the fuel tank is broken away, the pointer will
	// query the now disengaged connection first
	
	// again, this is an important part of the architecture, decisions on how to
	// structure this will have consequences elsewhere in the engine
	
	long double Get_maximum_torque(double dt);
	
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	// same as/see above
	~Monopropellant_thruster();
};

class Bipropellant_thruster: public Thruster
{	public:
	// this class is incomplete ATM, the already complicated algorithm for
	// monopropellant thrusters will become even more complex for this
	Bipropellant_thruster(double thruster_mass, double vexhaust, double optimal_mix_ratio, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, Resource_Tank * oxidizer_tank);	
	double Optimal_mixture_ratio;
	// mix ratio is (mass oxidizer/mass fuel) for optimum exhaust velocity
	// ratios above or below this experience Ve dropoffs defined by the output
	// of Get_exhaust_velocity(double mix_ratio);
	double Get_exhaust_velocity(double mix_ratio);
	// finds the exhaust velocity based on the ratio of propellants, which
	// needs to be tuned optimally for best results
	// I think the dropoff constant should be passed through to this as well,
	// that behaviour may vary depending on the specific fuel types
	// (and engine too probably)
	Resource_Tank * Fuel_tank, * Oxidizer_tank;
	// Again, horrible witchcraft, see above for explanation
	
	long double Get_maximum_torque(double dt);
	
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	// same as/see above
	~Bipropellant_thruster();
};


class Hull: public Vessel_component
{	// the basic object that any given vessel might need in order to cover its
	// complicated structure, ie, we could do the delta-gliders hull as a
	// million little cubes, spheres, etc., or we can just approximate its 
	// physical properties directly, since the hull usually has constant
	// properties, shape, size, mass, etc.
	public:
	Hull(double hull_mass, double inertia_factor, double hull_length, VectorVictor::Vector2 PositionVector);
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	// update is pointless here, but this function always needs definition, so..
	// might factor into physics later though.
	double Hull_mass, Hull_inertia, Length;		
	// Physical properties and Length which is important at different sim levels
	
	// hull distance just assumed to be zero, ie its location is always at the 
	// vessels coordinate system origin, since there isnt a good reason for it
	// to go anywhere else
	double Get_hull_length();
	double Get_hull_length_squared();
	double Get_component_mass();
	double Get_component_inertia(VectorVictor::Vector2 axis);
	// just regular methods to return data mentioned above		
	~Hull();
};




#endif
