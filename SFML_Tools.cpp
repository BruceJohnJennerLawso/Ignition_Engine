#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
//#include<GameObjects!!!!>		Theyre coming, they really are!!!
#include <math.h>
#include "SFML_Tools.hpp"








Cursor_commands::Cursor_commands()
{	position.x = 0.00;
	position.y = 0.00;
	
	Right_click = false;
	Left_click = false;
	Middle_click = false;
}

void Cursor_commands::Set_cursor_state(float x, float y, bool right, bool left, bool middle)
{	position.x = x;
	position.y = y;
	
	Right_click = right;
	Left_click = left;
	Middle_click = middle;
}

Cursor_commands::~Cursor_commands()
{
}



// this whole class is horrible, but there doesnt really seem to be any 
// alternatives that I can see at the moment. Its just too integral to how the
// engine handles inputs with regards to internal objects

key_commands::key_commands()
{	enter = false;
	space = false;
	period = false;
	left = false;
	right = false;
	up = false;
	down = false;
	tilde = false;
	v = false;
	z= false;
	x = false;
	t = false;
	r = false;
	q = false;
	w = false;
	e = false;
	a = false;
	s = false;
	d = false;
	c = false;
	l = false;
	i = false;
	comma = false;
	equal = false;
	dash = false;
	plus = false;
	numpad0 = false;
	numpad1 = false;
	numpad2 = false;
	numpad3 = false;
	numpad4 = false;
	numpad5 = false;
	numpad6 = false;
	numpad7 = false;
	numpad8 = false;
	numpad9 = false;
	plus = false;
	minus = false;
}

// may cause burning in eyes, but it is useful

key_commands::~key_commands()
{
}

// the function for handling which keyboard input was sent //

void Log_keystroke(sf::Keyboard::Key input_event, key_commands * icommands, bool key_down)
{	// simple concept really, just pass whether the key is being held down
	// as true or false to the stored value for that key, then if it is true,
	// also call the function for that keypress (so things can be handled a bit
	// easier in the main cpp file
	
	// to be honest though, it might just be easier to directly store a copy
	// of the sf event itself
	
	// but I still want to store info about the keys that are up and those that
	// are down
	
	// maybe the best solution here is to create a vector of strings 
	// (or sf keyboard types, then let the receiving object take a look at them
	// and deal with them how it sees fit
	
	// yes, yes, I like that. The boolean wasnt really used anyways
	
	// The Enter or Return key
	if(input_event == sf::Keyboard::Return)
	{	icommands->enter = key_down;
		if(key_down == true)
		{	icommands->Enter();
		}
	}
	// Spacebar, space oddity, space pressme
	else if(input_event == sf::Keyboard::Space)
	{	icommands->space = key_down;
		if(key_down == true)
		{	icommands->Space();
		}
	}
	// The Arrow Keys
	else if(input_event == sf::Keyboard::Up)
	{	icommands->up = key_down;
		if(key_down == true)
		{	icommands->Up();
		}
	}
	else if(input_event == sf::Keyboard::Down)
	{	icommands->down = key_down;
		if(key_down == true)
		{	icommands->Down();
		}
	}
	else if(input_event == sf::Keyboard::Right)
	{	icommands->right = key_down;
		if(key_down == true)
		{	icommands->Right();
		}
	}
	else if(input_event == sf::Keyboard::Left)
	{	icommands->left = key_down;
		if(key_down == true)
		{	icommands->Left();
		}
	}
	// Tab Key
	else if(input_event == sf::Keyboard::Tab)
	{
	}
	// Tilde Key
	else if(input_event == sf::Keyboard::Tilde)
	{	icommands->tilde = key_down;
		if(key_down == true)
		{	icommands->Tilde();
		}
	}	
	// Alphabetical keys A-Z
	else if(input_event == sf::Keyboard::A)
	{	icommands->a = key_down;
		if(key_down == true)
		{	icommands->A();
		}
	}
	else if(input_event == sf::Keyboard::B)
	{
	}
	else if(input_event == sf::Keyboard::C)
	{	icommands->c = key_down;
		if(key_down == true)
		{	icommands->C();
		}
	}
	else if(input_event == sf::Keyboard::D)
	{	icommands->d = key_down;
		if(key_down == true)
		{	icommands->D();
		}
	}
	else if(input_event == sf::Keyboard::E)
	{	icommands->e = key_down;
		if(key_down == true)
		{	icommands->E();
		}
	}
	else if(input_event == sf::Keyboard::F)
	{	icommands->f = key_down;
		if(key_down == true)
		{	icommands->F();
		}
	}
	//else if(input_event == sf::Keyboard::G)
	//{
	//}
	else if(input_event == sf::Keyboard::H)
	{
	}
	else if(input_event == sf::Keyboard::I)
	{	icommands->i = key_down;
		if(key_down == true)
		{	icommands->I();
		}
	}
	else if(input_event == sf::Keyboard::J)
	{
	}
	else if(input_event == sf::Keyboard::K)
	{
	}
	else if(input_event == sf::Keyboard::L)
	{	icommands->l = key_down;
		if(key_down == true)
		{	icommands->L();
		}
	}
	else if(input_event == sf::Keyboard::M)
	{
	}
	else if(input_event == sf::Keyboard::N)
	{	icommands->n = key_down;
		if(key_down == true)
		{	icommands->N();
		}
	}
	else if(input_event == sf::Keyboard::O)
	{
	}
	else if(input_event == sf::Keyboard::P)
	{
	}
	else if(input_event == sf::Keyboard::Q)
	{	icommands->q = key_down;
		if(key_down == true)
		{	icommands->Q();
		}
	}
	else if(input_event == sf::Keyboard::R)
	{	icommands->r = key_down;
		if(key_down == true)
		{	icommands->R();
		}
	}
	else if(input_event == sf::Keyboard::S)
	{	icommands->s = key_down;
		if(key_down == true)
		{	icommands->S();
		}
	}
	else if(input_event == sf::Keyboard::T)
	{	icommands->t = key_down;
		if(key_down == true)
		{	icommands->T();
		}
	}
	else if(input_event == sf::Keyboard::U)
	{
	}
	else if(input_event == sf::Keyboard::V)
	{	icommands->v = key_down;
		if(key_down == true)
		{	icommands->V();
		}
	}
	else if(input_event == sf::Keyboard::W)
	{	icommands->w = key_down;
		if(key_down == true)
		{	icommands->W();
		}
	}
	else if(input_event == sf::Keyboard::X)
	{	icommands->x = key_down;
		if(key_down == true)
		{	icommands->X();
		}
	}
	else if(input_event == sf::Keyboard::Y)
	{
	}
	else if(input_event == sf::Keyboard::Z)
	{	icommands->z = key_down;
		if(key_down == true)
		{	icommands->Z();
		}
	}	// Assorted keys, without any real category
	else if(input_event == sf::Keyboard::Comma)
	{	icommands->comma = key_down;
		if(key_down == true)
		{	icommands->Comma();
		}
	}
	else if(input_event == sf::Keyboard::Dash)
	{	icommands->dash = key_down;
		if(key_down == true)
		{	icommands->Dash();
		}
	}
	else if(input_event == sf::Keyboard::Delete)
	{
	}
	else if(input_event == sf::Keyboard::Divide)
	{
	}
	else if(input_event == sf::Keyboard::BackSlash)
	{
	}
	else if(input_event == sf::Keyboard::BackSpace)
	{	
	}
	else if(input_event == sf::Keyboard::Equal)
	{	icommands->equal = key_down;
		if(key_down == true)
		{	icommands->Equal();
		}
	}
	else if(input_event == sf::Keyboard::Escape)
	{
	}
	else if(input_event == sf::Keyboard::LAlt)
	{
	}
	else if(input_event == sf::Keyboard::LBracket)
	{
	}
	else if(input_event == sf::Keyboard::RBracket)
	{
	}
	else if(input_event == sf::Keyboard::LControl)
	{
	}
	else if(input_event == sf::Keyboard::RControl)
	{
	}
	else if(input_event == sf::Keyboard::LShift)
	{
	}
	else if(input_event == sf::Keyboard::RShift)
	{
	}
	else if(input_event == sf::Keyboard::Menu)
	{
	}
	else if(input_event == sf::Keyboard::Multiply)
	{
	}
	else if(input_event == sf::Keyboard::Period)
	{	icommands->period = key_down;
		if(key_down == true)
		{	icommands->Period();
		}
	}
	else if(input_event == sf::Keyboard::Quote)
	{
	}
	else if(input_event == sf::Keyboard::Slash)
	{
	}	// Pageup/Pagedown
	else if(input_event == sf::Keyboard::PageDown)
	{
	}
	else if(input_event == sf::Keyboard::PageUp)
	{
	}	// Windows/OSX/Super key whatever you wanna call it
	else if(input_event == sf::Keyboard::LSystem)
	{
	}
	else if(input_event == sf::Keyboard::RSystem)
	{
	}	// Numeric inputs (row beneath the F## keys)
	else if(input_event == sf::Keyboard::Num0)
	{	
	}
	else if(input_event == sf::Keyboard::Num1)
	{
	}
	else if(input_event == sf::Keyboard::Num2)
	{
	}
	else if(input_event == sf::Keyboard::Num3)
	{
	}
	else if(input_event == sf::Keyboard::Num4)
	{
	}
	else if(input_event == sf::Keyboard::Num5)
	{
	}
	else if(input_event == sf::Keyboard::Num6)
	{
	}
	else if(input_event == sf::Keyboard::Num7)
	{
	}
	else if(input_event == sf::Keyboard::Num8)
	{
	}
	else if(input_event == sf::Keyboard::Num9)
	{
	}	// The Numpad. Beware Vaughan!!!
	else if(input_event == sf::Keyboard::Numpad0)
	{	icommands->numpad0 = key_down;
		if(key_down == true)
		{	std::cout << "numpad0 keydown" << std::endl;
			icommands->Numpad_0();
		}
	}
	else if(input_event == sf::Keyboard::Numpad1)
	{	std::cout << "numpad1 keyevent" << std::endl;
		icommands->numpad1 = key_down;
		if(key_down == true)
		{	icommands->Numpad_1();
		}
	}
	else if(input_event == sf::Keyboard::Numpad2)
	{	std::cout << "numpad1 keyevent" << std::endl;
		icommands->numpad2 = key_down;
		if(key_down == true)
		{	icommands->Numpad_2();
		}
	}
	else if(input_event == sf::Keyboard::Numpad3)
	{	icommands->numpad3 = key_down;
		if(key_down == true)
		{	icommands->Numpad_3();
		}
	}
	else if(input_event == sf::Keyboard::Numpad4)
	{	icommands->numpad4 = key_down;
		if(key_down == true)
		{	icommands->Numpad_4();
		}
	}
	else if(input_event == sf::Keyboard::Numpad5)
	{	icommands->numpad5 = key_down;
		if(key_down == true)
		{	icommands->Numpad_5();
		}
	}
	else if(input_event == sf::Keyboard::Numpad6)
	{	icommands->numpad6 = key_down;
		if(key_down == true)
		{	icommands->Numpad_6();
		}
	}
	else if(input_event == sf::Keyboard::Numpad7)
	{	icommands->numpad7 = key_down;
		if(key_down == true)
		{	icommands->Numpad_7();
		}
	}
	else if(input_event == sf::Keyboard::Numpad8)
	{	icommands->numpad8 = key_down;
		if(key_down == true)
		{	icommands->Numpad_8();
		}
	}
	else if(input_event == sf::Keyboard::Numpad9)
	{	icommands->numpad9 = key_down;
		if(key_down == true)
		{	icommands->Numpad_9();
		}
	}
	else if(input_event == sf::Keyboard::Add)
	{	icommands->plus = key_down;
		if(key_down == true)
		{	icommands->Plus();
		}
	}		// must add minus
	else if(input_event == sf::Keyboard::Subtract)
	{	icommands->minus = key_down;
		if(key_down == true)
		{	icommands->Minus();
		}
	}
}


// SFML_Window Utility class ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// a really important part of the engine, because it stores data used throughout
// basically just a plain old wrapper for sf::RenderWindow, but it does store
// important info about the way that the window is related to the in game
// universe

SFML_Window::SFML_Window(std::string title, unsigned int h, unsigned int w)
{	Title = title;
	Height = h;
	Width = w;
	Aperture_height = h;
	Aperture_width = w;
	// no real rhyme or reason behind how big the aperture should be at start
	// so we just give it the same as the window itself
	
	
	// this was the reason why everything appeared 10x too big
	Set_origin();
	// make sure it has a value
	window = new sf::RenderWindow(sf::VideoMode(w, h), Title);
	// set up our RenderWindow
	window->setSize(sf::Vector2u(w, h));
	// and size it, but.. we dont need to here, cause the videomode already
	// did it.
	
	// I think this can be removed
	window->setTitle(Title);
	// again think its redundant, but its a rarely used constructor, so not too
	// concerning really
}

SFML_Window::SFML_Window(std::string title, unsigned int h, unsigned int w, long double initial_rotation)
{	Title = title;
	Height = h;
	Width = w;
	Aperture_height = h;
	Aperture_width = w;
	// no real rhyme or reason behind how big the aperture should be at start
	// so we just give it the same as the window itself
	
	
	// this was the reason why everything appeared 10x too big
	Set_origin();
	// make sure it has a value
	window = new sf::RenderWindow(sf::VideoMode(w, h), Title);
	// set up our RenderWindow
	window->setSize(sf::Vector2u(w, h));
	// and size it, but.. we dont need to here, cause the videomode already
	// did it.
	
	// I think this can be removed
	window->setTitle(Title);
	// again think its redundant, but its a rarely used constructor, so not too
	// concerning really
	this->Set_aperture_rotation(initial_rotation);
}

void SFML_Window::Set_origin()
{	origin.x = 0;
	origin.y = 0;
	// If we dont know, we just say (0,0)
}

void SFML_Window::Set_origin(long double x, long double y)
{	origin.x = x;
	origin.y = y;
	// set our VectorVictor2 to the right value
}

void SFML_Window::Set_aperture_rotation(long double new_rotation)
{	Aperture_rotation = new_rotation;
}

void SFML_Window::Set_aperture_dimensions(long long unsigned int ap_w, long long unsigned int ap_h)
{	Aperture_width = ap_w;
	Aperture_height = ap_h;
	// same deal, now just for the aperture dimensions
}

bool SFML_Window::Intersection(VectorVictor::Vector2 center, long double radius, long double cam_scale)
{	// this *works* for now, but I very strongly am considering just redoing it
	// as a circle with radius of the sum of the squares of the window axis.
	// This is efficient for small camera views, but could prove really bad at
	// large camera views...
	
	// I dunno, but the current thing is a mess, cant just leave it like this
	
	center -= this->origin;
	// get the relative offset of the center of the circle from the window
	// origin 
	center.Rotate_vector(-(this->Aperture_rotation));
	// rotate things around so that the window is straight up and down relative
	// to our coordinate system
	center.y *= -1;
	// now case statements, depending on the dimensions of the center vector
	// at this point
	if(center.x < 0)
	{	if(center.x < -radius)
		{	return false;
		}
		else
		{	if((center.y >= 0)&&(center.y <= Aperture_height))
			{	return true;
			}
			else
			{	// the long method for checking if we intersect the edge anyways
				
				// ehh screw it, why not do it right
				
				if(center.y > this->Aperture_height)
				{	center.y -= this->Aperture_height;
				}
				long double r_squared = (center.y*center.y) + (center.x*center.x);
				if(r_squared <= (radius*radius))
				{	// compare the squares so that we dont have to invoke
					// pythagoras
					return true;
				}
				else
				{	return false;
				}
			}
		}
	}
	else if((center.x >= 0)&&(center.x <= this->Aperture_width))
	{	// the middle case, maybe easier
		if(center.y > 0)
		{	long double v_offset = center.y - this->Aperture_height;
			if(v_offset > radius)
			{	return false;
			}
			else
			{	return true;
			}
		}
		else
		{	// center.y is negative, so the center of the circle is above the
			// box
			if(center.y > radius)
			{	return false;
			}
			else
			{	return true;
			}
		}
	}
	else
	{	// positive and beyond the edge of the aperture
		if(center.x > (radius + this->Aperture_width))
		{	return false;
			// too far away by definition, so false
		}
		else
		{	// we already know from the above middle section check that this
			// case must have an origin that is less than radius from the
			// aperture width along the x axis
			if((center.y >= 0)&&(center.y <= this->Aperture_height))
			{	// identical check to the negative side of the box
				return true;
			}
			else
			{	// the long method for checking if we intersect the edge anyways
				
				// ehh screw it, why not do it right
				long double dx = center.x - this->Aperture_width;
				if(center.y > this->Aperture_height)
				{	center.y -= this->Aperture_height;
				}				
				long double r_squared = (center.y*center.y) + (dx*dx);
				if(r_squared <= (radius*radius))
				{	// compare the squares so that we dont have to invoke
					// pythagoras
					return true;
				}
				else
				{	return false;
				}
			}
		}
	}
	// that was a beast
	
	// heres hoping its more efficient than it looks...
}

bool SFML_Window::Window_intersection(sf::Vector2f window_point)
{	if((window_point.x >= 0)&&(window_point.y >= 0))
	{	// we check and make sure that our point has at least positive
		// coordinate values
		if((window_point.x <= (float)this->Width)&&(window_point.y <= (float)this->Height))
		{	// and we check that the point is within the width and height of the
			// window in pixels away from the (0,0)
			return true;
		}
		else
		{	return false;
		}
	}
	else
	{	return false;
	}
}

bool SFML_Window::Intersection(VectorVictor::Vector2 point, long double cam_scale)
{	// implicitly in camera view based on the double being passed
	sf::Vector2f window_offset = Get_window_coordinates(point, this,cam_scale);
	// get the relative offset of the given point relative to the window given
	// the stuff we passed
	return this->Window_intersection(window_offset);
	// and return whatever Window_intersection thinks of that...
}

bool SFML_Window::Intersection(VectorVictor::Vector2 point, int zoom_factor)
{	// implicitly in map view based on the int being passed
	sf::Vector2f window_offset = Get_window_coordinates(point, this, zoom_factor);
	std::cout << "Window coordinates of point: " << window_offset.x << "," << window_offset.y << std::endl;
	// get the relative offset of the given point relative to the window given
	// the stuff we passed
	return this->Window_intersection(window_offset);
	// and return whatever Window_intersection thinks of that...
}

SFML_Window::~SFML_Window()
{	delete window;
	//	cant forget this ;D
}

// Utility functions to transform between window relative coordinates to ///////
// ignition universe coordinates and back again ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


sf::Vector2f Get_window_coordinates(VectorVictor::Vector2 sim_point, SFML_Window * iwindow, int map_scale)
{	VectorVictor::Vector2 camera_origin(iwindow->origin.Get_x(), iwindow->origin.Get_y());
	sim_point -= camera_origin;
	// get the relative offset from the camera origin to
	// the point in simulation as a VV2
	sim_point.Rotate_vector(-iwindow->Aperture_rotation);
	// rotate the vector around into the reference frame of
	// the window box, now straight up and down relative to
	// the global coordinate frame axes
	sim_point.y *= -1;
	// flip the y axis because SFML does things that way
	sim_point.y *= pow(0.1, (map_scale));
	sim_point.x *= pow(0.1, (map_scale));
	// scale back the length of the vector by 10^map_scale
	// so that its dimensions are in pixels instead of
	// meters
	sf::Vector2f camera_offset(sim_point.x, sim_point.y);
	// put the vector into an sfml 2f type so we can return
	// it properly
	return camera_offset;
}

sf::Vector2f Get_window_coordinates(VectorVictor::Vector2 sim_point, SFML_Window * iwindow, long double cam_scale)
{	
	
	
	VectorVictor::Vector2 camera_origin(iwindow->origin.Get_x(), iwindow->origin.Get_y());
	sim_point -= camera_origin;
	// get the relative offset of the point from the window
	// origin
	sim_point.Rotate_vector(-iwindow->Aperture_rotation);
	// rotate the vector so that the window is straight up and down
	sim_point.y *= -1;
	// flip the y axis so that SFML is happy
	sim_point.x *= (10.00000000/(long double)cam_scale);
	sim_point.y *= (10.00000000/(long double)cam_scale);
	// of course! Because at cam_scale = 1 (or default), the window has
	// dimensions 10x smaller in meters than its actual window dimensions
	// in pixels, ie dimensions of 102.4 x 60.9 meters for a window with
	// size of 1024 x 609. So thats why we have to scale it up by 10x
	
	sf::Vector2f camera_offset(sim_point.x, sim_point.y);
	// convert into an sf 2f for return
	return camera_offset;
}

VectorVictor::Vector2 Get_simulation_coordinates(sf::Vector2f window_point, SFML_Window * iwindow, int map_scale)
{	VectorVictor::Vector2 sim_point(window_point.x, window_point.y);
	// so we start with our relative offset of the point in the window
	// in pixels
	sim_point.y *= pow((long double)10, (map_scale));
	sim_point.x *= pow((long double)10, (map_scale));
	// and we scale it up by the zoom factor so we get the relative offset
	// of the point from the camera origin in universe coordinates
	sim_point.y *= -1;
	// we flip the y axis, since the SFML coordinates will be mirrored
	// vertically compared to the VV2 coordinate system that the engine
	// uses 
	
	sim_point.Rotate_vector(iwindow->Aperture_rotation);
	
	sim_point += iwindow->origin;
	// we add the main windows origin point to transform the point to the sim
	// sessions global origin point (0,0)
	return sim_point;
	// actually wasnt that complex I guess, just the previous operations in
	// reverse
}

VectorVictor::Vector2 Get_simulation_coordinates(sf::Vector2f window_point, SFML_Window * iwindow, double cam_scale)
{	VectorVictor::Vector2 sim_point(window_point.x, window_point.y);
	
	sim_point.x /= (10.00000000/(long double)cam_scale);
	sim_point.y /= (10.00000000/(long double)cam_scale);
	// here we now shrink it back, since the pixel coordinates are bigger
	// than the actual universe coordinates by a factor of 10/cam_scale
	sim_point.y *= -1;
	// again, flip the y axis because SFML has down as positive y, for reasons
	// that are its own
	sim_point.Rotate_vector(iwindow->Aperture_rotation);
	// rotate the vector back into the correct orientation of the window
	
	sim_point += iwindow->origin;
	// we add the main windows origin point to transform the point to the sim
	// sessions global origin point (0,0)
	return sim_point;
	// and we return the vector point now in universe coordinates
}

// gonna probably need to write at least one of these from scratch methinks






// Ignition Drawables //////////////////////////////////////////////////////////
// wrapper classes for stuff that needs to be drawn to the window like text ////
// displays & other infographics that the vessel instance draws as ui //////////
// displays and things like that ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



void Ignition_drawable::Draw_element(SFML_Window * iwindow)
{	Talkback("Bad call to Ignition_drawable::Draw_element(SFML_Window * iwindow)");
}

void Ignition_drawable::Center_element()
{	Talkback("Ignition_drawable::Center_element()");
}

// Ignition text type //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Ignition_text::Ignition_text(std::string text_font, sf::Vector2f initial_position, std::string initial_text, sf::Color initial_colour, unsigned int character_size, bool center_origin)
{	this->Init_object(text_font, initial_position, initial_text, initial_colour, character_size, center_origin);
}

Ignition_text::Ignition_text(sf::Font text_font, sf::Vector2f initial_position, std::string initial_text, sf::Color initial_colour, unsigned int character_size, bool center_origin)
{	this->Init_object(text_font, initial_position, initial_text, initial_colour, character_size, center_origin);
}

Ignition_text::Ignition_text()
{	// uhhh.
	// just try not to call this very often
}

bool Ignition_text::Init_object(std::string text_font, sf::Vector2f initial_position, std::string initial_text, sf::Color initial_colour, unsigned int character_size, bool center_origin)
{	font.loadFromFile(text_font);
	text.setFont(font);
	text.setPosition(initial_position);
	text.setString(initial_text);
	text.setColor(initial_colour);
	text.setCharacterSize(character_size);
	if(center_origin == true)
	{	this->Center_element();
	}
	return true;
	// just to let everybody know that nothing went wrong
}

bool Ignition_text::Init_object(sf::Font text_font, sf::Vector2f initial_position, std::string initial_text, sf::Color initial_colour, unsigned int character_size, bool center_origin)
{	font = text_font;
	// hopefully this works, we might have to pass by reference if not
	text.setFont(font);
	text.setPosition(initial_position);
	text.setString(initial_text);
	text.setColor(initial_colour);
	text.setCharacterSize(character_size);
	if(center_origin == true)
	{	this->Center_element();
	}
	return true;
	// just to let everybody know that nothing went wrong
}


void Ignition_text::Set_element(std::string text_string)
{	text.setString(text_string);
}

void Ignition_text::Set_element(sf::Vector2f new_position)
{	text.setPosition(new_position);
}

void Ignition_text::Set_element(sf::Color new_colour)
{	text.setColor(new_colour);
}

void Ignition_text::Set_element(unsigned int character_size)
{	text.setCharacterSize(character_size);
}

void Ignition_text::Set_element(std::string text_string, sf::Vector2f new_position)
{	this->Set_element(text_string);
	this->Set_element(new_position);		
}

void Ignition_text::Set_element(std::string text_string, sf::Vector2f new_position, sf::Color new_colour)
{	this->Set_element(text_string);
	this->Set_element(new_position);
	this->Set_element(new_colour);
}

void Ignition_text::Set_element(std::string text_string, sf::Color new_colour)
{	this->Set_element(text_string);
	this->Set_element(new_colour);
}

void Ignition_text::Set_element(sf::Vector2f new_position, sf::Color new_colour)
{	this->Set_element(new_position);
	this->Set_element(new_colour);
}

void Ignition_text::Draw_element(SFML_Window * iwindow)
{	iwindow->window->draw(text);
}

void Ignition_text::Center_element()
{	if(Centered == false)
	{	// do some centering
		// not sure how with sf::Text really
		// I mean what are the dimensions of the text strip?
		
		// important question to get answered
		Centered = true;
	}
}
	
Ignition_text::~Ignition_text()
{
}


// Ignition Circle /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


Ignition_circle::Ignition_circle(sf::Vector2f initial_position, sf::Color initial_colour, float initial_radius, bool center_origin)
{	this->Init_object(initial_position, initial_colour, initial_radius, center_origin);
}

Ignition_circle::Ignition_circle()
{
}

bool Ignition_circle::Init_object(sf::Vector2f initial_position, sf::Color initial_colour, float initial_radius, bool center_origin)
{	circle_shape.setPosition(initial_position);
	circle_shape.setFillColor(initial_colour);
	circle_shape.setRadius(initial_radius);
	if(center_origin == true)
	{	this->Center_element();
	}
	return true;
}

void Ignition_circle::Set_element(float new_radius)
{	circle_shape.setRadius(new_radius);
}

void Ignition_circle::Set_element(sf::Vector2f new_position)
{	circle_shape.setPosition(new_position);
}

void Ignition_circle::Set_element(sf::Color new_colour)
{	circle_shape.setFillColor(new_colour);
}

void Ignition_circle::Set_element(float new_radius, sf::Vector2f new_position)
{	this->Set_element(new_radius);
	this->Set_element(new_position);
}

void Ignition_circle::Set_element(float new_radius, sf::Vector2f new_position, sf::Color new_colour)
{	this->Set_element(new_radius);
	this->Set_element(new_position);
	this->Set_element(new_colour);
}

void Ignition_circle::Set_element(float new_radius, sf::Color new_colour)
{	this->Set_element(new_radius);
	this->Set_element(new_colour);
}

void Ignition_circle::Set_element(sf::Vector2f new_position, sf::Color new_colour)
{	this->Set_element(new_position);
	this->Set_element(new_colour);
}

void Ignition_circle::Draw_element(SFML_Window * iwindow)
{	iwindow->window->draw(circle_shape);
}

void Ignition_circle::Center_element()
{	if(Centered == false)
	{	// do some centering stuffz
		circle_shape.setOrigin(sf::Vector2f(circle_shape.getRadius(), circle_shape.getRadius()));
		Centered = true;
	}
}

Ignition_circle::~Ignition_circle()
{	// no pointers, no problems
}


// Ignition rectangle //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Ignition_rectangle::Ignition_rectangle(sf::Vector2f initial_position, sf::Color initial_colour, float width, float height, bool center_origin)
{	this->Init_object(initial_position, initial_colour, width, height, center_origin);
}

Ignition_rectangle::Ignition_rectangle()
{
}

bool Ignition_rectangle::Init_object(sf::Vector2f initial_position, sf::Color initial_colour, float width, float height, bool center_origin)
{	rect_shape.setPosition(initial_position);
	rect_shape.setFillColor(initial_colour);
	rect_shape.setSize(sf::Vector2f(width, height));
	if(center_origin == true)
	{	Center_element();
	}
	return true;
}

void Ignition_rectangle::Set_element(float new_width, float new_height)
{	rect_shape.setSize(sf::Vector2f(new_width, new_height));
}

void Ignition_rectangle::Set_element(sf::Vector2f new_position)
{	rect_shape.setPosition(new_position);
}

void Ignition_rectangle::Set_element(sf::Color new_colour)
{	rect_shape.setFillColor(new_colour);
}

void Ignition_rectangle::Set_element(float new_width, float new_height, sf::Vector2f new_position)
{	this->Set_element(new_width, new_height);
	this->Set_element(new_position);	
}

void Ignition_rectangle::Set_element(float new_width, float new_height, sf::Vector2f new_position, sf::Color new_colour)
{	this->Set_element(new_width, new_height);
	this->Set_element(new_position);
	this->Set_element(new_colour);
}

void Ignition_rectangle::Set_element(float new_width, float new_height, sf::Color new_colour)
{	this->Set_element(new_width, new_height);
	this->Set_element(new_colour);
}

void Ignition_rectangle::Set_element(sf::Vector2f new_position, sf::Color new_colour)
{	this->Set_element(new_position);
	this->Set_element(new_colour);
}	
	
void Ignition_rectangle::Draw_element(SFML_Window * iwindow)
{	iwindow->window->draw(rect_shape);
}

void Ignition_rectangle::Center_element()
{	if(Centered == false)
	{	rect_shape.setOrigin(sf::Vector2f((rect_shape.getSize().x)/2, (rect_shape.getSize().y)/2));
		// looks good
		Centered = true;
	}
}

Ignition_rectangle::~Ignition_rectangle()
{
}

// Ignition sprite class ///////////////////////////////////////////////////////
// just easier to wrap it and force it into the shape I want it in /////////////
////////////////////////////////////////////////////////////////////////////////

Ignition_sprite::Ignition_sprite(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, std::string texture_path)
{	this->Init_object(initial_position, initial_colour, center_origin, texture_path);
}

Ignition_sprite::Ignition_sprite(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, sf::Texture texture, bool copy_texture)
{	this->Init_object(initial_position, initial_colour, center_origin, texture, copy_texture);
}

Ignition_sprite::Ignition_sprite()
{
}

	
bool Ignition_sprite::Init_object(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, std::string texture_path)
{	
	if(!Tex.loadFromFile(texture_path))
	{	return false;
	}
	sprite.setTexture(Tex);
	sprite.setPosition(initial_position);
	// set the position of the sprite in the window
	
	// ehh, just dont worry about the image thing just yet
	if(center_origin == true)
	{	this->Center_element();
	}
	return true;
}

bool Ignition_sprite::Init_object(sf::Vector2f initial_position, sf::Color initial_colour, bool center_origin, sf::Texture &texture, bool copy_texture)
{	if(copy_texture == true)
	{	Tex = texture;
		// just make the internally stored texture into a clone of the one we
		// received.
		sprite.setTexture(Tex);
	}
	else
	{	sprite.setTexture(texture);
		// set the sprite directly off of the texture that we were passed.
		// If you do it this way, the sf::Texture had better survive for the
		// lifetime of the Ignition_sprite or you will have problems
	}
	sprite.setPosition(initial_position);
	// set the position of the sprite in the window
	
	// ehh, just dont worry about the image thing just yet
	if(center_origin == true)
	{	this->Center_element();
	}
	return true;	
}
	
void Ignition_sprite::Set_element(sf::Vector2f new_position)
{	sprite.setPosition(new_position);
}

void Ignition_sprite::Set_element(sf::Vector2f new_position, sf::Color new_colour)
{	sprite.setPosition(new_position);
}

void Ignition_sprite::Set_element(sf::Color new_colour)
{	// uhhh, I dont wanna deal with this right now
}

void Ignition_sprite::Center_element()
{	if(Centered == false)
	{	sprite.setOrigin(sf::Vector2f(((sprite.getTexture()->getSize().x)/2), ((sprite.getTexture()->getSize().y)/2)));
		// looks good
		Centered = true;
	}
}

void Ignition_sprite::Draw_element(SFML_Window * iwindow)
{	iwindow->window->draw(sprite);
}

Ignition_sprite::~Ignition_sprite()
{
}





// SFML splash screen class ////////////////////////////////////////////////////

// and everything after this point is an absolute mess

// avert your eyes ><

SFML_loadscreen::SFML_loadscreen(std::string texture_path, bool fadein, double fade_in_length, double fade_out_length)
{	Active = true;
	splash_texture = new sf::Texture();
	if(!splash_texture->loadFromFile(texture_path))
	{	this->~SFML_loadscreen();
	}
	else
	{	splash_sprite = new sf::Sprite();
		splash_sprite->setTexture(*splash_texture);
	}	Fader = 0;
	Fading_out = false;
	Fade_clock = new sf::Clock();
	if(fadein == true)
	{	Fade_in();
	}
	else
	{	Fading_in = false;
	}	Fade_in_time = fade_in_length;
	Fade_out_time = fade_out_length;
	splash_sprite->setColor(sf::Color(255, 255, 255, ((Fader)*255)));
}

void SFML_loadscreen::Fade_in()
{	Fading_in = true;
	Fade_clock->restart();
}

void SFML_loadscreen::Fade_out()
{	Fading_out = true;
	Fade_clock->restart();
}

void SFML_loadscreen::Toggle_fade()
{	if(Fading_in == true)
	{	if((Fading_out == false)&&(Fader != 1.000))
		{	std::cout << "Toggle Fadein" << std::endl;
			Fade_in();
		}
		else
		{	Fade_out();
			std::cout << "Toggle Fadeout" << std::endl;
		}
	}
}

void SFML_loadscreen::Update_screen()
{	if(((Fading_in == true)&&(Fader != 1.000))||((Fading_out == true)||(Fader != 0.000)))
	{	sf::Time time = Fade_clock->getElapsedTime();
		if(time.asMilliseconds() > 100)
		{	double increment = time.asMilliseconds();
			if((Fading_in == true)&&(Fading_out == false))
			{	Fader += (increment/(Fade_in_time*1000));
				if(Fader > 1.0000)
				{	Fader = 1.0000;
				}
			}
			else if((Fading_in == true)&&(Fading_out == true))
			{	Fader -= (increment/(Fade_in_time*1000));
				if(Fader < 0.0000)
				{	Fader = 0.0000;
				}
			}	splash_sprite->setColor(sf::Color(255, 255, 255, ((Fader)*255)));
			Fade_clock->restart();
		}
	}
	if((Fader == 0.000)&&(Fading_in == true)&&(Fading_out == true)&&(Active == true))
	{	std::cout << "Loadscreen Dropout" << std::endl;
		Active = false;
	}
}

SFML_loadscreen::~SFML_loadscreen()
{	delete splash_sprite;
	delete splash_texture;
	delete Fade_clock;
}

// SFML title screen class /////////////////////////////////////////////////////

SFML_titlescreen::SFML_titlescreen(std::string texture_path, bool fadein, double fade_in_length, double fade_out_length, std::string title, std::string font_path, unsigned int textr, unsigned int textg, unsigned int textb, unsigned int pfont, sf::Vector2f Title_pos)
{	Active = true;
	splash_texture = new sf::Texture();
	splash_texture->setSmooth(true);
	splash_sprite = new sf::Sprite();
	if(!splash_texture->loadFromFile(texture_path))
	{	this->~SFML_titlescreen();
	}
	else
	{	splash_sprite->setTexture(*splash_texture);
	}	Fader = 0;
	Fading_out = false;
	Fade_clock = new sf::Clock();
	if(fadein == true)
	{	Fade_in();
	}
	else
	{	Fading_in = false;
	}	Fade_in_time = fade_in_length;
	Fade_out_time = fade_out_length;
	splash_sprite->setColor(sf::Color(255, 255, 255, 0));
	Title_font = new sf::Font();
	if(!Title_font->loadFromFile(font_path))
	{	this->~SFML_titlescreen();
	}
	else
	{	Title_text = new sf::Text(title, *Title_font, pfont);
		Title_text->setColor(sf::Color(textr, textg, textb, 0));
		r = textr;
		g = textg;
		b = textb;
	}	Title_position = Title_pos;
	Title_text->setPosition(Title_position);
	finished = false;
}

void SFML_titlescreen::Fade_in()
{	Fading_in = true;
	Fade_clock->restart();
}

void SFML_titlescreen::Fade_out()
{	Fading_out = true;
	Fade_clock->restart();
}

void SFML_titlescreen::Toggle_fade()
{	if(Fading_in == true)
	{	if((Fading_out == false)&&(Fader != 1.000))
		{	std::cout << "Toggle Fadein" << std::endl;
			Fade_in();
		}
		else
		{	Fade_out();
			std::cout << "Toggle Fadeout" << std::endl;
		}
	}
}

void SFML_titlescreen::Update_screen()
{	if(((Fading_in == true)&&(Fader != 1.000))||((Fading_out == true)||(Fader != 0.000)))
	{	sf::Time time = Fade_clock->getElapsedTime();
		if(time.asMilliseconds() > 100)
		{	double increment = time.asMilliseconds();
			if((Fading_in == true)&&(Fading_out == false))
			{	Fader += (increment/(Fade_in_time*1000));
				if(Fader > 1.0000)
				{	Fader = 1.0000;
				}
			}
			else if((Fading_in == true)&&(Fading_out == true))
			{	Fader -= (increment/(Fade_in_time*1000));
				if(Fader < 0.0000)
				{	Fader = 0.0000;
					finished = true;
					//Deallocate_me();
				}
			}	splash_sprite->setColor(sf::Color(255, 255, 255, ((Fader)*255)));
			Title_text->setColor(sf::Color(r, g, b, Fader*(255)));
			Fade_clock->restart();
		}
	}
	if((Fader == 0.000)&&(Fading_in == true)&&(Fading_out == true)&&(Active == true))
	{	std::cout << "Loadscreen Dropout" << std::endl;
		Active = false;
	}
}
 



SFML_titlescreen::~SFML_titlescreen()
{	delete Title_text;
	delete Title_font;
	delete splash_sprite;
	delete splash_texture;
	delete Fade_clock;
}

// Utility function ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::string Input_string(std::string prompt)		// this should go, not
{	std::string data = "";	// a great idea in a project primarily focused
	while(true)				// on gui
	{	std::cout << std::endl;
		std::cout << prompt;
		while(std::getline(std::cin, data))
		{	if (data.size() == 0)
			{	continue;	
			}	
			else
			{	return data;
			}	
		}
	}
}
