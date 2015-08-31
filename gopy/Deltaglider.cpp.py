# Deltaglider.cpp ##############################/
# A testbed for ignition, off of ####################
# the one and only DG, Orbiter #####################/
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include "Deltaglider.hpp"


# Delta Glider Class #############################
########################################
########################################


DeltaGlider.DeltaGlider(double initial_x_position, initial_y_position, initial_x_velocity, initial_y_velocity, initial_theta, initial_omega, initial_main_propellant, initial_rcs_propellant, vessel_textures, flag_sprite, ivessel_name, panel_path, controls_font, propagator)
{	Talkback("Constructing Delta Glider")
	# we write to the console for feedback while debugging
	NewtonianState.Current_state = Flight
	Propagator = propagator
	
	Start_MFD(surface_mfd, upper_right, sf.Vector2f(20, 60))
	
	Flag_sprite = flag_sprite;	
	# set our flag affilation here
	# self looks wrongish, think self needs to be copied by value, definitely
	# not reference, the same flag may need to be drawn several times per
	# frame
	Flag_sprite.setScale(0.1,0.1)
	# scales it back by 1/10 along each axis, the orginal was too big
	# self is baddish
	Object_name = ivessel_name
	Vessel_class = "Delta Glider"
	# self should rather be frozen so it cant be changed in sim, for
	# safety/not chaos
	Acceleration.Set_values(0, 0)
	# just an call to be safe
	NewtonianState.FlightState.Position.Set_values(initial_x_position, initial_y_position)
	NewtonianState.FlightState.Velocity.Set_values(initial_x_velocity, initial_y_velocity)
	
	self.NewtonianState.Rotation.Theta = initial_theta; 
	self.NewtonianState.Rotation.Omega = initial_omega; 
	self.NewtonianState.Rotation.Alpha = 0
	# we take what we were passed and set our properties to it
	# important that self gets set explicitly, it the compiler just 
	# sets it to random values for some godawful reason
	
	VectorVictor.Vector2 origin(0,0)
	Hull_component = Hull(16000, 0.4, 17.5, origin)
	Object_components.insert(Object_components.end(), Hull_component)
	# we construct the hull component and insert it into the general component
	# list. The vessel can access it by either the Hull component reference
	# that it stores internally, through its general version in the parts
	# list
			
	# the pointer passin crap didnt work ^^
	# ^ not sure exactly what that was referring to
	Main_fuel = Resource_Tank(initial_main_propellant, 800, 0.6, 0.8, 2.2, origin)
	Fuel_tanks.insert(Fuel_tanks.end(), Main_fuel);	
	Object_components.insert(Object_components.end(), Main_fuel.Get_vessel_component_pointer())
	# construct the main fuel tank and insert it into both the fuel tank list
	# and the general parts list.
	# Come to think of it, exactly is there a separate list for fuel tanks?
	# they arent even accessed that way, yet anyways
	
	Init_thruster(False, 4000, 50000, 290, 0, -6.2000, 0, -1, 0.4, 0.49, Main_fuel, main_engines); 
	# construct the main engine thrsuter, it into the thruster vector
	# and the general components vector
	
	# self is at least necessary, the main thruster vector is used for
	# throttling back all engines each frame unless the throttle up is doing
	# the opposite
	
	VectorVictor.Vector2 rcs_pos(0, 2)
	RCS_fuel = Resource_Tank(initial_rcs_propellant, 500, 0.6, 0.64, 1,rcs_pos)
	Fuel_tanks.insert(Fuel_tanks.end(), RCS_fuel);		
	Object_components.insert(Object_components.end(), RCS_fuel.Get_vessel_component_pointer());	
	
	Init_thruster(True, 470, 49000, 61, 0, 6.2000, -1, 0, 0.10, 0.12, RCS_fuel, rotate_clockwise, translate_right)
	Init_thruster(True, 470, 49000, 61, 0, 6.2000, 1, 0, 0.10, 0.12, RCS_fuel, rotate_counterclockwise, translate_left)
	Init_thruster(True, 470, 49000, 46, 0, 6.400, 0, 1, 0.10, 0.12, RCS_fuel, translate_back); 
	Init_thruster(True, 470, 49000, 63, -0.20, -6.0000, -1, 0, 0.10, 0.12, RCS_fuel, rotate_counterclockwise, translate_right); 
	Init_thruster(True, 470, 49000, 63, -0.20, -6.0000, 1, 0, 0.10, 0.12, RCS_fuel, rotate_clockwise, translate_left); 
	Init_thruster(True, 470, 49000, 46, -0.2, -6.0000, 0, -1, 0.10, 0.12, RCS_fuel, translate_forward)
	# and we construct all of the RCS thrusters in the same way
	
	std.cout << Object_components.size() << " Vessel components" << std.endl
	# not sure why, good to have the number of components on the vessel
	# for debugging
	
	RCS_Throttle_constant = 29.9000
	# we set self up so that the thruster goes from 0 to 2990 % in one second
	# obviously that doesnt work, it really represents 0 to 100% in 1/29.9 s
	# (0.033 s, 2 frames if fps is a healthy 60)
	
	Main_throttle_constant = 0.25
	# significantly slower for the main engines, they take a full four
	# seconds to go from 0 - 100
	
	
	Throttle_lock = False
	# Throttle lock keeps the main engines at their current throttle setting
	# instead of trying to slide back to zero each frame, is useful
	# during long burns
	
	Update_PMI()
	# once all of the vessel components are in, should be run to get the
	# initial value
	# we create it and assign it the texture passed by the constructor
	# (less memory waste per vessel than each one loading a copy of the texture
	# for every instance
	
	Talkback("Setting up texture stuff")
	Vessel_tex = sf.Texture()
	*Vessel_tex = vessel_textures
	Talkback("Copied texture")
	# not sure why we need to copy the reference to self...
	
	Object_sprite = sf.Sprite()
	Talkback("Init the sprite")
	Object_sprite.setTexture(*Vessel_tex)
	
	Talkback("Finished textures stuff")
	
	# self appears to be redundant, a good candidate to remove
	text_colour = sf.Color(194, 38, 15)
	# nice red colour for all of the vessel displays
	
	vessel_id.Init_object(controls_font, sf.Vector2f(20, 380), "-", *text_colour, 16, False)
	main_fuel_level.Init_object(controls_font, sf.Vector2f(20, 420), "-", *text_colour, 14, False)
	rcs_fuel_level.Init_object(controls_font, sf.Vector2f(20, 440), "-", *text_colour, 14, False);	
	omega_value.Init_object(controls_font, sf.Vector2f(20, 460), "-", *text_colour, 14, False);		
	theta_value.Init_object(controls_font, sf.Vector2f(20, 480), "-", *text_colour, 14, False)
	position_values.Init_object(controls_font, sf.Vector2f(20, 500), "-", *text_colour, 14, False);	
	velocity_values.Init_object(controls_font, sf.Vector2f(20, 520), "-", *text_colour, 14, False);		
	vessel_display.Init_object(sf.Vector2f(820, 460), sf.Color(255, 255, 255, 168), False, *Vessel_tex, True)
	vessel_display.sprite.setScale(sf.Vector2f(0.5f, 0.5f))
	# Ill get around to it later
	
	# the pretty semi-transparent image of the ship that appears at bottom
	# right in the screen. Really just eye candy for now, I have plans for
	# it later on. Its vessel implementation-specific anywayss
	
	display_panel.Init_object(sf.Vector2f(5, 360), sf.Color(255, 255, 255), False, panel_path)
	# Ill get around to it later
	
	# display_panel = sf.Sprite()
	# display_panel.setTexture(*panel_texture1)
	# display_panel.setPosition(sf.Vector2f(5, 360));	
	# the metal looking thing at bottom right. Only there because the onscreen
	# displays proved hard to read against the background
	
	pix_length = Hull_component.Length*10
	# get the length of the vessels long axis in meters
	# the multiple of ten thing is fine, the current window size was
	# accidentally made 10x too large, vessels appeared 10x larger than
	# they really were
	
	# not terribly important, self will change when the window view
	# has multiple scales to be a lot like the one for planets, it is
	# important to remember that the base view 1m = 10 pixels, reasons
	# of easiest scaling
	if Vessel_tex.getSize().y != pix_length:
	{	Object_sprite.setScale((pix_length/(Vessel_tex.getSize().y)),(pix_length/(Vessel_tex.getSize().y)))
		# rescale the axes of the texture to match pix_length in the y and the
		# appropriate scale for the x dimension
		
		# not sure why they both use y, I think self was distorted when
		# it used x & y.
		# This area needs to be looked over again
	}	
	Object_sprite.setOrigin((Vessel_tex.getSize().x/2), (Vessel_tex.getSize().y/2))
	# ok the sprite definitely gets centered there...
	Object_sprite.setRotation(self.NewtonianState.Rotation.Theta)
	# and we center the origin and rotate it appropriately
	Talkback("...Constructed Delta Glider")
	# feed back that the constructor is now finished to the console
}	


DeltaGlider.DeltaGlider(ObjectState initial_object_state, initial_main_propellant, initial_rcs_propellant, vessel_textures, flag_sprite, ivessel_name, panel_path, controls_font, propagator): CNewtonian_Object(initial_object_state)
{	# the & improved constructor using the call to CNewtonians constructor
	# so hopefully a fair bit shorter
	# although we still have a long way to go in self area till the whole
	# system is really ready to go. In fact, current setup probably wont
	# work with dynamic binding at all just yet, to only take CNewtonian
	# parameters to get set up
	
	# simplest way to get there would probably be to half implement scenario
	# loading from file first so that the constructor is clean for calling in
	# a dll callback without all of self type specific stuff
	
	# the individual sprites and textures should probably be loaded internally
	# to each object. The whole idea of sharing references between all of the
	# objects was cute, it just wont work with the way generic vessels will
	# need to work
	Talkback("Constructing Delta Glider")
	# we write to the console for feedback while debugging
	Propagator = propagator
	
	Start_MFD(surface_mfd, upper_right, sf.Vector2f(20, 60))
	
	Flag_sprite = flag_sprite;			
	# set our flag affilation here
	# self looks wrongish, think self needs to be copied by value, definitely
	# not reference, the same flag may need to be drawn several times per
	# frame
	Flag_sprite.setScale(0.1,0.1)
	# scales it back by 1/10 along each axis, the orginal was too big
	# self is baddish
	Object_name = ivessel_name
	Vessel_class = "Delta Glider"
	# self should rather be frozen so it cant be changed in sim, for
	# safety/not chaos
	
	VectorVictor.Vector2 origin(0,0)
	Hull_component = Hull(16000, 0.4, 17.5, origin)
	Object_components.insert(Object_components.end(), Hull_component)
	# we construct the hull component and insert it into the general component
	# list. The vessel can access it by either the Hull component reference
	# that it stores internally, through its general version in the parts
	# list
			
	# the pointer passin crap didnt work ^^
	# ^ not sure exactly what that was referring to
	Main_fuel = Resource_Tank(initial_main_propellant, 800, 0.6, 0.8, 2.2, origin)
	Fuel_tanks.insert(Fuel_tanks.end(), Main_fuel);	
	Object_components.insert(Object_components.end(), Main_fuel.Get_vessel_component_pointer())
	# construct the main fuel tank and insert it into both the fuel tank list
	# and the general parts list.
	# Come to think of it, exactly is there a separate list for fuel tanks?
	# they arent even accessed that way, yet anyways
	
	Init_thruster(False, 4000, 50000, 290, 0, -6.2000, 0, -1, 0.4, 0.49, Main_fuel, main_engines); 
	# construct the main engine thrsuter, it into the thruster vector
	# and the general components vector
	
	# self is at least necessary, the main thruster vector is used for
	# throttling back all engines each frame unless the throttle up is doing
	# the opposite
	
	VectorVictor.Vector2 rcs_pos(0, 2)
	RCS_fuel = Resource_Tank(initial_rcs_propellant, 500, 0.6, 0.64, 1,rcs_pos)
	Fuel_tanks.insert(Fuel_tanks.end(), RCS_fuel);		
	Object_components.insert(Object_components.end(), RCS_fuel.Get_vessel_component_pointer());	
	
	Init_thruster(True, 470, 49000, 61, 0, 6.2000, -1, 0, 0.10, 0.12, RCS_fuel, rotate_clockwise, translate_right)
	Init_thruster(True, 470, 49000, 61, 0, 6.2000, 1, 0, 0.10, 0.12, RCS_fuel, rotate_counterclockwise, translate_left)
	Init_thruster(True, 470, 49000, 46, 0, 6.400, 0, 1, 0.10, 0.12, RCS_fuel, translate_back); 
	Init_thruster(True, 470, 49000, 63, -0.20, -6.0000, -1, 0, 0.10, 0.12, RCS_fuel, rotate_counterclockwise, translate_right); 
	Init_thruster(True, 470, 49000, 63, -0.20, -6.0000, 1, 0, 0.10, 0.12, RCS_fuel, rotate_clockwise, translate_left); 
	Init_thruster(True, 470, 49000, 46, -0.2, -6.0000, 0, -1, 0.10, 0.12, RCS_fuel, translate_forward)
		
	# and we construct all of the RCS thrusters in the same way
	
	# self should definitely be a function to minimize mistakes
	
	std.cout << Object_components.size() << " Vessel components" << std.endl
	# not sure why, good to have the number of components on the vessel
	# for debugging
	
	RCS_Throttle_constant = 29.9000
	# we set self up so that the thruster goes from 0 to 2990 % in one second
	# obviously that doesnt work, it really represents 0 to 100% in 1/29.9 s
	# (0.033 s, 2 frames if fps is a healthy 60)
	
	Main_throttle_constant = 0.25
	# significantly slower for the main engines, they take a full four
	# seconds to go from 0 - 100
	
	Throttle_lock = False
	# Throttle lock keeps the main engines at their current throttle setting
	# instead of trying to slide back to zero each frame, is useful
	# during long burns
	
	Update_PMI()
	# once all of the vessel components are in, should be run to get the
	# initial value
	# we create it and assign it the texture passed by the constructor
	# (less memory waste per vessel than each one loading a copy of the texture
	# for every instance
	Vessel_tex = sf.Texture();	
	*Vessel_tex = vessel_textures
	# not sure why we need to copy the reference to self...
	Object_sprite = sf.Sprite()
	Object_sprite.setTexture(*Vessel_tex)
	# self appears to be redundant, a good candidate to remove
	text_colour = sf.Color(194, 38, 15)
	# nice red colour for all of the vessel displays
	
	vessel_id.Init_object(controls_font, sf.Vector2f(20, 380), "-", *text_colour, 16, False)
	main_fuel_level.Init_object(controls_font, sf.Vector2f(20, 420), "-", *text_colour, 14, False)
	rcs_fuel_level.Init_object(controls_font, sf.Vector2f(20, 440), "-", *text_colour, 14, False);	
	omega_value.Init_object(controls_font, sf.Vector2f(20, 460), "-", *text_colour, 14, False);		
	theta_value.Init_object(controls_font, sf.Vector2f(20, 480), "-", *text_colour, 14, False);	
	position_values.Init_object(controls_font, sf.Vector2f(20, 500), "-", *text_colour, 14, False);	
	velocity_values.Init_object(controls_font, sf.Vector2f(20, 520), "-", *text_colour, 14, False);		
	vessel_display.Init_object(sf.Vector2f(820, 460), sf.Color(255, 255, 255, 168), False, *Vessel_tex, True)
	vessel_display.sprite.setScale(sf.Vector2f(0.5f, 0.5f))
	display_panel.Init_object(sf.Vector2f(5, 360), sf.Color(255, 255, 255), False, panel_path)
	
	pix_length = Hull_component.Length*10
	# get the length of the vessels long axis in meters
	# the multiple of ten thing is fine, the current window size was
	# accidentally made 10x too large, vessels appeared 10x larger than
	# they really were
	
	# not terribly important, self will change when the window view
	# has multiple scales to be a lot like the one for planets, it is
	# important to remember that the base view 1m = 10 pixels, reasons
	# of easiest scaling
	if Vessel_tex.getSize().y != pix_length:
	{	Object_sprite.setScale((pix_length/(Vessel_tex.getSize().y)),(pix_length/(Vessel_tex.getSize().y)))
		# rescale the axes of the texture to match pix_length in the y and the
		# appropriate scale for the x dimension
		
		# not sure why they both use y, I think self was distorted when
		# it used x & y.
		# This area needs to be looked over again
	}	
	Object_sprite.setOrigin((Vessel_tex.getSize().x/2), (Vessel_tex.getSize().y/2))
	Object_sprite.setRotation(self.NewtonianState.Rotation.Theta)
	# and we center the origin and rotate it appropriately
	Talkback("...Constructed Delta Glider")
	# feed back that the constructor is now finished to the console


def Receive_inputs(self, * current_inputs, dt):
{	if current_inputs.z == True:
	{	Translate_left(dt)

	elif current_inputs.c == True:
	{	Translate_right(dt)
	}	
	elif current_inputs.x == True:
	{	Translate_backward(dt)

	elif current_inputs.w == True:
	{	Translate_forward(dt)

	elif current_inputs.d == True:
	{	Rotate_right(dt)

	elif current_inputs.a == True:
	{	Rotate_left(dt)
	}	
	elif current_inputs.s == True:
	{	Kill_rotation(dt)
	}	# map the rotational and translational commands to the WASD side of
		# the keyboard. It works very similar to the numpad in Orbiter, I
		# used the keyboard for it since numpad inputs dont seem to be received
		# on linux for some reason
		
		# It can always work on both, I think using the WASD keys makes
		# more sense, every keyboard will have that, not necessarily
		# a numpad
	else:
	{	No_command(dt)
	}	# the throttle back of every thruster since no inputs are being sent
		# intended for RCS thrusters, also applies to main/retro/hover,
		# not exactly a perfect setup here
		
	if current_inputs.up == True:
	{	Throttle_up(dt*5)

	elif current_inputs.down == True:
	{	Throttle_down(dt*5)



def Get_total_mass(self):
{	net_mass = 0
	for(std.vector<Vessel_component*>it = Object_components.begin(); it != Object_components.end(); ++it)
	{	net_mass += (*it).Get_component_mass()
	}	# iterate through all components and get the mass. I think self was
		# defined already in TVessel or Newtonian, need definition here	
	return net_mass


long double DeltaGlider.Get_PMI()
{	return PMI


def Print_data(self):
{	std.cout << "RCS Fuel: " << RCS_fuel.Resource_mass << std.endl
	std.cout << "Crash state: " << self.NewtonianState.Current_state << std.endl
	# general useful data
	
	# should probably reformat self a wee bit


def Toggle_throttle_lock(self):
{	if Throttle_lock == True:
	{	Throttle_lock = False

	else:
	{	Throttle_lock = True

	# simple bit flippin


def Draw_controls(self, * iwindow, Map_status):
{	mainfuel = std.to_string((long long int)Main_fuel.Resource_mass)
	mainfuel.append(" kg")
	main_fuel_level.Set_element(mainfuel)
	
	rcsfuel = std.to_string((long long int)RCS_fuel.Resource_mass)
	rcsfuel.append(" kg")
	rcs_fuel_level.Set_element(rcsfuel);	
	
	omega_value.Set_element(std.to_string((long long int)self.NewtonianState.Rotation.Omega));	
	
	theta_value.Set_element(std.to_string((long long int)self.NewtonianState.Rotation.Theta))
	
	pos = "Position "
	pos.append(NewtonianState.FlightState.Position.Get_vector("m"));	
	position_values.Set_element(pos);	
	
	vel = "Velocity "
	vel.append(NewtonianState.FlightState.Velocity.Get_vector("m/s"))
	velocity_values.Set_element(vel);	
	
	id = Get_vessel_class()
	id.append(" Class\n")
	id.append(Get_vessel_name())
	vessel_id.Set_element(id)
	# do a hideous transfer of formatted data to the sf texts
	
	vessel_display.Draw_element(iwindow)
	display_panel.Draw_element(iwindow)
	
	main_fuel_level.Draw_element(iwindow)
	rcs_fuel_level.Draw_element(iwindow);	
	
	omega_value.Draw_element(iwindow)
	theta_value.Draw_element(iwindow)
	
	position_values.Draw_element(iwindow)
	velocity_values.Draw_element(iwindow)
	
	vessel_id.Draw_element(iwindow)
	


def Receive_cursor_inputs(self, * cursor_action, double dt):
{	# self way we can check if the cursor is on us, whether that merits
	# any changes on the vessels end of things



DeltaGlider.~DeltaGlider()
{	Thrusters.clear()
	Fuel_tanks.clear()
	delete Object_sprite
	delete Hull_component
	delete text_colour
	delete Hull_component


TVessel * Construct_deltaglider(ObjectState initial_object_state, initial_main_propellant, initial_rcs_propellant, vessel_textures, flag_sprite, ivessel_name, panel_path, controls_font, propagator)
{	TVessel new_deltaglider = DeltaGlider(initial_object_state, initial_main_propellant, initial_rcs_propellant, vessel_textures, flag_sprite, ivessel_name, panel_path, controls_font, propagator)
	# create a DeltaGlider on the heap
	return new_deltaglider
	# and pass along its pointer at the TVessel level


def Create_deltaglider(self, &Vessels_list, &Newtonian_list, New_dg_pointer):
{	Vessels_list.insert(Vessels_list.end(), New_dg_pointer)
	Newtonian_list.insert(Newtonian_list.end(), New_dg_pointer.Get_Newtonian_pointer())
	return True

