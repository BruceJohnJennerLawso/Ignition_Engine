// Ignition_Engine.h ///////////////////////////////////////////////////////////
// The class wrapper for the Ignition Engine as a whole ////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "Game_Objects.h"

#ifndef IgnitionEngine
#define IgnitionEngine

class Ignition_engine
{	public:
	Ignition_engine(std::string title, unsigned int initial_window_width, unsigned int initial_window_height, std::string program_version, double redraw_displays_interval, std::string Intro_audio_path, std::string Game_audio_path);
	// base constructor
	// future versions should eventually have more overloads for things like
	// start on construction, loading from a scn file or somesuch...
	// constructors always seem to be such exhausting work for some reason...
	
	
	SFML_Window * Main_Window;
	// pointer to our main window object, containing a sf::RenderWindow along
	// with some info about its title, viewport size & so on...
	
	key_commands * commands;
	// the awkward but effective method of handling inputs to objects
	// gets sent to the current vessel each frame
	
	TVessel * Current_vessel;
	// Reference to the current vessel the player is controlling. Might be
	// better to replace this with something less risky like a string handle
	
	sf::Texture * Background_tex;
	sf::Sprite * Background_sprite;
	// texture and sprite for our background starfield
	
	SFML_gameaudio * Game_audio;
	// Hideous thing that handles our music. Needs a lot of work eventually
	// but its working for now, so dont touch
	
	bool map_view;
	// is the map view active? False denotes normal camera view
	
	long double deltat;	
	// length of the frame previous to the current one in seconds. 
	// retrieved by a lovely sf::Clock in our main loop.
	// since the whole ignition may be passed by reference back to the game
	// objects themselves, this might be better to separate into actual deltat
	// and the modified value based on time acceleration
	
	double Redraw_interval;	
	// uhhh... oh yeah, the period for the refresh cycle of the onscreen
	// feedback displays, so that we arent wasting lots of processing power
	// updating their contents every frame when every 1-2 s or so is just fine. 
	// Also makes them easier to read too IIRC
	
	// this should also be tied into MFD/DAI display refresh methinks, since
	// the same problem applies there
	
	long long int zoom_exponent;
	// the exponent a of the scale of our map view (ie base scale*(10^a))
	// 10^a is multiplied by the dimensions of the base map view (1024 x 609?)
	// originally this was some sort of issue with resets over a certain scale,
	// but its fixed now. I think this is safe to transfer back to a normal
	// int, the problem was located elsewhere I believe
	
	int time_acceleration_exponent;
	// same concept as the zoom exponent, just multiplied by the base deltat
	// time so that we can run the program faster than just real time
	
	
	long double simulation_time;
	// the total time the simulation has been open in in-universe seconds
	// (ie time accelerated time, not real time). This is a prime candidate
	// for merging with the time class from Countdown, seconds are unwieldy
	// for long timescales like this
	
	std::string Program_version;
	// ahhh... this might be the window titles last few digits indicating the
	// project number (0.1, 0.11, 0.71, 0.9, 1.0, etc...). Or maybe it was
	// something else entirely, I dont remember right now
	
	// this should be investigated soon, although I see no reason why it would
	// cause instability, so no worries
	
	bool Displays_active;	
	// indicates whether the feedback displays are on or off, toggled by hitting
	// the i key just like in Orbiter. This turns off everything save the basic
	// rendering (stars, ships, planets, etc)
	
	int Ignition();		
	// Lights this candle!
	// Basically the function that runs everything in the Ignition_Engine object
	// automatically
	
	// so you simply need to create a ignition engine object with its
	// constructor, call Ignition, and everything is handled from there
	
	std::vector<CKeplerian_Object*> Celestial_list;
	std::vector<CNewtonian_Object*> Newtonian_list;
	std::vector<TVessel*> Vessel_list;
	
	void Next_vessel();
	void Previous_vessel();
	
	void Map_view();
	void Increase_map_scale();
	// zooming out, making the area it covers bigger
	void Decrease_map_scale();
	// zooming in, making the area it covers smaller
	
	void Camera_view();
	
	void Set_aperture_scale();
	
	~Ignition_engine();
};

void Init_assets();
// load up our external assets in a baddish way. really needs fixing soon


void Init_text_displays();
// same as assets, just the default feedback displays (fps, simtime, time accel)
// at the very least this should clearly be done in the Ignition_Engine object

void Update_text_displays();
// ah gets feedback from the Ignition object to update those generic outputs
// another thing that should go ignition native

void Redraw_text_displays(bool in_map_view, SFML_Window * draw_window);
// same as above, just the draw calls for generic displays

void Exit_program();
// ugly cleanup function that never seems to have everything it needs.
// again, should be superceeded by the Ignition destructor
// just ugly remnants of a young project

#endif
