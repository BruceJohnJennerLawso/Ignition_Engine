#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "VectorVictor2.h"

#ifndef SFMLTools
#define SFMLTools

std::string Input_string(std::string prompt);

struct vector2
{	long double x, y;	
};

class Program_data
{	Program_data();
	enum prog_state{Intro, Menu, Game}Game_status;
	~Program_data();
};

// Key_commands class //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class key_commands
{	public:
	key_commands();
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
	bool enter, space, period, comma, up, down, right, left, tilde, v, c, q, w, e, a, s, d, f, n,  i, l, x, z, t, r, dash, equal, numpad0, numpad1, numpad2, numpad3, numpad4, numpad5, numpad6, numpad7, numpad8, numpad9, plus, minus;
	~key_commands();
};	// important future idea for this is to retrieve mouse location & clickstate

void Log_keystroke(sf::Keyboard::Key input_event, key_commands * icommands, bool key_down);
void Null_queue();

// SFML Window /////////////////////////////////////////////////////////////////
// Cause I keep getting lost every time I look for this ////////////////////////

class SFML_Window
{	public:
	SFML_Window(std::string title, unsigned int h, unsigned int w);
	sf::RenderWindow * window;
	VectorVictor::Vector2 origin;
	void Set_origin();
	void Set_origin(long double x, long double y);
	void Set_aperture_dimensions(long long unsigned int ap_w, long long unsigned int ap_h);
	std::string Title;
	long long unsigned int Width, Height;
	// the dimensions of the actual window itself, in pixels
	long double Aperture_width, Aperture_height;
	// the dimensions of what the window is currently spanning, in pixels(1px/m)
	~SFML_Window();
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
