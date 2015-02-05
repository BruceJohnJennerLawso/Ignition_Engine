// SFML_Tools.h ////////////////////////////////////////////////////////////////
// A couple of sfml wrappers that  automate a few useful things ////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "VectorVictor2.hpp"

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


// SFML Window /////////////////////////////////////////////////////////////////
// Cause I keep getting lost every time I look for this ////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SFML_Window
{	public:
	SFML_Window(std::string title, unsigned int h, unsigned int w);
	// implicit rotation of 0 in this case
	SFML_Window(std::string title, unsigned int h, unsigned int w, long double initial_rotation);
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
	long double Aperture_rotation;
	// clockwise about the origin, in degrees
	void Set_aperture_rotation(long double new_rotation);
	// set the rotation to its new value
	void Set_aperture_dimensions(long long unsigned int ap_w, long long unsigned int ap_h);
	// set those dimensions in meters
	bool Intersection(VectorVictor::Vector2 center, long double radius);
	// check if a sphere with center and radius intersects the aperture box of
	// the window, so that we know if it should be drawn
	~SFML_Window();
	// close up shop (and delete our RenderWindow)
};

// helper functions here to automate transforming a sf::Vector2f location in ///
// a window with float precision to a VV2 with long double precision in the ////
// engines global coordinate system ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// there isnt anything really new here, just an automation of an existing //////
// process that a couple of functions implemented each in their own way ////////

sf::Vector2f Get_window_coordinates(VectorVictor::Vector2 sim_point, SFML_Window * iwindow, int map_scale);

sf::Vector2f Get_window_coordinates(VectorVictor::Vector2 sim_point, SFML_Window * iwindow, double cam_scale);

VectorVictor::Vector2 Get_simulation_coordinates(sf::Vector2f window_point, SFML_Window * iwindow, int map_scale);

VectorVictor::Vector2 Get_simulation_coordinates(sf::Vector2f window_point, SFML_Window * iwindow, double cam_scale);




// Generic displays ////////////////////////////////////////////////////////////
// This whole section is kinda hazy. Basically, the idea is that we wrap ///////
// anything that is sf::Drawable into a wrapper class with some useful /////////
// methods so that working with it is much easier //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Ignition_drawable
{	public:
	virtual void Draw_element(SFML_Window * iwindow);
	
	bool Centered;
	virtual void Center_element();
	//std::vector<std::vector<*Ignition_drawable>> idrawable_pointers;
	//std::vector<Ignition_handle<Ignition_drawable>
	//bool Clean_drawable_references();
	// this idea must wait for now
};

// next thing to implement is rotation, since pretty much everything here is
// sf::Transformable

class Ignition_text: public Ignition_drawable
{	public:
	Ignition_text(sf::Font &text_font, sf::Vector2f initial_position, std::string initial_text, sf::Color initial_colour, unsigned int character_size, bool center_origin);
	Ignition_text();
	bool Init_object(sf::Font &text_font, sf::Vector2f initial_position, std::string initial_text, sf::Color initial_colour, unsigned int character_size, bool center_origin);
	
	// should receive a sf::Font by reference
	void Set_element(std::string text_string);
	void Set_element(sf::Vector2f new_position);
	void Set_element(sf::Color new_colour);	
	void Set_element(unsigned int character_size);
	//	size is in pixels, although I dont know if this is height or width
	void Set_element(std::string text_string, sf::Vector2f new_position);
	void Set_element(std::string text_string, sf::Vector2f new_position, sf::Color new_colour);
	void Set_element(std::string text_string, sf::Color new_colour);
	void Set_element(sf::Vector2f new_position, sf::Color new_colour);
	// 
	void Draw_element(SFML_Window * iwindow);
	void Center_element();
	sf::Text text;
	
	~Ignition_text();
	// later on this destructor will use info stored about the stored references
	// to this object to clean up all of said references so that null pointers
	// arent laying around like landmines
};

class Ignition_circle: public Ignition_drawable
{	public:
	Ignition_circle(sf::Vector2f initial_position, sf::Color initial_colour, float initial_radius, bool center_origin);
	Ignition_circle();
	// later this will have a second constructor to texture it with a passed
	// sf::Texture reference
	bool Init_object(sf::Vector2f initial_position, sf::Color initial_colour, float initial_radius, bool center_origin);
	// the init call in case we dont have a direct call to the constructor
	void Set_element(float new_radius);
	void Set_element(sf::Vector2f new_position);	
	void Set_element(sf::Color new_colour);	
	void Set_element(float new_radius, sf::Vector2f new_position);
	void Set_element(float new_radius, sf::Vector2f new_position, sf::Color new_colour);
	void Set_element(float new_radius, sf::Color new_colour);
	void Set_element(sf::Vector2f new_position, sf::Color new_colour);	
	// all of the given setters, applied directly to our sf::CircleShape,
	// instead of holding internal variables. There could be a lot of displays
	// in sim, so we want to keep memory usage to a bare minimum
	void Draw_element(SFML_Window * iwindow);
	void Center_element();
	sf::CircleShape circle_shape;
	// oddly, sfml seems to automatically pick a point count for circles by
	// default, so that the given circle looks halfway decent. oh well...
	~Ignition_circle();
};

class Ignition_rectangle: public Ignition_drawable
{	public:
	Ignition_rectangle(sf::Vector2f initial_position, sf::Color initial_colour, float width, float height, bool center_origin);
	Ignition_rectangle();
	// later this will have a second constructor to texture it with a passed
	// sf::Texture reference
	bool Init_object(sf::Vector2f initial_position, sf::Color initial_colour, float width, float height, bool center_origin);
	void Set_element(float new_width, float new_height);
	void Set_element(sf::Vector2f new_position);	
	void Set_element(sf::Color new_colour);	
	void Set_element(float new_width, float new_height, sf::Vector2f new_position);
	void Set_element(float new_width, float new_height, sf::Vector2f new_position, sf::Color new_colour);
	void Set_element(float new_width, float new_height, sf::Color new_colour);
	void Set_element(sf::Vector2f new_position, sf::Color new_colour);	
	void Center_element();
	void Draw_element(SFML_Window * iwindow);
	sf::RectangleShape rect_shape;
	~Ignition_rectangle();
};

class Ignition_sprite: public Ignition_drawable
{	public:
	Ignition_sprite(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, std::string texture_path);
	Ignition_sprite(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, sf::Texture texture, bool copy_texture);
	Ignition_sprite();
	
	bool Init_object(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, std::string texture_path);
	bool Init_object(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, sf::Texture &texture, bool copy_texture);
	
	void Set_element(sf::Vector2f new_position);
	void Set_element(sf::Vector2f new_position, sf::Color new_colour);
	void Set_element(sf::Color new_colour);
	void Center_element();
	void Draw_element(SFML_Window * iwindow);
	
	sf::Texture Tex;
	sf::Sprite sprite;
	~Ignition_sprite();
};


//class Ignition_line: public Ignition_drawable
//{	public:
//	Ignition_line();
//	sf::VertexArray line;
//	~Ignition_line();	
//};

//class Ignition_triangle: public Ignition_drawable
//{	public:
//	Ignition_triangle();
//	sf::VertexArray triangle;
//	// gotta find the appropriate setters here
//	~Ignition_triangle();
//	// a nice sf primitive vertex here
//};

// here there be dragons, big ones with sharp pointy teeth


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

#endif
