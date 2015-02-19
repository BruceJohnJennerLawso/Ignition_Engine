// Deltaglider.hpp /////////////////////////////////////////////////////////////
// A testbed to get started with ignition, based off of ////////////////////////
// the one and only DG, from Orbiter ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
//#include "Vessel.hpp"
#include "mfd.hpp"

#ifndef DeeeGEEEE
#define DeeeGEEEE
// I dont know who you are, but if you try to use this as a variable name,
// I will find you and I will kill you

// the world already has enough poorly named variables as it is

class DeltaGlider: public TVessel
{	public:
	// the first class of this type written for testing purposes
	// a homage to Orbiter, without which this project never would have happened
	// Hail the Probe!!!
	DeltaGlider(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Texture vessel_textures, sf::Sprite flag_sprite, std::string ivessel_name, std::string panel_path, std::string controls_font, Propagator_type propagator);
	// Only constructor at the moment, will eventually need one that works for
	// reloading sims from a scenario file
	DeltaGlider(ObjectState initial_object_state, double initial_main_propellant, double initial_rcs_propellant, sf::Texture vessel_textures, sf::Sprite flag_sprite, std::string ivessel_name, std::string panel_path, std::string controls_font, Propagator_type propagator);	
	
	// the rate at which the throttle moves around in (throttle units/ second)
	// throttle only has a range of 1, so values need to be between that and
	// zero (generally quite small, although it depends on the specific case)
	Ignition_text main_fuel_level, rcs_fuel_level, omega_value, theta_value, position_values, velocity_values, vessel_id;
	// All of the text displays for important information about the vessel while
	// in flight.
	
	// This all needs to be abstracted if possible
	Ignition_sprite vessel_display, display_panel;
	
	//sf::Sprite * vessel_display, * display_panel;
	
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
	long double Get_PMI();			
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
	
	Resource_Tank * Main_fuel, * RCS_fuel;
	// Fuel tanks, with no specific connection to anything above this class
	sf::Color * text_colour;
	// doesnt really need to be only one, but if its working, dont touch
	// abstraction will remove this anyways
	
	// ughh this is just awful
	
	void Render_MFDs();
	~DeltaGlider();
};

TVessel * Construct_deltaglider(ObjectState initial_object_state, double initial_main_propellant, double initial_rcs_propellant,  sf::Texture vessel_textures, sf::Sprite flag_sprite, std::string ivessel_name, std::string panel_path, std::string controls_font, Propagator_type propagator);
// this should be stripped back parameter wise until it fits some kind of
// format thats agnostic to the exact type of the vessel being created, and then
// the specific variables like tank fullness can be grabbed by whatever function
// is loading from the scenario file

// this is so that we can just figure out what kind of vessel we need to make
// from the scenario file and then just make it using the right function pointer
// which has equivalent arguments to the above function

bool Create_deltaglider(std::vector<TVessel*> &Vessels_list, std::vector<CNewtonian_Object*> &Newtonian_list, TVessel* New_dg_pointer); 
#endif

