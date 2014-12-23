// SFML_Tools.h ////////////////////////////////////////////////////////////////
// A couple of sfml wrappers that  automate a few useful things ////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "VectorVictor2.h"

#ifndef SFMLTools
#define SFMLTools

std::string Input_string(std::string prompt);

struct vector2
{	// old data type used by sfml tools before VV2 was available here
	// should be removed wherever possible for consistency
	
	// for all intents and purposes, this is the exact same thing as VV2,
	// but an object oriented interface is a much better idea
	long double x, y;	
};

class Program_data
{	// not actually sure what this is, think it was just a passing idea
	// that never actually got implemented
	Program_data();
	enum prog_state{Intro, Menu, Game}Game_status;
	~Program_data();
};

// Mouse_commands class ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Cursor_commands
{	public:
	Cursor_commands();
	sf::Vector2f position;
	// the coordinates of the mouse relative to the window?
	
	// I dont actually know yet, so this is kinda going in blind. We'll see how
	// sfml likes to handle things
	
	// my working assumption here is that it will be in pixels, from the window origin
	bool Right_click, Left_click, Middle_click;
	// if the given mouse button is down, its true, otherwise false
	// dont want to mess about with extra buttons just yet
	
	void Set_cursor_state(float x, float y, bool right, bool left, bool middle);
	~Cursor_commands();
	// surprisingly this whole section went off without a hitch. suspicious...
};

// Key_commands class //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class key_commands
{	public:
	key_commands();
	// simple constructor, no params needed
	void Enter();
	void Space();
	void Period();
	void Comma();
	void Up();
	void Down();
	void Right();
	void Left();
	void Tilde();
	void Q();
	void W();
	void E();
	void A();
	void S();
	void D();
	void C();
	void N();
	void F();
	void I();
	void V();
	void Z();
	void X();
	void R();
	void T();
	void L();
	void Dash();
	void Equal();
	void Numpad_0();
	void Numpad_1();
	void Numpad_2();
	void Numpad_3();
	void Numpad_4();
	void Numpad_5();
	void Numpad_6();
	void Numpad_7();
	void Numpad_8();
	void Numpad_9();
	void Plus();				// oddly enough, this hasnt caused any errors by not being defined
	void Minus();
	// all of the functions that get called when log keystroke indicates that
	// the key in question was pressed
	bool enter, space, period, comma, up, down, right, left, tilde, v, c, q, w, e, a, s, d, f, n,  i, l, x, z, t, r, dash, equal, numpad0, numpad1, numpad2, numpad3, numpad4, numpad5, numpad6, numpad7, numpad8, numpad9, plus, minus;
	// and the booleans that are set to the key down status of the given key
	// in the same function
	~key_commands();
};	// important future idea for this is to retrieve mouse location & clickstate

void Log_keystroke(sf::Keyboard::Key input_event, key_commands * icommands, bool key_down);
void Null_queue();
// I dont think this was ever defined. Probably safe to remove




// Generic displays ////////////////////////////////////////////////////////////
// This whole section is kinda hazy. Basically, the idea is that we wrap ///////
// anything that is sf::Drawable into a wrapper class with some useful /////////
// methods so that working with it is much easier //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Ignition_drawable
{	public:
	virtual void Draw_element(SFML_Window * iwindow);
};

class Ignition_text: public Ignition_drawable
{	public:
	Ignition_text();
	// this needs params specific to sf::Text, and sf::Font
	void Draw_element(SFML_Window * iwindow);
	sf::Text text;
};


// SFML Window /////////////////////////////////////////////////////////////////
// Cause I keep getting lost every time I look for this ////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SFML_Window
{	public:
	SFML_Window(std::string title, unsigned int h, unsigned int w);
	// constructor, quite simple really
	std::string Title;
	// the string that gets printed on the menubar at the top of the window
	
	// might be removable if the sf::RenderWindow already contains a string
	// for this internally
	sf::RenderWindow * window;
	// the pointer to the sfml window that the class wraps
	VectorVictor::Vector2 origin;
	// the ingame origin (top left corner) of the windows aperture
	// (the area that the window covers)
	void Set_origin();
	// if not specified, sets it to (0,0)
	void Set_origin(long double x, long double y);
	// or set the coordinates of the origin more explicitly
	long long unsigned int Width, Height;
	// the dimensions of the actual window itself, in pixels
	long double Aperture_width, Aperture_height;
	// the dimensions of what the window is currently spanning in the game
	// universe, in meters
	void Set_aperture_dimensions(long long unsigned int ap_w, long long unsigned int ap_h);
	// set those dimensions in meters
	~SFML_Window();
	// close up shop (and delete our RenderWindow)
};

class SFML_loadscreen
{	public:
	SFML_loadscreen(std::string texture_path, bool fadein, double fade_in_length, double fade_out_length);
	void Fade_in();
	void Fade_out();
	void Update_screen();
	void Toggle_fade();
	double Fader;
	bool Fading_in, Fading_out, Active;
	double Fade_in_time, Fade_out_time;
	sf::Clock * Fade_clock;
	sf::Texture * splash_texture;
	sf::Sprite * splash_sprite;
	~SFML_loadscreen();
};

class SFML_titlescreen
{	public:
	SFML_titlescreen(std::string texture_path, bool fadein, double fade_in_length, double fade_out_length, std::string title, std::string font_path, unsigned int textr, unsigned int textg, unsigned int textb, unsigned int pfont, sf::Vector2f Title_pos);
	void Fade_in();
	void Fade_out();
	void Update_screen();
	void Toggle_fade();
	double Fader;
	bool Fading_in, Fading_out, Active;
	double Fade_in_time, Fade_out_time;
	unsigned int r, g, b;
	bool finished;
	sf::Text * Title_text;
	sf::Font * Title_font;
	sf::Vector2f Title_position;
	sf::Clock * Fade_clock;
	sf::Texture * splash_texture;
	sf::Sprite * splash_sprite;
	~SFML_titlescreen();
};

class SFML_gameaudio
{	public:
	SFML_gameaudio(std::string Intro_audiopath, std::string Game_musicpath);
	std::vector<std::string> Music_paths;
	void Music_mode(bool menu_mode);
	
	bool Intro_finished;
	sf::SoundBuffer * Intro_audio;
	sf::Sound * Audio;
	
	void Update_game_audio();			// this call is ahh...
	void Increment_game_music();		// move us on to the next file in the list... a lot of this is kinda bad... I dont know...
	
	void Play_intro();
	
	std::vector<std::string>::iterator it_music;
	sf::Music * Game_music;
		
	void Play_music(bool loop);
	void Stop_music();
	~SFML_gameaudio();
};

#endif
