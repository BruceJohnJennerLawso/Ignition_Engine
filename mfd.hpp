// mfd.hpp /////////////////////////////////////////////////////////////////////
//  MFD Multifunctional Displays, so we dont have to fly blind anymore /////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include "Vessel.hpp"

enum orientation{upper_left, lower_left, upper_right, lower_right};

class MFD
{	public:

	virtual std::string Get_MFD_Name();

	virtual bool Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation);
	// phew, I think that is everything
	void Draw_MFD(SFML_Window &window, sf::Color Placard_color);
//	virtual void Draw_MFD(SFML_Window &window, sf::Texture Placard_background, bool scale_lock);	
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
	
	sf::Vector2f MFD_offset;
	// ie this way we can tell it where it should be relative to onscreen, ie
	// from which corner, so that mfds show up in the right places regardless
	// of window size
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
	
	//virtual bool Load(filestuffz);
	//virtual bool Save(filestuffz&);
	// save things to a text scenario file (an .isc ?)
	// just like how parts can do that so we can have persistent MFDs
	// this will allow for things like guidance programs in the MFD to be
	// persistently saved as an MFD
	
	//virtual ~MFD();
};



class Surface_MFD: public MFD
{	Surface_MFD();
	//ahh... hmm, not actually sure
	// maybe it can sort the list to find out the closest planet and save its
	// string name
	
	// also the window orientation... etc.
	std::string Get_MFD_Name();
	
	bool Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation);
	//void Draw_MFD(SFML_Window &window, sf::Color Placard_color);
	// dont actually need the function, its done in the MFD class since we dont
	// need to know anything about the specific mfd to do the grunt work
	// of drawing it
	
	~Surface_MFD();
};


//bool Create_MFD(
