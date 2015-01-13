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

class Vessel_component
{	public:
	// anything on a vessel that has mass and does stuff
	virtual void Update_component(double dt, std::vector<Force> &parent_force_list);
	// the equivalent of Frame() for parts that make up vessels. parent force
	// list is passed by reference so forces contributed by parts to their
	// parent vessel can be tacked on
	virtual void Draw_component(SFML_Window * iwindow, bool Map_status);
	// a nice way of outsourcing the rendering on a part by part basis. This
	// allows parts to handle their own rendering like exhaust from an engine,
	// or the internal animations of a robotic arm or something
	
	// currently vessels are drawn onscreen by their hull object
	Inertia_moment * Component_moment;
	// object oriented way of getting the moment of inertia of a given simply
	// shaped part. This may prove a bit inadequate especially if the part is
	// dynamic/highly complex. Best option might be to stick with the virtual
	// call to Get_component_inertia() and sort out issues on a case-by-case
	// basis
	
	// this should be an issue on github
	Vessel_component* Get_vessel_component_pointer();
	// a wee bit confusing, but the idea here is that parts defined by more
	// specific implementations can still pass a general definition pointer
	// kind of like saying houses can come in many shapes, sizes, configurations
	// but they all have an address. This is like give me the address
	virtual double Get_component_mass();
	// simple as it sounds. Virtual for obvious reasons
	virtual double Get_component_inertia();
	// simple as it sounds, just retrieving the moment of inertia for the part
	// not sure where the parallel axis theorem is applied to this though, need
	// to find that
	~Vessel_component();
	
	friend class CNewtonian_Object;		// Didnt fix it anyways
	// this was an old hack, the purpose of which I dont recall. Should be
	// removed, but I want to test the change when I do it.
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
	double Get_tank_inertia();
	// just a quick method to automate things. Exactly what it sounds like
	double Get_resource_mass();
	// again simple, just the resource mass, ignoring the mass of the tank
	// structure
	double Get_component_mass();
	// tank and resource mass together
	double Get_component_inertia();	
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
	VectorVictor::Vector2 Thruster_position;						
	// coordinates local to the parent vessel where the thruster has its origin
	// these are just relative to positions of where things are located on a
	// vessel, might change relative to the center of mass once that feature is
	// implemented 
	VectorVictor::Vector2 Thruster_direction;
	// normalized along with direction at construction if necessary, since its
	// gotta have a magnitude of 1
	// this is the direction that the nozzle points in, opposite to the
	// direction of the actual force vector
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
	double Get_component_inertia();		
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
	double Get_hull_inertia();
	double Get_component_mass();
	double Get_component_inertia();
	// just regular methods to return data mentioned above		
	~Hull();
};




#endif
