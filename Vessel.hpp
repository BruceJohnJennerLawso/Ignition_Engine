// Vessel.hpp //////////////////////////////////////////////////////////////////
// The TVessel type used for vessels ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Newtonian_objects.hpp"

#ifndef vESSEL
#define vESSEL

// TVessel Class ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class TVessel: virtual public CNewtonian_Object
{	// made virtual so that implementation classes can use CNewtonians
	// constuctor for nice clean code
	public:
	// More specific type than Newtonian, deals with things that a vessel really
	// must have, like a hull & standard ways of dealing with vesselly objects
	Hull * Hull_component;
	// the hull, of which there should only ever be one
	// I cant imagine why a second one of these should ever be needed
	
	
	double RCS_Throttle_constant, Main_throttle_constant;
	
	void Rotate_left(double dt);
	void Rotate_right(double dt);
	
	void Rotate_left(double dt, double throttle_target);
	void Rotate_right(double dt, double throttle_target);
	
	void Kill_rotation(double dt);					
	// this seems at first to conflict with rot left/right, but since input
	// commands come in one by one, they shouldnt. I hope
	void Translate_forward(double dt);
	void Translate_backward(double dt);
	void Translate_left(double dt);
	void Translate_right(double dt);
	void Throttle_up(double dt);
	void Throttle_down(double dt);
	void No_command(double dt);
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
	
	// wait wat
	// noooooooooooooooooooooooooooooooooooooooooooooooooooooooo
	// not want
	
	bool In_view(SFML_Window * window, int zoom_factor);
	// This is implemented at this level
	// (I think because we finally have access to the hull & its length)
	bool In_view(SFML_Window * window, long double cam_scale);
	
	void Draw_flag(SFML_Window * window, int zoom_factor);	
	// also implemented here, although no particular reason that I see

	void Draw_vessel(SFML_Window * iwindow, long double cam_scale);												
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

#endif
