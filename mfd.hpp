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
	virtual void Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double cameraScale, long double dt, 	int time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation);
	// phew, I think that is everything
	virtual void Render_MFD(SFML_Window &window);
	// and a second version that we can call between update passes, since we
	// wanna sync mfd update calls with updating the standard displays too, so
	// we dont have stuff redrawing itself way too fast every frame
	sf::Texture MFD_image;
	orientation Image_orientation;
	sf::Vector2f Image_offset;
	// ie this way we can tell it where it should be relative to onscreen, ie
	// from which corner, so that mfds show up in the right places regardless
	// of window size
	
	//virtual bool Load(filestuffz);
	//virtual bool Save(filestuffz&);
	// save things to a text scenario file (an .isc ?)
	// just like how parts can do that so we can have persistent MFDs
	// this will allow for things like guidance programs in the MFD to be
	// persistently saved as an MFD
	
	virtual ~MFD();
};

class Surface_MFD: public MFD
{	Surface_MFD();
	//ahh... hmm, not actually sure
	// maybe it can sort the list to find out the closest planet and save its
	// string name
	
	// also the window orientation... etc.
	
	void Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double cameraScale, long double dt, 	int time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation);
	void Render_MFD(SFML_Window &window);
};
