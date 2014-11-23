// Ignition_Engine.h ///////////////////////////////////////////////////////////
// The core classes for the Ignition Engine ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "Game_Objects.h"

class Ignition_engine
{	public:
	Ignition_engine(std::string title, unsigned int initial_window_width, unsigned int initial_window_height, std::string program_version, double redraw_displays_interval, std::string Intro_audio_path, std::string Game_audio_path);
	SFML_Window * Main_Window;
	key_commands * commands;
	TVessel * Current_vessel;
	
	sf::Texture * Background_tex;
	sf::Sprite * Background_sprite;
	
	SFML_gameaudio * Game_audio;
	
	bool map_view;
	long double deltat;	
	double Redraw_interval;	
	int zoom_exponent;
	int time_acceleration_exponent;
	long double simulation_time;
	std::string Program_version;
	
	bool Displays_active;	
	
	int Ignition();		
	~Ignition_engine();
};

void Init_assets();

void Init_text_displays();

void Update_text_displays();
void Redraw_text_displays(bool in_map_view, SFML_Window * draw_window);

void Exit_program();
