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
// all of your physics are belong to Newton

// Newtonian Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

enum Propagator_type{Euler1, RK4};
// thats all I can think of just yet...

enum Object_status{Flight, Landed, Crashed};
// pretty simple, just are we in flight or on the ground,
// and if we are on the ground, will we be able to take off again, or is the 
//ship really just a metal smear across the ground somewhere

class Rotation_state
{	public:
	Rotation_state();
	Rotation_state(long double theta, long double omega, long double alpha);
	Rotation_state operator = (const Rotation_state r);	
	long double Theta, Omega, Alpha;
	~Rotation_state();
};

class Flight_state
{	public:
	Flight_state();
	Flight_state(VectorVictor::Vector2 initial_position, VectorVictor::Vector2 initial_velocity);
	bool Init_flight_state(VectorVictor::Vector2 initial_position, VectorVictor::Vector2 initial_velocity);
	Flight_state operator = (const Flight_state s);
	VectorVictor::Vector2 Position;
	VectorVictor::Vector2 Velocity;
	~Flight_state();
};

class Surface_state
{	public:
	Surface_state();
	Surface_state(std::string planet_name, long double longitude);
	Surface_state operator = (const Surface_state s);
	std::string Planet_name;
	// ehh.. I forget wat the plan was here
	long double Longitude;
	~Surface_state();
};

class ObjectState
{	public:
	ObjectState();
	ObjectState(Flight_state initial_flight_state, Rotation_state rotation);
	// implicitly in flight
	ObjectState(Surface_state initial_landed_state, Rotation_state rotation);
	// implicitly uhh landed, and if we need to start off crashed, we can call
	// a set call to the Object_status
	ObjectState operator = (const ObjectState o);
	
	Flight_state FlightState;
	Surface_state LandedState;
	
	Object_status Current_state;
	void Set_status(Object_status new_status);
	// the indication of what our current state is, are we crashed, landed or
	// in flight. This in turn is used to decide which of the states is used
	// from above, either the flightstate or the landedstate, since each state
	// stores different information
	Rotation_state Rotation;
	~ObjectState();
};




class CNewtonian_Object
{	public:
	// Nature and nature's laws lay hid in night;
	//	God said "Let Newton be" and all was light.
	
	// rather true
	// and I certainly wouldnt be doing any of this if it werent for him, so...
	// yeah...
	CNewtonian_Object();
	CNewtonian_Object(ObjectState initial_newtonian_state);
	
	ObjectState PreviousState;
	// I know, i know, inconsistency, so shoot me...
	
	// so this state stores whatever our vessel looks like at the top of the
	// frame, that way we can compare and contrast at the end of the frame
	// & use it to do physics stuffz
	
	// but we arent using it just yet...
	
	ObjectState NewtonianState;
	VectorVictor::Vector2 Acceleration;	
	
	// a cursory check shows that this definitely could be changed so that the
	// functions using it now just use their own VV2s
	
	bool Collision_checked;
	// a quick value that gets flipped once the vessel in question has handled
	// its own collision detection. This prevents vessel A from colliding with
	// vessel B, and then Vessel B colliding a second time with A and so on
	
	// hmmm, this is trickier than I thought
	
	// cause it branches out with each vessel that is being collided with
	
	
	
	
	// this isnt strictly necessary as an object variable, although it makes
	// the code a wee bit simpler just leaving it in each frame
	Propagator_type Propagator;
	// so this is just a nice little enum type that we can use here to indicate
	// what kind of propagator the newtonian object is using in its quest to
	// obey Newtons laws
	
	// The sim can switch this back and forth depending on whatever the hell
	// it feels like doing with the newtonian object at that particular point
	// in time (orbit stabilization and the like?)
	long double Length, PMI;
	// I think the length is outdated now, since hull handles that.
	// PMI is another good physical property, the total moment of inertia of the
	// vessel around whatever our reference axis (center of mass) is
	bool Crash_state(long double sim_time, std::vector<CKeplerian_Object*> &ignition_celestials);
	// if we aint crashed, check if our position implies that we are
	// auch, this is why the setup above was so ugly, shouldnt need to have
	// simtime to check this
	double Get_omega();
	double Get_theta_in_degrees();		
	double Get_theta_in_radians();
	// returns for the data above
	long double Get_length();
	// important property for graphics stuff. Handled by hull object nowadays
	double Get_hull_mass();
	// no trick questions here, exactly what it sounds like
	virtual double Get_total_mass();
	// and again...
	virtual long double Get_PMI();
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
	void Update_motion(long double simtime, long double dt, std::vector<CKeplerian_Object*> &ignition_celestials);
	// State propagator of the cartesian coordinates of the vessel
	// currently single pass linear, (ie bad) but will be updated
	void Propagate_Euler1(long double sim_time, long double dt, VectorVictor::Vector2 &net_force, std::vector<CKeplerian_Object*> &ignition_celestials);
	// brute force it. Dont even know where this would be useful anymore
	// once the RK4 is implemented, but I guess it cant hurt
	void Propagate_RK4(long double sim_time, long double dt, VectorVictor::Vector2 &net_force, std::vector<CKeplerian_Object*> &ignition_celestials);
	// run a step of length dt with an RK4 propagator for much better accuracy
	// than an implicit Euler one will give
	Flight_state evaluate(const Flight_state &initial_state, long double simtime, long double dt, const Flight_state &derivative, std::vector<CKeplerian_Object*> &ignition_celestials, VectorVictor::Vector2 &net_force);
	// It doesnt absolutely need to be a member method, but this makes it much easier
	void Update_rotation(long double simtime, long double dt);
	// Same as for motion, just using the torques implied by the Force list
	// in each frame
	void Collision_detect(std::vector<CNewtonian_Object*> &collidable_objects);
	
	
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
	virtual bool In_view(SFML_Window * window, long double cam_scale);
	// aww yeah	
	sf::Sprite * Object_sprite;	
	// this is fine for now, but I really would prefer shifting it to the
	// vessel components (specifically the hull in this case)
	
	double pix_length;
	// this needs documenting
	
	sf::Sprite Flag_sprite;	
	// the little icon that gets drawn in the map view.
	// A couple of things should change for this I think, specifically the flag
	// will behave as a dynamic popup when the user mouses over it
	// (only for non-selected vessels) and the current vessel always has it on
	// right now its kinda off, since the sprite is drawing from the top left
	// corner of the image, which caused confusion before
	
	void Draw_flag(SFML_Window * iwindow, int zoom_factor);
	// the draw call used for map view	
	std::string Object_name;	

	CNewtonian_Object* Get_Newtonian_pointer();
	// Abstract way of referencing the object at the Newtonian level
};

#endif

