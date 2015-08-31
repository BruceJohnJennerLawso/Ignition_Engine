# SFML_Tools.h ################################
# A couple of sfml wrappers that  automate a few useful things ########
########################################
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "VectorVictor2.hpp"

#ifndef SFMLTools
#define SFMLTools

def Input_string(self, prompt):

struct vector2
{	# old data type used by sfml tools before VV2 was available here
	# should be removed wherever possible for consistency
	
	# for all intents and purposes, is the exact same thing as VV2,
	# but an object oriented interface is a much better idea
	long double x, y;	


class Program_data
{	# not actually sure what self is, it was just a passing idea
	# that never actually got implemented
	Program_data()
	enum prog_state{Intro, Menu, Game}Game_status
	~Program_data()


# Mouse_commands class ############################
########################################

class Cursor_commands
{	public:
	Cursor_commands()
	sf.Vector2f position
	# the coordinates of the mouse relative to the window?
	
	# I dont actually know yet, self is kinda going in blind. We'll see how
	# sfml likes to handle things
	
	# my working assumption here is that it will be in pixels, the window origin
	bool Right_click, Left_click, Middle_click
	# if the given mouse button is down, True, False
	# dont want to mess about with extra buttons just yet
	
	void Set_cursor_state(float x, y, right, left, middle)
	~Cursor_commands()
	# surprisingly self whole section went off without a hitch. suspicious...


# Key_commands class #############################
########################################

class key_commands
{	public:
	key_commands()
	# simple constructor, params needed
	void Enter()
	void Space()
	void Period()
	void Comma()
	void Up()
	void Down()
	void Right()
	void Left()
	void Tilde()
	void Q()
	void W()
	void E()
	void A()
	void S()
	void D()
	void C()
	void N()
	void F()
	void I()
	void V()
	void Z()
	void X()
	void R()
	void T()
	void L()
	void Dash()
	void Equal()
	void Numpad_0()
	void Numpad_1()
	void Numpad_2()
	void Numpad_3()
	void Numpad_4()
	void Numpad_5()
	void Numpad_6()
	void Numpad_7()
	void Numpad_8()
	void Numpad_9()
	void Plus();				# oddly enough, hasnt caused any errors by not being defined
	void Minus()
	# all of the functions that get called when log keystroke indicates that
	# the key in question was pressed
	bool enter, space, period, comma, up, down, right, left, tilde, v, c, q, w, e, a, s, d, f, n,  i, l, x, z, t, r, dash, equal, numpad0, numpad1, numpad2, numpad3, numpad4, numpad5, numpad6, numpad7, numpad8, numpad9, plus, minus
	# and the booleans that are set to the key down status of the given key
	# in the same function
	~key_commands()
};	# important future idea for self is to retrieve mouse location & clickstate

def Log_keystroke(self, input_event, * icommands, key_down):
def Null_queue(self):
# I dont think self was ever defined. Probably safe to remove


# SFML Window ################################/
# Cause I keep getting lost every time I look for self ############
########################################

class SFML_Window
{	public:
	SFML_Window(std.string title, int h, int w)
	# implicit rotation of 0 in self case
	SFML_Window(std.string title, int h, int w, double initial_rotation)
	std.string Title
	# the string that gets printed on the menubar at the top of the window
	
	# might be removable if the sf.RenderWindow already contains a string
	# for self internally
	sf.RenderWindow * window
	# the pointer to the sfml window that the class wraps
	VectorVictor.Vector2 origin
	# the ingame origin (top left corner) of the windows aperture
	# (the area that the window covers)
	void Set_origin()
	# if not specified, it to (0,0)
	void Set_origin(long double x, double y)
	# or set the coordinates of the origin more explicitly
	long long unsigned int Width, Height
	# the dimensions of the actual window itself, pixels
	# I think self is unnecessary to have the long long here, was some
	# sort of desperate problem solving with the window jitter issue I think
	
	# wastin memoryz here
	long double Aperture_width, Aperture_height
	# the dimensions of what the window is currently spanning in the game
	# universe, meters
	long double Aperture_rotation
	# clockwise about the origin, degrees
	void Set_aperture_rotation(long double new_rotation)
	# set the rotation to its value
	void Set_aperture_dimensions(long long unsigned int ap_w, long unsigned int ap_h)
	# set those dimensions in meters
	bool Intersection(VectorVictor.Vector2 center, double radius, double cam_scale)
	# check if a sphere with center and radius intersects the aperture box of
	# the window, that we know if it should be drawn
	bool Window_intersection(sf.Vector2f window_point)
	
	bool Intersection(VectorVictor.Vector2 point, double cam_scale)
	# intersection of a given VV2 point with the window in camera view
	bool Intersection(VectorVictor.Vector2 point, zoom_factor)
	# intersection of a given VV2 with the window in map view	
	~SFML_Window()
	# close up shop (and delete our RenderWindow)


# helper functions here to automate transforming a sf.Vector2f location in #/
# a window with float precision to a VV2 with long double precision in the ##
# engines global coordinate system ######################
########################################
# there isnt anything really here, an automation of an existing ###
# process that a couple of functions implemented each in their own way ####

def Get_window_coordinates(self, sim_point, * iwindow, map_scale):

def Get_window_coordinates(self, sim_point, * iwindow, double cam_scale):



def Get_simulation_coordinates(self, window_point, * iwindow, map_scale):

def Get_simulation_coordinates(self, window_point, * iwindow, cam_scale):




# Generic displays ##############################
# This whole section is kinda hazy. Basically, idea is that we wrap ###/
# anything that is sf.Drawable into a wrapper class with some useful ####/
# methods so that working with it is much easier ###############
########################################

class Ignition_drawable
{	public:
	virtual void Draw_element(SFML_Window * iwindow)
	
	bool Centered
	virtual void Center_element()
	#std.vector<std.vector<*Ignition_drawable>> idrawable_pointers
	#std.vector<Ignition_handle<Ignition_drawable>
	#bool Clean_drawable_references()
	# self idea must wait for now


# next thing to implement is rotation, pretty much everything here is
# sf.Transformable

class Ignition_text: public Ignition_drawable
{	public:
	Ignition_text(std.string text_font, initial_position, initial_text, initial_colour, int character_size, center_origin)
	Ignition_text(sf.Font text_font, initial_position, initial_text, initial_colour, int character_size, center_origin);	
	Ignition_text()
	bool Init_object(std.string text_font, initial_position, initial_text, initial_colour, int character_size, center_origin)
	bool Init_object(sf.Font text_font, initial_position, initial_text, initial_colour, int character_size, center_origin);	
	
	# should receive a sf.Font by reference
	void Set_element(std.string text_string)
	void Set_element(sf.Vector2f new_position)
	void Set_element(sf.Color new_colour);	
	void Set_element(unsigned int character_size)
	#	size is in pixels, I dont know if self is height or width
	void Set_element(std.string text_string, new_position)
	void Set_element(std.string text_string, new_position, new_colour)
	void Set_element(std.string text_string, new_colour)
	void Set_element(sf.Vector2f new_position, new_colour)
	# 
	void Draw_element(SFML_Window * iwindow)
	void Center_element()
	sf.Font font
	sf.Text text
	
	~Ignition_text()
	# later on self destructor will use info stored about the stored references
	# to self object to clean up all of said references so that null pointers
	# arent laying around like landmines


class Ignition_circle: public Ignition_drawable
{	public:
	Ignition_circle(sf.Vector2f initial_position, initial_colour, initial_radius, center_origin)
	Ignition_circle()
	# later self will have a second constructor to texture it with a passed
	# sf.Texture reference
	bool Init_object(sf.Vector2f initial_position, initial_colour, initial_radius, center_origin)
	# the init call in case we dont have a direct call to the constructor
	void Set_element(float new_radius)
	void Set_element(sf.Vector2f new_position);	
	void Set_element(sf.Color new_colour);	
	void Set_element(float new_radius, new_position)
	void Set_element(float new_radius, new_position, new_colour)
	void Set_element(float new_radius, new_colour)
	void Set_element(sf.Vector2f new_position, new_colour);	
	# all of the given setters, directly to our sf.CircleShape,
	# instead of holding internal variables. There could be a lot of displays
	# in sim, we want to keep memory usage to a bare minimum
	void Draw_element(SFML_Window * iwindow)
	void Center_element()
	sf.CircleShape circle_shape
	# oddly, seems to automatically pick a point count for circles by
	# default, that the given circle looks halfway decent. oh well...
	~Ignition_circle()


class Ignition_rectangle: public Ignition_drawable
{	public:
	Ignition_rectangle(sf.Vector2f initial_position, initial_colour, width, height, center_origin)
	Ignition_rectangle()
	# later self will have a second constructor to texture it with a passed
	# sf.Texture reference
	bool Init_object(sf.Vector2f initial_position, initial_colour, width, height, center_origin)
	void Set_element(float new_width, new_height)
	void Set_element(sf.Vector2f new_position);	
	void Set_element(sf.Color new_colour);	
	void Set_element(float new_width, new_height, new_position)
	void Set_element(float new_width, new_height, new_position, new_colour)
	void Set_element(float new_width, new_height, new_colour)
	void Set_element(sf.Vector2f new_position, new_colour);	
	void Center_element()
	void Draw_element(SFML_Window * iwindow)
	sf.RectangleShape rect_shape
	~Ignition_rectangle()


class Ignition_sprite: public Ignition_drawable
{	public:
	Ignition_sprite(sf.Vector2f initial_position, initial_colour, center_origin, texture_path)
	Ignition_sprite(sf.Vector2f initial_position, initial_colour, center_origin, texture, copy_texture)
	Ignition_sprite()
	
	bool Init_object(sf.Vector2f initial_position, initial_colour, center_origin, texture_path)
	bool Init_object(sf.Vector2f initial_position, initial_colour, center_origin, &texture, copy_texture)
	
	void Set_element(sf.Vector2f new_position)
	void Set_element(sf.Vector2f new_position, new_colour)
	void Set_element(sf.Color new_colour)
	void Center_element()
	void Draw_element(SFML_Window * iwindow)
	
	sf.Texture Tex
	sf.Sprite sprite
	~Ignition_sprite()



#class Ignition_line: public Ignition_drawable
#{	public:
#	Ignition_line()
#	sf.VertexArray line
#	~Ignition_line();	
#

#class Ignition_triangle: public Ignition_drawable
#{	public:
#	Ignition_triangle()
#	sf.VertexArray triangle
#	# gotta find the appropriate setters here
#	~Ignition_triangle()
#	# a nice sf primitive vertex here
#

# here there be dragons, ones with sharp pointy teeth


class SFML_loadscreen
{	public:
	SFML_loadscreen(std.string texture_path, fadein, fade_in_length, fade_out_length)
	void Fade_in()
	void Fade_out()
	void Update_screen()
	void Toggle_fade()
	double Fader
	bool Fading_in, Fading_out, Active
	double Fade_in_time, Fade_out_time
	sf.Clock * Fade_clock
	sf.Texture * splash_texture
	sf.Sprite * splash_sprite
	~SFML_loadscreen()


class SFML_titlescreen
{	public:
	SFML_titlescreen(std.string texture_path, fadein, fade_in_length, fade_out_length, title, font_path, int textr, int textg, int textb, int pfont, Title_pos)
	void Fade_in()
	void Fade_out()
	void Update_screen()
	void Toggle_fade()
	double Fader
	bool Fading_in, Fading_out, Active
	double Fade_in_time, Fade_out_time
	unsigned int r, g, b
	bool finished
	sf.Text * Title_text
	sf.Font * Title_font
	sf.Vector2f Title_position
	sf.Clock * Fade_clock
	sf.Texture * splash_texture
	sf.Sprite * splash_sprite
	~SFML_titlescreen()


#endif
