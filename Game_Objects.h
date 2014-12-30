// Game_Objects.h //////////////////////////////////////////////////////////////
// All of the core classes that the ignition engine requires to function, //////
// including Newtonian and Keplerian parent classes for all celestial bodies ///
// and vessels /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Inertia_moment.h"

#ifndef GameObjects
#define GameObjects

#define Max_map_scale 16
// the maximum scale of the map view from the base window size, ie if the base
// window has a width of 1024 pixels, its maximum zoom scale will encompass
// 1024*(10^16) meters in universe. Thats about 100 light years or so if I
// recall correctly, or 1000? The scale might only go up to 10^15 or something
// in actual practice

// I should look that up

#define Min_map_scale 1

#define Max_cam_scale 10.00
#define Min_cam_scale 0.01

class Force
{	public:
	// all really just a nice placeholder for two vectors, since a force acting
	// on a body has a point where it acts on the body and a direction
	Force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force);
	// simple as it gets, although maybe an alternate for direct variables might
	// be nice here
	VectorVictor::Vector2 Attack_point;
	VectorVictor::Vector2 Force_vector;
	// our vectors...
	double Get_force_torque();
	// basically find the cross product of the two vectors, which should always
	// only have a z component if the vectors are all 2d. This simple version
	// just assumes that the origin we want the waaaiit. Oh shit
	
	// okay, now that should be cleaned up. The basic idea here is that the
	// vanilla Get_force_torque(); assumes our origin to be at (0,0), when we
	// may want to translate it around (center of mass)
	double Get_force_torque(VectorVictor::Vector2 reference_point);
	// and the second version of the function mentioned above, where our origin
	// is shifted by reference_point
	VectorVictor::Vector2 Get_force_vector();
	// just as simple as it sounds
	VectorVictor::Vector2 Get_force_vector(double angle);
	// returns the vector rotated by the given angle clockwise around the origin
	// angle is inputted in degrees. No affect to the direction of the stored
	// Force vector in this object
	// I dont recall where this is used, should look that up
	~Force();
};

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

// Celestial Bodies ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class CKeplerian_Object
{	public:
	// not sure if keplerian is the right category, but this type is for any
	// body that "moves on rails" since dynmic update of a planet would be
	// agony.
	virtual void Frame(double dt, long double simtime);
	long double Theta, Omega;
	// rotation and angular rate, both stored in degrees, but can be accessed as
	// radians where required
	long double Get_omega();
	// should add this for radians
	long double Get_theta_in_degrees();		
	long double Get_theta_in_radians();
	// self explanatory I think
	
	long double Simulation_time;
	// ahh, this I do not really like, it sort of syncs between the master
	// ignition engine objects simtime, since this object needs to know the
	// current time with precision to know its position (or will soon anyways)
	
	long double Radius;
	// in meters. Sort of a global mean, specific radii will be determined later
	// by  Get_radius(doubl longitude);
	long double Get_radius(double longitude);
	// Simple as it gets, just return the terrain height at given longitude
	// from the planets prime meridian (all the little greenwiches xD)
	// This just returns the constant above for the moment, but the world will
	// not always be a perfect sphere!	
	long double Mass;
	// in kilograms
	long double Get_mass();
	// return for above	
	VectorVictor::Vector2 Get_position(long double sim_time);
	// position at a given time. This is wacky in its current form, better if it
	// were updated every frame and just stored until needed. This was overthink
	// methinks
	void Gravitate(long double satellite_mass, long double satellite_rotation, VectorVictor::Vector2 satellite_position, std::vector<Force> &parent_force_list);
	// new function that tacks its gravity force onto a newtonian object when
	// requested by that function. May not last long given changes in how the 
	// state updaters work, but we will see
	std::string Object_name;
	// Welcome to <Object_name>!
	std::string Get_object_name();
	// trivial
	sf::Texture * Object_texture;
	// odd, the texture of the planet is global...
	// I think this was related to multiple sprites being generated for
	// different zoom factors, but I dont see a specific reason why they couldnt
	// be stored here instead
	virtual bool In_view(SFML_Window * window, int zoom_factor);	
	// how we check if the planet should be drawn
	virtual void Draw_flag(SFML_Window * iwindow, int zoom_factor);
	// and how we do it if it should
};

//std::vector<CKeplerian_Object*> Celestial_list;
// naughty, naughty, tsk tsk
// maybe this could use the pseudo-header guards to avoid issues

class TPlanet: public CKeplerian_Object
{	public:
	// basic type derived from Keplerian object that handles official planets
	// and mostly dwarf planets too. Basically anything that orbits the sun and
	// is to big to realistically handle as a newtonian object
	TPlanet(long double initial_theta, long double omega, long double radius, long double atmosphere_height, long double mass, std::string planet_texture_path);
	void Frame(double dt, long double simtime);
	VectorVictor::Vector2 Get_position(long double sim_time);
	// this... this should work a bit differently, sim_time should be implicitly
	// now, position evaluated and stored each frame
	std::vector<sf::Sprite*> Planet_sprites;
	// the whole sequence of sprites for each successive scaled map view
	// need to remember what the order was for scales
	// after a certain point of zooming out, we just assumed that the image
	// looks the same no matter how much farther we go
	bool In_view(SFML_Window * window, int zoom_factor);
	void Draw_flag(SFML_Window * iwindow, int zoom_factor);	
	// Similar deal as before. Should double check that the inview check uses
	// the correct radius for that map scale
	~TPlanet();
};

// Newtonian Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class CNewtonian_Object
{	public:
	// Nature and nature's laws lay hid in night;
	//	God said "Let Newton be" and all was light.
	
	// rather true
	// and I certainly wouldnt be doing any of this if it werent for him, so...
	// yeah...
	VectorVictor::Vector2 Position;
	// Where we are
	VectorVictor::Vector2 Velocity;
	// Where we are going
	VectorVictor::Vector2 Acceleration;	
	// where we are going is going
	// another shitter to get rid of. Need to look up all references	// maybe
	
	// this isnt strictly necessary as an object variable, although it makes
	// the code a wee bit simpler just leaving it in each frame
	bool Crashed;
	// the current type of collision detection is quite simple, if we hit the
	// ground at all, we crashed, and our position is locked to the point
	// on the surface where we bought the farm
	
	// very much temporary, but wouldnt hurt to fix this up so that it works
	// properly in following the surface position on the body where it crashed
	// right now it just sits on the surface of the sphere as if it were
	// frictionless
	bool Crash_state(long double sim_time, std::vector<CKeplerian_Object*> &ignition_celestials);
	// if we aint crashed, check if our position implies that we are
	// auch, this is why the setup above was so ugly, shouldnt need to have
	// simtime to check this
	double Theta, Omega, Alpha;
	// Our orientation in space and how its changing
	// both stored in degrees
	double Get_omega();
	double Get_theta_in_degrees();		
	double Get_theta_in_radians();
	// returns for the data above
	double Length, PMI;
	// I think the length is outdated now, since hull handles that.
	// PMI is another good physical property, the total moment of inertia of the
	// vessel around whatever our reference axis (center of mass) is
	double Get_length();
	// important property for graphics stuff. Handled by hull object nowadays
	double Get_hull_mass();
	// no trick questions here, exactly what it sounds like
	virtual double Get_total_mass();
	// and again...
	virtual double Get_PMI();
	// and again.......		
	std::vector<Resource_Tank*> Fuel_tanks;
	// any components that have a resource with mass
	// should also have a list of thrusters here, so tanks get mapped to
	// thrusters at runtime
	std::vector<Vessel_component*> Object_components;
	// literally any component on the vessel that is of the vessel component 
	// type
	void Add_force(double attack_point_x, double attack_point_y, double force_x, double force_y);
	// this... um, might be worthless
	
	// should watch to see if this should just be removed to avoid bloat
	void Add_force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force_vector);
	// again, never used this yet
	std::vector<Force> Force_list;
	// All forces acting on the vessel in the current frame. This list gets wiped
	// each frame after being used to determine net force and torque for that
	// frame	
	void Frame(long double dt, long double simtime, std::vector<CKeplerian_Object*> &ignition_celestials);
	// The wonderful grand update that handles updating everything in the object
	// parts, vessel status, etc.
	void Update_motion(long double dt);
	// State propagator of the cartesian coordinates of the vessel
	// currently single pass linear, (ie bad) but will be updated
	void Update_rotation(long double dt);
	// Same as for motion, just using the torques implied by the Force list
	// in each frame
	
	bool Update_flag;
	// this was an older idea
	bool Update_flag_state();
	// I think this was originally intended to allow for only updating the
	// inertia when a vessel component signaled it. It could still be
	// implemented
	void Update_PMI();	
	// queries all vessel components for their respective moments of inertia
	// and adds them up. Updates are required over time to account for changes
	// in mass inside the vessels parts
	
	virtual void Print_data();	
	// A handy function for debugging, basically just a console printout of
	// whatever is relevant when working on the thing
	virtual void Receive_inputs(key_commands * current_inputs, double dt);	
	// we pass the vessel the object with data on all keypresses in the last
	// frame, and each implementation class of the Newtonian object handles
	// it in its own way
	
	virtual void Receive_cursor_inputs(Cursor_commands * cursor_action, long double dt);
	// very similarly to Receive_inputs, except we send it info about the mouse
	// cursor and this applies to all vessels in the window
	
	// super-duper important thing to remember about this:
	// its called for any vessels in view after they get drawn in the main
	// ignition loop, so it essentially is happening at the end of the frame,
	// and its effects will only be felt next frame
	// this saves some speed by piggybacking on all of the in view calls made
	// by the main loop, but results in a bit of an odd structure, since in a
	// perfect world this would be done after frame(dt) but before everything
	// gets drawn onscreen
	
	// this should be partially moved around so that some actions are standard
	// like rot/trans inputs should be handled from specific keys and should
	// cause similar effects
	virtual bool In_view(SFML_Window * window, int zoom_factor);	
	// virtual for reasons I dont recall, maybe asteroids will handle this in
	// some unorthodox way
	sf::Sprite * Object_sprite;	
	// this is fine for now, but I really would prefer shifting it to the
	// vessel components (specifically the hull in this case)
	
	double pix_length;
	
	sf::Sprite * Flag_sprite;	
	// the little icon that gets drawn in the map view.
	// A couple of things should change for this I think, specifically the flag
	// will behave as a dynamic popup when the user mouses over it
	// (only for non-selected vessels) and the current vessel always has it on
	// right now its kinda off, since the sprite is drawing from the top left
	// corner of the image, which caused confusion before
	
	
	void Draw_flag(SFML_Window * iwindow, int zoom_factor);
	// same stuff as before
	
	std::string Object_name;	
	// again, same idea as the one in CKeplerian
	// this might also be nice as an inherited
	// "named object" type. I dunno. Probably not worth it
	
	CNewtonian_Object* Get_Newtonian_pointer();
	// Abstract way of referencing the object at the Newtonian level
};

//std::vector<CNewtonian_Object*> Newtonian_list;
// not even sure how I am getting away with this as it is
// really should be local to the Ignition Engine object

// TVessel Class ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class TVessel: public CNewtonian_Object
{	public:
	// More specific type than Newtonian, deals with things that a vessel really
	// must have, like a hull & standard ways of dealing with vesselly objects
	Hull * Hull_component;
	// the hull, of which there should only ever be one
	// I cant imagine why a second one of these should ever be needed
	
	void Init_vessel_type();
	// it doesnt now, but this must need arguments in the future
	// how could it possibly not?
	
	// I mean like things like position and whatnot
	// although the init will probably
	
	virtual void Rotate_left(double dt);
	virtual void Rotate_right(double dt);
	virtual void Kill_rotation(double dt);					
	// this seems at first to conflict with rot left/right, but since input
	// commands come in one by one, they shouldnt. I hope
	virtual void Translate_forward(double dt);
	virtual void Translate_backward(double dt);
	virtual void Translate_left(double dt);
	virtual void Translate_right(double dt);
	virtual void Throttle_up(double dt);
	virtual void Throttle_down(double dt);
	virtual void No_command(double dt);
	// basically, if Receive_inputs, or any other function decides that a
	// particular control action needs to be run, one (or more?) of these
	// methods gets called to make it happen
	
	// I think this is structured wrong, should actually have a list of
	// thrusters to engage for a particular control action, and the global one
	// gets throttled down
	
	// if the thrusters in the action list are throttled up by double the actual
	// amount, that should work
	std::vector<Thruster*> Thrusters;
	// the global list containing every thruster on the craft
	bool Init_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_id);
	// we get all of the parameters needed to make a new object of type, and we
	// pass it the id type of which thruster group it will be placed in
	bool Init_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_id1, thruster_group group_id2);
	// and we do the same thing, except now with two groups
	bool Init_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_id1, thruster_group group_id2, thruster_group group_id3);
	// and with three groups. Seriously, how can we possibly need more than 3
	// groups to a thruster?
	
	// the three previous calls are implicitly assumed to be monopropellant
	// thrusters based on the number and type of arguments supplied to the
	// function
	
	long double Get_max_alpha(rotation_direction direction);
	
	sf::Texture * Vessel_tex;
	// pointer to the texture of the vessels hull. Pointer is kind of nice here
	// so we dont need to make a copy of the same texture every time a
	// vessel of the same texture gets created (which might happen a lot)
	
	bool In_view(SFML_Window * window, int zoom_factor);
	// This is implemented at this level
	// (I think because we finally have access to the hull & its length)
	void Draw_flag(SFML_Window * window, int zoom_factor);	
	// also implemented here, although no particular reason that I see

	void Draw_vessel(SFML_Window * iwindow, double cam_scale);												
	// this should eventually become a vertex array with the thruster visuals 
	// thrown in alongside it
	virtual void Draw_controls(SFML_Window * iwindow, bool Map_status);
	// the standard function for any vessel that allows the user to define
	// a very specific display (ie maybe an Apollo AGC display for a LEM?)
	// in SFML types, then draw it onscreen each frame.
	

	
	// preferably, I think this can be moved upstairs to the Newtonian
	// and renamed Draw_displays
	// no reason why it cant be applied to asteroids & somesuch
	// isnt really that much else besides asteroids thats a Newtonian but
	// not a TVessel... 
	std::string Vessel_class;
	// Just the name of the vessel class, ie "Delta-Glider"
	// could also be other distinctions, like vessel - Delta Glider
	// as opposed to asteroid
	std::string Get_vessel_class();
	std::string Get_vessel_name();
	// returns for the above string, as well as the object name defined in 
	// Newtonian
	
	TVessel* Get_Vessel_pointer();
	// the access at this abstraction layer
	// this could always be tossed around by passing *pointer by reference
	// in functions I guess
};



class DeltaGlider: public TVessel
{	public:
	// the first class of this type written for testing purposes
	// a homage to Orbiter, without which this project never would have happened
	// Hail the Probe!!!
	DeltaGlider(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, sf::Texture * status_texture, sf::Font * controls_font, sf::Texture * panel_texture1);
	// Only constructor at the moment, will eventually need one that works for
	// reloading sims from a scenario file
	double k_throttle;
	// the rate at which the throttle moves around in (throttle units/ second)
	// throttle only has a range of 1, so values need to be between that and
	// zero (generally quite small, although it depends on the specific case)
	sf::Text * main_fuel_level, * rcs_fuel_level, * omega_value, * theta_value, * position_values, * velocity_values, * vessel_id;
	// All of the text displays for important information about the vessel while
	// in flight.
	
	// This all needs to be abstracted if possible
	sf::Sprite * vessel_display, * display_panel;
	// the semi transparent image of the ship, and the background to the
	// displays panel, respectively. The vessel_display is really just eyecandy
	// for the moment. Does look nice though
	bool Throttle_lock;
	// on/off switch that holds the main throttle open at its current value
	// when true, instead of sliding back to zero like it normally does
	// this behaviour is specific to Ignition Engine, Orbiter and KSP hold their
	// throttles constant if untouched. Might be a decent candidate to make
	// an optional feature
	void Receive_inputs(key_commands * current_inputs, double dt);
	void Receive_cursor_inputs(Cursor_commands * cursor_action, long double dt);
	// finally, specific implementation of how the class type handles input
	// from the user
	double Get_total_mass();
	double Get_PMI();			
	// pretty much what it says on the box
	
	//void Drain_fuel(double dt);
	// hmm, odd... no clue why this existed in the first place, not very useful
	// to spring leaks in your fuel tanks
	
	// this should probably be removed
	void Print_data();
	// Implementation of the handy debug feature from before
	void Toggle_throttle_lock();
	void Draw_controls(SFML_Window * iwindow, bool Map_status);	
	// pretty much what they sound like
	// Draw controls should become Draw displays if the change I mentioned
	// above with the instance specific readouts goes into effect
	
	
	
	void Rotate_left(double dt);
	void Rotate_right(double dt);
	
	void Rotate_left(double dt, double throttle_target);
	void Rotate_right(double dt, double throttle_target);
	
	void Translate_forward(double dt);
	void Translate_backward(double dt);
	void Translate_left(double dt);
	void Translate_right(double dt);
	void Throttle_up(double dt);
	void Throttle_down(double dt);	
	void No_command(double dt);
	void Kill_rotation(double dt);
	// implementations of what gets executed when the commands are called
	
	// this is all about to bite the dust
	
	Resource_Tank * Main_fuel, * RCS_fuel;
	// Fuel tanks, with no specific connection to anything above this class
	sf::Color * text_colour;
	// doesnt really need to be only one, but if its working, dont touch
	// abstraction will remove this anyways
	~DeltaGlider();
};

#endif

