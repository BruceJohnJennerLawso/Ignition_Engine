# Vessel.hpp #################################
# The TVessel type used for vessels #####################/
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Newtonian_objects.hpp"

#ifndef vESSEL
#define vESSEL

enum orientation{upper_left, lower_left, upper_right, lower_right


class MFD

# forward declarin yo

def Create_MFD(self, type_name, * &new_mfd, mfd_orientation, mfd_offset):


# TVessel Class ###############################/
########################################

class TVessel: virtual public CNewtonian_Object
{	# made virtual so that implementation classes can use CNewtonians
	# constuctor for nice clean code
	public:
	# More specific type than Newtonian, with things that a vessel really
	# must have, a hull & standard ways of dealing with vesselly objects
	Hull * Hull_component
	# the hull, which there should only ever be one
	# I cant imagine why a second one of these should ever be needed
	
	
	double RCS_Throttle_constant, Main_throttle_constant
	
	void Rotate_left(double dt)
	void Rotate_right(double dt)
	
	void Rotate_left(double dt, throttle_target)
	void Rotate_right(double dt, throttle_target)
	
	void Kill_rotation(double dt);					
	# self seems at first to conflict with rot left/right, since input
	# commands come in one by one, shouldnt. I hope
	void Translate_forward(double dt)
	void Translate_backward(double dt)
	void Translate_left(double dt)
	void Translate_right(double dt)
	void Throttle_up(double dt)
	void Throttle_down(double dt)
	void No_command(double dt)
	# basically, Receive_inputs, any other function decides that a
	# particular control action needs to be run, one (or more?) of these
	# methods gets called to make it happen
	
	# I think self is structured wrong, actually have a list of
	# thrusters to engage for a particular control action, the global one
	# gets throttled down
	
	# if the thrusters in the action list are throttled up by double the actual
	# amount, should work
	std.vector<Thruster*> Thrusters
	# the global list containing every thruster on the craft
	bool Init_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_id)
	# we get all of the parameters needed to make a object of type, we
	# pass it the id type of which thruster group it will be placed in
	bool Init_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_id1, group_id2)
	# and we do the same thing, now with two groups
	bool Init_thruster(bool is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_id1, group_id2, group_id3)
	# and with three groups. Seriously, can we possibly need more than 3
	# groups to a thruster?
	
	# the three previous calls are implicitly assumed to be monopropellant
	# thrusters based on the number and type of arguments supplied to the
	# function
	
	long double Get_max_alpha(rotation_direction direction)
	
	sf.Texture * Vessel_tex
	# pointer to the texture of the vessels hull. Pointer is kind of nice here
	# so we dont need to make a copy of the same texture every time a
	# vessel of the same texture gets created (which might happen a lot)
	
	# wait wat
	# noooooooooooooooooooooooooooooooooooooooooooooooooooooooo
	# not want
	
	bool In_view(SFML_Window * window, zoom_factor)
	# This is implemented at self level
	# (I think because we finally have access to the hull & its length)
	bool In_view(SFML_Window * window, double cam_scale)
	
	void Draw_flag(SFML_Window * window, zoom_factor);	
	# also implemented here, no particular reason that I see

	void Draw_vessel(SFML_Window * iwindow, double cam_scale);												
	# self should eventually become a vertex array with the thruster visuals 
	# thrown in alongside it
	virtual void Draw_controls(SFML_Window * iwindow, Map_status)
	# the standard function for any vessel that allows the user to define
	# a very specific display (ie maybe an Apollo AGC display for a LEM?)
	# in SFML types, draw it onscreen each frame.
	

	
	# preferably, think self can be moved upstairs to the Newtonian
	# and renamed Draw_displays
	# no reason why it cant be applied to asteroids & somesuch
	# isnt really that much else besides asteroids thats a Newtonian but
	# not a TVessel... 
	std.string Vessel_class
	# Just the name of the vessel class, ie "Delta-Glider"
	# could also be other distinctions, vessel - Delta Glider
	# as opposed to asteroid
	std.string Get_vessel_class()
	std.string Get_vessel_name()
	# returns for the above string, well as the object name defined in 
	# Newtonian
	
	std.vector<MFD*> MFD_displays
	bool Start_MFD(std.string mfd_type, mfd_orientation, mfd_offset)
	void Render_MFDs(SFML_Window &window, &keyCommands, &cursorCommands, double &cameraScale, double dt, &time_acceleration, double sim_time, &newtonians, &keplerians, &vessels, &Camera_target, double &Camera_rotation)
	void Draw_MFDs(SFML_Window &window)
	
	TVessel* Get_Vessel_pointer()
	# the access at self abstraction layer
	# self could always be tossed around by passing *pointer by reference
	# in functions I guess


class MFD
{	public:

	virtual std.string Get_MFD_Name()

	virtual bool Render_MFD(SFML_Window &window, &keyCommands, &cursorCommands, double &cameraScale, double dt, &time_acceleration, double sim_time, &current_vessel, &newtonians, &keplerians, &vessels, &Camera_target, double &Camera_rotation)
	# phew, think that is everything
	void Draw_MFD(SFML_Window &window, Placard_color)
	# and the function that actually creates a sprite and slaps it onto the 
	# SFML window that we pass by reference
	
	# we call Render_MFD to refresh the image every couple of milliseconds, and
	# we call Draw_MFD to actually draw the MFD to the window *EVERY FRAME*
	# sudo dodododo important, remember that
	
	# also, vessel calls Draw MFD in a function that Ignition Engine
	# calls for it, that we can pass internal parameters like the colour for
	# each vessel
	
	sf.Texture MFD_image
	orientation MFD_orientation
	void Set_MFD_orientation(orientation new_orientation)
	
	sf.Vector2f MFD_offset
	# ie self way we can tell it where it should be relative to onscreen, ie
	# from which corner, that mfds show up in the right places regardless
	# of window size
	void Set_MFD_offset(sf.Vector2f new_offset)
	float Height, Width
	# define two things:
	# -the max size of the Texture image output size (if we go over it, can
	# just relative scale the output sprite back to fit in the correct size)
	
	# the size of the 'placard' that we draw behind the mfd so it has a
	# background to make it easier to see
	# self is an sf.Color stored by the vessel, individual vessels can have
	# a 'style'
	
	# in a future version commented out for the moment, can also be done as
	# a passed texture which can be scale locked, that users can create cool
	# looking mfd styles (like an apollo panel for the Apollo CSM MFDs)
	# self might be a wee bit glitchy for the mfd having its image forcibly
	# resized like that, the feature is worthwhile IMO
	
	sf.Vector2f Get_mfd_position(SFML_Window &window)
	# uses the render window, image offset, height, width, the
	# orientation to tell us where to draw stuff
	
	# specifically, tells us in window coordinates, location of
	# the upper left corner of self MFD
	
	sf.Vector2f Transform_to_mfd(SFML_Window &window, input)
	
	#virtual bool Load(filestuffz)
	#virtual bool Save(filestuffz&)
	# save things to a text scenario file (an .isc ?)
	# just like how parts can do that so we can have persistent MFDs
	# self will allow for things like guidance programs in the MFD to be
	# persistently saved from session to session
	
	#virtual ~MFD()




#endif
