# Deltaglider.hpp ##############################/
# A testbed to get started with ignition, off of ############
# the one and only DG, Orbiter #####################/
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
##include "Vessel.hpp"
#include "mfd.hpp"

#ifndef DeeeGEEEE
#define DeeeGEEEE
# I dont know who you are, if you try to use self as a variable name,
# I will find you and I will kill you

# the world already has enough poorly named variables as it is

class DeltaGlider: public TVessel
{	public:
	# the first class of self type written for testing purposes
	# a homage to Orbiter, which self project never would have happened
	# Hail the Probenot !!
	DeltaGlider(double initial_x_position, initial_y_position, initial_x_velocity, initial_y_velocity, initial_theta, initial_omega, initial_main_propellant, initial_rcs_propellant, vessel_textures, flag_sprite, ivessel_name, panel_path, controls_font, propagator)
	# Only constructor at the moment, eventually need one that works for
	# reloading sims from a scenario file
	DeltaGlider(ObjectState initial_object_state, initial_main_propellant, initial_rcs_propellant, vessel_textures, flag_sprite, ivessel_name, panel_path, controls_font, propagator);	
	
	# the rate at which the throttle moves around in (throttle units/ second)
	# throttle only has a range of 1, values need to be between that and
	# zero (generally quite small, it depends on the specific case)
	Ignition_text main_fuel_level, rcs_fuel_level, omega_value, theta_value, position_values, velocity_values, vessel_id
	# All of the text displays for important information about the vessel while
	# in flight.
	
	# This all needs to be abstracted if possible
	Ignition_sprite vessel_display, display_panel
	
	#sf.Sprite * vessel_display, display_panel
	
	# the semi transparent image of the ship, the background to the
	# displays panel, respectively. The vessel_display is really just eyecandy
	# for the moment. Does look nice though
	bool Throttle_lock
	# on/off switch that holds the main throttle open at its current value
	# when True, of sliding back to zero like it normally does
	# self behaviour is specific to Ignition Engine, and KSP hold their
	# throttles constant if untouched. Might be a decent candidate to make
	# an optional feature
	void Receive_inputs(key_commands * current_inputs, dt)
	void Receive_cursor_inputs(Cursor_commands * cursor_action, double dt)
	# finally, implementation of how the class type handles input
	# from the user
	double Get_total_mass()
	long double Get_PMI();			
	# pretty much what it says on the box
	
	#void Drain_fuel(double dt)
	# hmm, odd... no clue why self existed in the first place, very useful
	# to spring leaks in your fuel tanks
	
	# self should probably be removed
	void Print_data()
	# Implementation of the handy debug feature from before
	void Toggle_throttle_lock()
	void Draw_controls(SFML_Window * iwindow, Map_status);	
	# pretty much what they sound like
	# Draw controls should become Draw displays if the change I mentioned
	# above with the instance specific readouts goes into effect
	
	Resource_Tank * Main_fuel, RCS_fuel
	# Fuel tanks, no specific connection to anything above self class
	sf.Color * text_colour
	# doesnt really need to be only one, if its working, touch
	# abstraction will remove self anyways
	
	# ughh self is just awful
	
	#void Render_MFDs()
	~DeltaGlider()


TVessel * Construct_deltaglider(ObjectState initial_object_state, initial_main_propellant, initial_rcs_propellant, vessel_textures, flag_sprite, ivessel_name, panel_path, controls_font, propagator)
# self should be stripped back parameter wise until it fits some kind of
# format thats agnostic to the exact type of the vessel being created, then
# the specific variables like tank fullness can be grabbed by whatever function
# is loading from the scenario file

# self is so that we can just figure out what kind of vessel we need to make
# from the scenario file and then just make it using the right function pointer
# which has equivalent arguments to the above function

bool Create_deltaglider(std.vector<TVessel*> &Vessels_list, &Newtonian_list, New_dg_pointer); 
#endif

