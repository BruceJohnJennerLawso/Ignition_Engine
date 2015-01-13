// Game_Objects.h //////////////////////////////////////////////////////////////
// All of the core classes that the ignition engine requires to function, //////
// including Newtonian and Keplerian parent classes for all celestial bodies ///
// and vessels /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Vessel_components.hpp"

#ifndef NewtonsObjects
#define NewtonsObjects

// Newtonian Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

enum Propagator_type{Euler1, RK4};
// thats all I can think of just yet...

class CNewtonian_Object
{	public:
	// Nature and nature's laws lay hid in night;
	//	God said "Let Newton be" and all was light.
	
	// rather true
	// and I certainly wouldnt be doing any of this if it werent for him, so...
	// yeah...
	
	
	// this whole pos/vel/accel thing can be unionized with some sort of
	// identifier for a landed/crashed (surface?) state
	
	VectorVictor::Vector2 Position;
	// Where we are
	VectorVictor::Vector2 Velocity;
	// Where we are going
	VectorVictor::Vector2 Acceleration;	
	// where we are going is going
	// another shitter to get rid of. Need to look up all references
	// on the other hand this is somewhat useful...
	
	// but this is going to become a bit muddy once the RK4 propagator is
	// online
	
	// this isnt strictly necessary as an object variable, although it makes
	// the code a wee bit simpler just leaving it in each frame
	
	Propagator_type Propagator;
	// okedely doke
	
	// so this is just a nice little enum type that we can use here to indicate
	// what kind of propagator the newtonian object is using in its quest to
	// obey Newtons laws
	
	// The sim can switch this back and forth depending on whatever the hell
	// it feels like doing with the newtonian object at that particular point
	// in time (orbit stabilization and the like?)
		
	double Length, PMI;
	// I think the length is outdated now, since hull handles that.
	// PMI is another good physical property, the total moment of inertia of the
	// vessel around whatever our reference axis (center of mass) is
	
	long double Theta, Omega, Alpha;
	// Our orientation in space and how its changing
	// all stored in degrees
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
	double Get_omega();
	double Get_theta_in_degrees();		
	double Get_theta_in_radians();
	// returns for the data above
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
	
	// this maybe... hmm. This could be a bad idea...
	// I think it should be moved back to the top of the loop so the inputs
	// are received in this frame
	
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

