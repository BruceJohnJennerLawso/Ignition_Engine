// mfd.hpp /////////////////////////////////////////////////////////////////////
//  MFD Multifunctional Displays, so we dont have to fly blind anymore /////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include "Vessel.hpp"


#define  surface_mfd "Surface"

class Surface_MFD: public MFD
{	public:
	Surface_MFD();
	//ahh... hmm, not actually sure
	// maybe it can sort the list to find out the closest planet and save its
	// string name
	
	// also the window orientation... etc.
	std::string Get_MFD_Name();
	sf::Font Display_font;
	sf::RenderTexture canvas;
	
	bool Render_MFD(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, TVessel &current_vessel, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation);
	
	~Surface_MFD();
};


