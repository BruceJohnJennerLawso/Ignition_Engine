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
#include "Newtonian_objects.hpp"


#ifndef DeeeGEEEE
#define DeeeGEEEE


class DeltaGlider: public TVessel
{	public:
	// the first class of this type written for testing purposes
	// a homage to Orbiter, without which this project never would have happened
	// Hail the Probe!!!
	DeltaGlider(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, sf::Texture * status_texture, sf::Font * controls_font, sf::Texture * panel_texture1, Propagator_type propagator);
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
	
	// ughh this is just awful
	~DeltaGlider();
};

#endif

