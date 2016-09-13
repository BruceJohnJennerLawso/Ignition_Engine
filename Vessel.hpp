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

enum orientation{upper_left, lower_left, upper_right, lower_right};


class MFD;

// forward declarin yo

bool Create_MFD(std::string type_name, MFD * &new_mfd, orientation mfd_orientation, sf::Vector2f mfd_offset);


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
	// commands come in one by one, they shouldn't. I hope
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
	
	std::vector<MFD*> MFD_displays;
	bool Start_MFD(std::string mfd_type, orientation mfd_orientation, sf::Vector2f mfd_offset);
	void Render_MFDs(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation);
	void Draw_MFDs(SFML_Window &window);
	
	TVessel* Get_Vessel_pointer();
	// the access at this abstraction layer
	// this could always be tossed around by passing *pointer by reference
	// in functions I guess
};

class MFD
{	public:

	virtual std::string Get_MFD_Name();

	virtual bool Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation);
	// phew, I think that is everything
	void Draw_MFD(SFML_Window &window, sf::Color Placard_color);
	// and the function that actually creates a sprite and slaps it onto the 
	// SFML window that we pass by reference
	
	// we call Render_MFD to refresh the image every couple of milliseconds, and
	// we call Draw_MFD to actually draw the MFD to the window *EVERY FRAME*
	// sudo dodododo important, must remember that
	
	// also, each vessel calls Draw MFD in a function that Ignition Engine
	// calls for it, so that we can pass internal parameters like the colour for
	// each vessel
	
	sf::Texture MFD_image;
	orientation MFD_orientation;
	void Set_MFD_orientation(orientation new_orientation);
	
	sf::Vector2f MFD_offset;
	// ie this way we can tell it where it should be relative to onscreen, ie
	// from which corner, so that mfds show up in the right places regardless
	// of window size
	void Set_MFD_offset(sf::Vector2f new_offset);
	float Height, Width;
	// define two things:
	// -the max size of the Texture image output size (if we go over it, we can
	// just relative scale the output sprite back to fit in the correct size)
	
	// the size of the 'placard' that we draw behind the mfd so it has a
	// background to make it easier to see
	// this is an sf::Color stored by the vessel, so individual vessels can have
	// a 'style'
	
	// in a future version commented out for the moment, it can also be done as
	// a passed texture which can be scale locked, so that users can create cool
	// looking mfd styles (like an apollo panel for the Apollo CSM MFDs)
	// this might be a wee bit glitchy for the mfd having its image forcibly
	// resized like that, but the feature is worthwhile IMO
	
	sf::Vector2f Get_mfd_position(SFML_Window &window);
	// uses the render window, our image offset, the height, the width, and the
	// orientation to tell us where to draw stuff
	
	// specifically, this tells us in window coordinates, the location of
	// the upper left corner of this MFD
	
	sf::Vector2f Transform_to_mfd(SFML_Window &window, sf::Vector2f input);
	
	//virtual bool Load(filestuffz);
	//virtual bool Save(filestuffz&);
	// save things to a text scenario file (an .isc ?)
	// just like how parts can do that so we can have persistent MFDs
	// this will allow for things like guidance programs in the MFD to be
	// persistently saved from session to session
	
	//virtual ~MFD();
};



#endif
