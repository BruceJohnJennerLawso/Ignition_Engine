#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
##include<GameObjectsnot !not !>		Theyre coming, really arenot !!
#include <math.h>
#include "SFML_Tools.hpp"








Cursor_commands.Cursor_commands()
{	position.x = 0.00
	position.y = 0.00
	
	Right_click = False
	Left_click = False
	Middle_click = False


def Set_cursor_state(self, x, y, right, left, middle):
{	position.x = x
	position.y = y
	
	Right_click = right
	Left_click = left
	Middle_click = middle


Cursor_commands.~Cursor_commands()




# self whole class is horrible, there doesnt really seem to be any 
# alternatives that I can see at the moment. Its just too integral to how the
# engine handles inputs with regards to internal objects

key_commands.key_commands()
{	enter = False
	space = False
	period = False
	left = False
	right = False
	up = False
	down = False
	tilde = False
	v = False
	z= False
	x = False
	t = False
	r = False
	q = False
	w = False
	e = False
	a = False
	s = False
	d = False
	c = False
	l = False
	i = False
	comma = False
	equal = False
	dash = False
	plus = False
	numpad0 = False
	numpad1 = False
	numpad2 = False
	numpad3 = False
	numpad4 = False
	numpad5 = False
	numpad6 = False
	numpad7 = False
	numpad8 = False
	numpad9 = False
	plus = False
	minus = False


# may cause burning in eyes, it is useful

key_commands.~key_commands()


# the function for handling which keyboard input was sent #

def Log_keystroke(self, input_event, * icommands, key_down):
{	# simple concept really, pass whether the key is being held down
	# as True or False to the stored value for that key, if it is True,
	# also call the function for that keypress (so things can be handled a bit
	# easier in the main cpp file
	
	# to be honest though, might just be easier to directly store a copy
	# of the sf event itself
	
	# but I still want to store info about the keys that are up and those that
	# are down
	
	# maybe the best solution here is to create a vector of strings 
	# (or sf keyboard types, let the receiving object take a look at them
	# and deal with them how it sees fit
	
	# yes, yes, like that. The boolean wasnt really used anyways
	
	# The Enter or Return key
	if input_event == sf.Keyboard.Return:
	{	icommands.enter = key_down
		if key_down == True:
		{	icommands.Enter()


	# Spacebar, oddity, pressme
	elif input_event == sf.Keyboard.Space:
	{	icommands.space = key_down
		if key_down == True:
		{	icommands.Space()


	# The Arrow Keys
	elif input_event == sf.Keyboard.Up:
	{	icommands.up = key_down
		if key_down == True:
		{	icommands.Up()


	elif input_event == sf.Keyboard.Down:
	{	icommands.down = key_down
		if key_down == True:
		{	icommands.Down()


	elif input_event == sf.Keyboard.Right:
	{	icommands.right = key_down
		if key_down == True:
		{	icommands.Right()


	elif input_event == sf.Keyboard.Left:
	{	icommands.left = key_down
		if key_down == True:
		{	icommands.Left()


	# Tab Key
	elif input_event == sf.Keyboard.Tab:

	# Tilde Key
	elif input_event == sf.Keyboard.Tilde:
	{	icommands.tilde = key_down
		if key_down == True:
		{	icommands.Tilde()

	}	
	# Alphabetical keys A-Z
	elif input_event == sf.Keyboard.A:
	{	icommands.a = key_down
		if key_down == True:
		{	icommands.A()


	elif input_event == sf.Keyboard.B:

	elif input_event == sf.Keyboard.C:
	{	icommands.c = key_down
		if key_down == True:
		{	icommands.C()


	elif input_event == sf.Keyboard.D:
	{	icommands.d = key_down
		if key_down == True:
		{	icommands.D()


	elif input_event == sf.Keyboard.E:
	{	icommands.e = key_down
		if key_down == True:
		{	icommands.E()


	elif input_event == sf.Keyboard.F:
	{	icommands.f = key_down
		if key_down == True:
		{	icommands.F()


	#elif input_event == sf.Keyboard.G:
	#	#
	elif input_event == sf.Keyboard.H:

	elif input_event == sf.Keyboard.I:
	{	icommands.i = key_down
		if key_down == True:
		{	icommands.I()


	elif input_event == sf.Keyboard.J:

	elif input_event == sf.Keyboard.K:

	elif input_event == sf.Keyboard.L:
	{	icommands.l = key_down
		if key_down == True:
		{	icommands.L()


	elif input_event == sf.Keyboard.M:

	elif input_event == sf.Keyboard.N:
	{	icommands.n = key_down
		if key_down == True:
		{	icommands.N()


	elif input_event == sf.Keyboard.O:

	elif input_event == sf.Keyboard.P:

	elif input_event == sf.Keyboard.Q:
	{	icommands.q = key_down
		if key_down == True:
		{	icommands.Q()


	elif input_event == sf.Keyboard.R:
	{	icommands.r = key_down
		if key_down == True:
		{	icommands.R()


	elif input_event == sf.Keyboard.S:
	{	icommands.s = key_down
		if key_down == True:
		{	icommands.S()


	elif input_event == sf.Keyboard.T:
	{	icommands.t = key_down
		if key_down == True:
		{	icommands.T()


	elif input_event == sf.Keyboard.U:

	elif input_event == sf.Keyboard.V:
	{	icommands.v = key_down
		if key_down == True:
		{	icommands.V()


	elif input_event == sf.Keyboard.W:
	{	icommands.w = key_down
		if key_down == True:
		{	icommands.W()


	elif input_event == sf.Keyboard.X:
	{	icommands.x = key_down
		if key_down == True:
		{	icommands.X()


	elif input_event == sf.Keyboard.Y:

	elif input_event == sf.Keyboard.Z:
	{	icommands.z = key_down
		if key_down == True:
		{	icommands.Z()

	}	# Assorted keys, any real category
	elif input_event == sf.Keyboard.Comma:
	{	icommands.comma = key_down
		if key_down == True:
		{	icommands.Comma()


	elif input_event == sf.Keyboard.Dash:
	{	icommands.dash = key_down
		if key_down == True:
		{	icommands.Dash()


	elif input_event == sf.Keyboard.Delete:

	elif input_event == sf.Keyboard.Divide:

	elif input_event == sf.Keyboard.BackSlash:

	elif input_event == sf.Keyboard.BackSpace:
	{	

	elif input_event == sf.Keyboard.Equal:
	{	icommands.equal = key_down
		if key_down == True:
		{	icommands.Equal()


	elif input_event == sf.Keyboard.Escape:

	elif input_event == sf.Keyboard.LAlt:

	elif input_event == sf.Keyboard.LBracket:

	elif input_event == sf.Keyboard.RBracket:

	elif input_event == sf.Keyboard.LControl:

	elif input_event == sf.Keyboard.RControl:

	elif input_event == sf.Keyboard.LShift:

	elif input_event == sf.Keyboard.RShift:

	elif input_event == sf.Keyboard.Menu:

	elif input_event == sf.Keyboard.Multiply:

	elif input_event == sf.Keyboard.Period:
	{	icommands.period = key_down
		if key_down == True:
		{	icommands.Period()


	elif input_event == sf.Keyboard.Quote:

	elif input_event == sf.Keyboard.Slash:
	}	# Pageup/Pagedown
	elif input_event == sf.Keyboard.PageDown:

	elif input_event == sf.Keyboard.PageUp:
	}	# Windows/OSX/Super key whatever you wanna call it
	elif input_event == sf.Keyboard.LSystem:

	elif input_event == sf.Keyboard.RSystem:
	}	# Numeric inputs (row beneath the F## keys)
	elif input_event == sf.Keyboard.Num0:
	{	

	elif input_event == sf.Keyboard.Num1:

	elif input_event == sf.Keyboard.Num2:

	elif input_event == sf.Keyboard.Num3:

	elif input_event == sf.Keyboard.Num4:

	elif input_event == sf.Keyboard.Num5:

	elif input_event == sf.Keyboard.Num6:

	elif input_event == sf.Keyboard.Num7:

	elif input_event == sf.Keyboard.Num8:

	elif input_event == sf.Keyboard.Num9:
	}	# The Numpad. Beware Vaughannot !!
	elif input_event == sf.Keyboard.Numpad0:
	{	icommands.numpad0 = key_down
		if key_down == True:
		{	std.cout << "numpad0 keydown" << std.endl
			icommands.Numpad_0()


	elif input_event == sf.Keyboard.Numpad1:
	{	std.cout << "numpad1 keyevent" << std.endl
		icommands.numpad1 = key_down
		if key_down == True:
		{	icommands.Numpad_1()


	elif input_event == sf.Keyboard.Numpad2:
	{	std.cout << "numpad1 keyevent" << std.endl
		icommands.numpad2 = key_down
		if key_down == True:
		{	icommands.Numpad_2()


	elif input_event == sf.Keyboard.Numpad3:
	{	icommands.numpad3 = key_down
		if key_down == True:
		{	icommands.Numpad_3()


	elif input_event == sf.Keyboard.Numpad4:
	{	icommands.numpad4 = key_down
		if key_down == True:
		{	icommands.Numpad_4()


	elif input_event == sf.Keyboard.Numpad5:
	{	icommands.numpad5 = key_down
		if key_down == True:
		{	icommands.Numpad_5()


	elif input_event == sf.Keyboard.Numpad6:
	{	icommands.numpad6 = key_down
		if key_down == True:
		{	icommands.Numpad_6()


	elif input_event == sf.Keyboard.Numpad7:
	{	icommands.numpad7 = key_down
		if key_down == True:
		{	icommands.Numpad_7()


	elif input_event == sf.Keyboard.Numpad8:
	{	icommands.numpad8 = key_down
		if key_down == True:
		{	icommands.Numpad_8()


	elif input_event == sf.Keyboard.Numpad9:
	{	icommands.numpad9 = key_down
		if key_down == True:
		{	icommands.Numpad_9()


	elif input_event == sf.Keyboard.Add:
	{	icommands.plus = key_down
		if key_down == True:
		{	icommands.Plus()

	}		# must add minus
	elif input_event == sf.Keyboard.Subtract:
	{	icommands.minus = key_down
		if key_down == True:
		{	icommands.Minus()





# SFML_Window Utility class #########################/
########################################

# a really important part of the engine, it stores data used throughout
# basically just a plain old wrapper for sf.RenderWindow, it does store
# important info about the way that the window is related to the in game
# universe

SFML_Window.SFML_Window(std.string title, int h, int w)
{	Title = title
	Height = h
	Width = w
	Aperture_height = h
	Aperture_width = w
	# no real rhyme or reason behind how big the aperture should be at start
	# so we just give it the same as the window itself
	
	
	# self was the reason why everything appeared 10x too big
	Set_origin()
	# make sure it has a value
	window = sf.RenderWindow(sf.VideoMode(w, h), Title)
	# set up our RenderWindow
	window.setSize(sf.Vector2u(w, h))
	# and size it, but.. we dont need to here, the videomode already
	# did it.
	
	# I think self can be removed
	window.setTitle(Title)
	# again think its redundant, its a rarely used constructor, not too
	# concerning really


SFML_Window.SFML_Window(std.string title, int h, int w, double initial_rotation)
{	Title = title
	Height = h
	Width = w
	Aperture_height = h
	Aperture_width = w
	# no real rhyme or reason behind how big the aperture should be at start
	# so we just give it the same as the window itself
	
	
	# self was the reason why everything appeared 10x too big
	Set_origin()
	# make sure it has a value
	window = sf.RenderWindow(sf.VideoMode(w, h), Title)
	# set up our RenderWindow
	window.setSize(sf.Vector2u(w, h))
	# and size it, but.. we dont need to here, the videomode already
	# did it.
	
	# I think self can be removed
	window.setTitle(Title)
	# again think its redundant, its a rarely used constructor, not too
	# concerning really
	self.Set_aperture_rotation(initial_rotation)


def Set_origin(self):
{	origin.x = 0
	origin.y = 0
	# If we dont know, just say (0,0)


def Set_origin(self, double x, double y):
{	origin.x = x
	origin.y = y
	# set our VectorVictor2 to the right value


def Set_aperture_rotation(self, double new_rotation):
{	Aperture_rotation = new_rotation


def Set_aperture_dimensions(self, long unsigned int ap_w, long unsigned int ap_h):
{	Aperture_width = ap_w
	Aperture_height = ap_h
	# same deal, just for the aperture dimensions


def Intersection(self, center, double radius, double cam_scale):
{	# self *works* for now, I very strongly am considering just redoing it
	# as a circle with radius of the sum of the squares of the window axis.
	# This is efficient for small camera views, could prove really bad at
	# large camera views...
	
	# I dunno, the current thing is a mess, just leave it like self
	
	center -= self.origin
	# get the relative offset of the center of the circle from the window
	# origin 
	center.Rotate_vector(-(self.Aperture_rotation))
	# rotate things around so that the window is straight up and down relative
	# to our coordinate system
	center.y *= -1
	# now case statements, on the dimensions of the center vector
	# at self point
	if center.x < 0:
	{	if center.x < -radius:
		{	return False

		else:
		{	if (center.y >= 0)and(center.y <= Aperture_height):
			{	return True

			else:
			{	# the long method for checking if we intersect the edge anyways
				
				# ehh screw it, not do it right
				
				if center.y > self.Aperture_height:
				{	center.y -= self.Aperture_height

				long r_squared = (center.y*center.y) + (center.x*center.x)
				if r_squared <= (radius*radius):
				{	# compare the squares so that we dont have to invoke
					# pythagoras
					return True

				else:
				{	return False




	elif (center.x >= 0)and(center.x <= self.Aperture_width):
	{	# the middle case, easier
		if center.y > 0:
		{	long v_offset = center.y - self.Aperture_height
			if v_offset > radius:
			{	return False

			else:
			{	return True


		else:
		{	# center.y is negative, the center of the circle is above the
			# box
			if center.y > radius:
			{	return False

			else:
			{	return True



	else:
	{	# positive and beyond the edge of the aperture
		if center.x > (radius + self.Aperture_width):
		{	return False
			# too far away by definition, False

		else:
		{	# we already know from the above middle section check that self
			# case must have an origin that is less than radius from the
			# aperture width along the x axis
			if (center.y >= 0)and(center.y <= self.Aperture_height):
			{	# identical check to the negative side of the box
				return True

			else:
			{	# the long method for checking if we intersect the edge anyways
				
				# ehh screw it, not do it right
				long dx = center.x - self.Aperture_width
				if center.y > self.Aperture_height:
				{	center.y -= self.Aperture_height
				}				
				long r_squared = (center.y*center.y) + (dx*dx)
				if r_squared <= (radius*radius):
				{	# compare the squares so that we dont have to invoke
					# pythagoras
					return True

				else:
				{	return False




	# that was a beast
	
	# heres hoping its more efficient than it looks...


def Window_intersection(self, window_point):
{	if (window_point.x >= 0)and(window_point.y >= 0):
	{	# we check and make sure that our point has at least positive
		# coordinate values
		if (window_point.x <= (float)self.Width)and(window_point.y <= (float)self.Height):
		{	# and we check that the point is within the width and height of the
			# window in pixels away from the (0,0)
			return True

		else:
		{	return False


	else:
	{	return False



def Intersection(self, point, double cam_scale):
{	# implicitly in camera view based on the double being passed
	window_offset = Get_window_coordinates(point, self,cam_scale)
	# get the relative offset of the given point relative to the window given
	# the stuff we passed
	return self.Window_intersection(window_offset)
	# and return whatever Window_intersection thinks of that...


def Intersection(self, point, zoom_factor):
{	# implicitly in map view based on the int being passed
	window_offset = Get_window_coordinates(point, self, zoom_factor)
	std.cout << "Window coordinates of point: " << window_offset.x << "," << window_offset.y << std.endl
	# get the relative offset of the given point relative to the window given
	# the stuff we passed
	return self.Window_intersection(window_offset)
	# and return whatever Window_intersection thinks of that...


SFML_Window.~SFML_Window()
{	delete window
	#	cant forget self ;D


# Utility functions to transform between window relative coordinates to ###/
# ignition universe coordinates and back again ################
########################################


def Get_window_coordinates(self, sim_point, * iwindow, map_scale):
{	VectorVictor.Vector2 camera_origin(iwindow.origin.Get_x(), iwindow.origin.Get_y())
	sim_point -= camera_origin
	# get the relative offset from the camera origin to
	# the point in simulation as a VV2
	sim_point.Rotate_vector(-iwindow.Aperture_rotation)
	# rotate the vector around into the reference frame of
	# the window box, straight up and down relative to
	# the global coordinate frame axes
	sim_point.y *= -1
	# flip the y axis because SFML does things that way
	sim_point.y *= pow(0.1, (map_scale))
	sim_point.x *= pow(0.1, (map_scale))
	# scale back the length of the vector by 10^map_scale
	# so that its dimensions are in pixels instead of
	# meters
	sf.Vector2f camera_offset(sim_point.x, sim_point.y)
	# put the vector into an sfml 2f type so we can return
	# it properly
	return camera_offset


def Get_window_coordinates(self, sim_point, * iwindow, double cam_scale):
{	
	
	
	VectorVictor.Vector2 camera_origin(iwindow.origin.Get_x(), iwindow.origin.Get_y())
	sim_point -= camera_origin
	# get the relative offset of the point from the window
	# origin
	sim_point.Rotate_vector(-iwindow.Aperture_rotation)
	# rotate the vector so that the window is straight up and down
	sim_point.y *= -1
	# flip the y axis so that SFML is happy
	sim_point.x *= (10.00000000/(long double)cam_scale)
	sim_point.y *= (10.00000000/(long double)cam_scale)
	# of coursenot  Because cam_scale = 1 (or default), window has
	# dimensions 10x smaller in meters than its actual window dimensions
	# in pixels, dimensions of 102.4 x 60.9 meters for a window with
	# size of 1024 x 609. So thats why we have to scale it up by 10x
	
	sf.Vector2f camera_offset(sim_point.x, sim_point.y)
	# convert into an sf 2f for return
	return camera_offset


def Get_simulation_coordinates(self, window_point, * iwindow, map_scale):
{	VectorVictor.Vector2 sim_point(window_point.x, window_point.y)
	# so we start with our relative offset of the point in the window
	# in pixels
	sim_point.y *= pow((long double)10, (map_scale))
	sim_point.x *= pow((long double)10, (map_scale))
	# and we scale it up by the zoom factor so we get the relative offset
	# of the point from the camera origin in universe coordinates
	sim_point.y *= -1
	# we flip the y axis, the SFML coordinates will be mirrored
	# vertically compared to the VV2 coordinate system that the engine
	# uses 
	
	sim_point.Rotate_vector(iwindow.Aperture_rotation)
	
	sim_point += iwindow.origin
	# we add the main windows origin point to transform the point to the sim
	# sessions global origin point (0,0)
	return sim_point
	# actually wasnt that complex I guess, the previous operations in
	# reverse


def Get_simulation_coordinates(self, window_point, * iwindow, cam_scale):
{	VectorVictor.Vector2 sim_point(window_point.x, window_point.y)
	
	sim_point.x /= (10.00000000/(long double)cam_scale)
	sim_point.y /= (10.00000000/(long double)cam_scale)
	# here we now shrink it back, the pixel coordinates are bigger
	# than the actual universe coordinates by a factor of 10/cam_scale
	sim_point.y *= -1
	# again, the y axis because SFML has down as positive y, reasons
	# that are its own
	sim_point.Rotate_vector(iwindow.Aperture_rotation)
	# rotate the vector back into the correct orientation of the window
	
	sim_point += iwindow.origin
	# we add the main windows origin point to transform the point to the sim
	# sessions global origin point (0,0)
	return sim_point
	# and we return the vector point now in universe coordinates


# gonna probably need to write at least one of these from scratch methinks






# Ignition Drawables #############################
# wrapper classes for stuff that needs to be drawn to the window like text ##
# displays & other infographics that the vessel instance draws as ui #####
# displays and things like that #######################/
########################################



def Draw_element(self, * iwindow):
{	Talkback("Bad call to Ignition_drawable.Draw_element(SFML_Window * iwindow)")


def Center_element(self):
{	Talkback("Ignition_drawable.Center_element()")


# Ignition text type #############################
########################################

Ignition_text.Ignition_text(std.string text_font, initial_position, initial_text, initial_colour, int character_size, center_origin)
{	self.Init_object(text_font, initial_position, initial_text, initial_colour, character_size, center_origin)


Ignition_text.Ignition_text(sf.Font text_font, initial_position, initial_text, initial_colour, int character_size, center_origin)
{	self.Init_object(text_font, initial_position, initial_text, initial_colour, character_size, center_origin)


Ignition_text.Ignition_text()
{	# uhhh.
	# just try not to call self very often


def Init_object(self, text_font, initial_position, initial_text, initial_colour, int character_size, center_origin):
{	font.loadFromFile(text_font)
	text.setFont(font)
	text.setPosition(initial_position)
	text.setString(initial_text)
	text.setColor(initial_colour)
	text.setCharacterSize(character_size)
	if center_origin == True:
	{	self.Center_element()

	return True
	# just to let everybody know that nothing went wrong


def Init_object(self, text_font, initial_position, initial_text, initial_colour, int character_size, center_origin):
{	font = text_font
	# hopefully self works, might have to pass by reference if not
	text.setFont(font)
	text.setPosition(initial_position)
	text.setString(initial_text)
	text.setColor(initial_colour)
	text.setCharacterSize(character_size)
	if center_origin == True:
	{	self.Center_element()

	return True
	# just to let everybody know that nothing went wrong



def Set_element(self, text_string):
{	text.setString(text_string)


def Set_element(self, new_position):
{	text.setPosition(new_position)


def Set_element(self, new_colour):
{	text.setColor(new_colour)


def Set_element(self, int character_size):
{	text.setCharacterSize(character_size)


def Set_element(self, text_string, new_position):
{	self.Set_element(text_string)
	self.Set_element(new_position);		


def Set_element(self, text_string, new_position, new_colour):
{	self.Set_element(text_string)
	self.Set_element(new_position)
	self.Set_element(new_colour)


def Set_element(self, text_string, new_colour):
{	self.Set_element(text_string)
	self.Set_element(new_colour)


def Set_element(self, new_position, new_colour):
{	self.Set_element(new_position)
	self.Set_element(new_colour)


def Draw_element(self, * iwindow):
{	iwindow.window.draw(text)


def Center_element(self):
{	if Centered == False:
	{	# do some centering
		# not sure how with sf.Text really
		# I mean what are the dimensions of the text strip?
		
		# important question to get answered
		Centered = True


	
Ignition_text.~Ignition_text()



# Ignition Circle ##############################/
########################################


Ignition_circle.Ignition_circle(sf.Vector2f initial_position, initial_colour, initial_radius, center_origin)
{	self.Init_object(initial_position, initial_colour, initial_radius, center_origin)


Ignition_circle.Ignition_circle()


def Init_object(self, initial_position, initial_colour, initial_radius, center_origin):
{	circle_shape.setPosition(initial_position)
	circle_shape.setFillColor(initial_colour)
	circle_shape.setRadius(initial_radius)
	if center_origin == True:
	{	self.Center_element()

	return True


def Set_element(self, new_radius):
{	circle_shape.setRadius(new_radius)


def Set_element(self, new_position):
{	circle_shape.setPosition(new_position)


def Set_element(self, new_colour):
{	circle_shape.setFillColor(new_colour)


def Set_element(self, new_radius, new_position):
{	self.Set_element(new_radius)
	self.Set_element(new_position)


def Set_element(self, new_radius, new_position, new_colour):
{	self.Set_element(new_radius)
	self.Set_element(new_position)
	self.Set_element(new_colour)


def Set_element(self, new_radius, new_colour):
{	self.Set_element(new_radius)
	self.Set_element(new_colour)


def Set_element(self, new_position, new_colour):
{	self.Set_element(new_position)
	self.Set_element(new_colour)


def Draw_element(self, * iwindow):
{	iwindow.window.draw(circle_shape)


def Center_element(self):
{	if Centered == False:
	{	# do some centering stuffz
		circle_shape.setOrigin(sf.Vector2f(circle_shape.getRadius(), circle_shape.getRadius()))
		Centered = True



Ignition_circle.~Ignition_circle()
{	# no pointers, problems



# Ignition rectangle #############################
########################################

Ignition_rectangle.Ignition_rectangle(sf.Vector2f initial_position, initial_colour, width, height, center_origin)
{	self.Init_object(initial_position, initial_colour, width, height, center_origin)


Ignition_rectangle.Ignition_rectangle()


def Init_object(self, initial_position, initial_colour, width, height, center_origin):
{	rect_shape.setPosition(initial_position)
	rect_shape.setFillColor(initial_colour)
	rect_shape.setSize(sf.Vector2f(width, height))
	if center_origin == True:
	{	Center_element()

	return True


def Set_element(self, new_width, new_height):
{	rect_shape.setSize(sf.Vector2f(new_width, new_height))


def Set_element(self, new_position):
{	rect_shape.setPosition(new_position)


def Set_element(self, new_colour):
{	rect_shape.setFillColor(new_colour)


def Set_element(self, new_width, new_height, new_position):
{	self.Set_element(new_width, new_height)
	self.Set_element(new_position);	


def Set_element(self, new_width, new_height, new_position, new_colour):
{	self.Set_element(new_width, new_height)
	self.Set_element(new_position)
	self.Set_element(new_colour)


def Set_element(self, new_width, new_height, new_colour):
{	self.Set_element(new_width, new_height)
	self.Set_element(new_colour)


def Set_element(self, new_position, new_colour):
{	self.Set_element(new_position)
	self.Set_element(new_colour)
}	
	
def Draw_element(self, * iwindow):
{	iwindow.window.draw(rect_shape)


def Center_element(self):
{	if Centered == False:
	{	rect_shape.setOrigin(sf.Vector2f((rect_shape.getSize().x)/2, (rect_shape.getSize().y)/2))
		# looks good
		Centered = True



Ignition_rectangle.~Ignition_rectangle()


# Ignition sprite class ###########################/
# just easier to wrap it and force it into the shape I want it in ######/
########################################

Ignition_sprite.Ignition_sprite(sf.Vector2f initial_position, initial_colour, center_origin, texture_path)
{	self.Init_object(initial_position, initial_colour, center_origin, texture_path)


Ignition_sprite.Ignition_sprite(sf.Vector2f initial_position, initial_colour, center_origin, texture, copy_texture)
{	self.Init_object(initial_position, initial_colour, center_origin, texture, copy_texture)


Ignition_sprite.Ignition_sprite()


	
def Init_object(self, initial_position, initial_colour, center_origin, texture_path):
{	
	if not Tex.loadFromFile(texture_path):
	{	return False

	sprite.setTexture(Tex)
	sprite.setPosition(initial_position)
	# set the position of the sprite in the window
	
	# ehh, dont worry about the image thing just yet
	if center_origin == True:
	{	self.Center_element()

	return True


def Init_object(self, initial_position, initial_colour, center_origin, &texture, copy_texture):
{	if copy_texture == True:
	{	Tex = texture
		# just make the internally stored texture into a clone of the one we
		# received.
		sprite.setTexture(Tex)

	else:
	{	sprite.setTexture(texture)
		# set the sprite directly off of the texture that we were passed.
		# If you do it self way, sf.Texture had better survive for the
		# lifetime of the Ignition_sprite or you will have problems

	sprite.setPosition(initial_position)
	# set the position of the sprite in the window
	
	# ehh, dont worry about the image thing just yet
	if center_origin == True:
	{	self.Center_element()

	return True;	

	
def Set_element(self, new_position):
{	sprite.setPosition(new_position)


def Set_element(self, new_position, new_colour):
{	sprite.setPosition(new_position)


def Set_element(self, new_colour):
{	# uhhh, dont wanna deal with self right now


def Center_element(self):
{	if Centered == False:
	{	sprite.setOrigin(sf.Vector2f(((sprite.getTexture().getSize().x)/2), ((sprite.getTexture().getSize().y)/2)))
		# looks good
		Centered = True



def Draw_element(self, * iwindow):
{	iwindow.window.draw(sprite)


Ignition_sprite.~Ignition_sprite()






# SFML splash screen class ##########################

# and everything after self point is an absolute mess

# avert your eyes ><

SFML_loadscreen.SFML_loadscreen(std.string texture_path, fadein, fade_in_length, fade_out_length)
{	Active = True
	splash_texture = sf.Texture()
	if not splash_texture.loadFromFile(texture_path):
	{	self.~SFML_loadscreen()

	else:
	{	splash_sprite = sf.Sprite()
		splash_sprite.setTexture(*splash_texture)
	}	Fader = 0
	Fading_out = False
	Fade_clock = sf.Clock()
	if fadein == True:
	{	Fade_in()

	else:
	{	Fading_in = False
	}	Fade_in_time = fade_in_length
	Fade_out_time = fade_out_length
	splash_sprite.setColor(sf.Color(255, 255, 255, ((Fader)*255)))


def Fade_in(self):
{	Fading_in = True
	Fade_clock.restart()


def Fade_out(self):
{	Fading_out = True
	Fade_clock.restart()


def Toggle_fade(self):
{	if Fading_in == True:
	{	if (Fading_out == False)and(Fader != 1.000):
		{	std.cout << "Toggle Fadein" << std.endl
			Fade_in()

		else:
		{	Fade_out()
			std.cout << "Toggle Fadeout" << std.endl




def Update_screen(self):
{	if ((Fading_in == True)and(Fader != 1.000))or((Fading_out == True)or(Fader != 0.000)):
	{	time = Fade_clock.getElapsedTime()
		if time.asMilliseconds() > 100:
		{	increment = time.asMilliseconds()
			if (Fading_in == True)and(Fading_out == False):
			{	Fader += (increment/(Fade_in_time*1000))
				if Fader > 1.0000:
				{	Fader = 1.0000


			elif (Fading_in == True)and(Fading_out == True):
			{	Fader -= (increment/(Fade_in_time*1000))
				if Fader < 0.0000:
				{	Fader = 0.0000

			}	splash_sprite.setColor(sf.Color(255, 255, 255, ((Fader)*255)))
			Fade_clock.restart()


	if (Fader == 0.000)and(Fading_in == True)and(Fading_out == True)and(Active == True):
	{	std.cout << "Loadscreen Dropout" << std.endl
		Active = False



SFML_loadscreen.~SFML_loadscreen()
{	delete splash_sprite
	delete splash_texture
	delete Fade_clock


# SFML title screen class ##########################/

SFML_titlescreen.SFML_titlescreen(std.string texture_path, fadein, fade_in_length, fade_out_length, title, font_path, int textr, int textg, int textb, int pfont, Title_pos)
{	Active = True
	splash_texture = sf.Texture()
	splash_texture.setSmooth(True)
	splash_sprite = sf.Sprite()
	if not splash_texture.loadFromFile(texture_path):
	{	self.~SFML_titlescreen()

	else:
	{	splash_sprite.setTexture(*splash_texture)
	}	Fader = 0
	Fading_out = False
	Fade_clock = sf.Clock()
	if fadein == True:
	{	Fade_in()

	else:
	{	Fading_in = False
	}	Fade_in_time = fade_in_length
	Fade_out_time = fade_out_length
	splash_sprite.setColor(sf.Color(255, 255, 255, 0))
	Title_font = sf.Font()
	if not Title_font.loadFromFile(font_path):
	{	self.~SFML_titlescreen()

	else:
	{	Title_text = sf.Text(title, *Title_font, pfont)
		Title_text.setColor(sf.Color(textr, textg, textb, 0))
		r = textr
		g = textg
		b = textb
	}	Title_position = Title_pos
	Title_text.setPosition(Title_position)
	finished = False


def Fade_in(self):
{	Fading_in = True
	Fade_clock.restart()


def Fade_out(self):
{	Fading_out = True
	Fade_clock.restart()


def Toggle_fade(self):
{	if Fading_in == True:
	{	if (Fading_out == False)and(Fader != 1.000):
		{	std.cout << "Toggle Fadein" << std.endl
			Fade_in()

		else:
		{	Fade_out()
			std.cout << "Toggle Fadeout" << std.endl




def Update_screen(self):
{	if ((Fading_in == True)and(Fader != 1.000))or((Fading_out == True)or(Fader != 0.000)):
	{	time = Fade_clock.getElapsedTime()
		if time.asMilliseconds() > 100:
		{	increment = time.asMilliseconds()
			if (Fading_in == True)and(Fading_out == False):
			{	Fader += (increment/(Fade_in_time*1000))
				if Fader > 1.0000:
				{	Fader = 1.0000


			elif (Fading_in == True)and(Fading_out == True):
			{	Fader -= (increment/(Fade_in_time*1000))
				if Fader < 0.0000:
				{	Fader = 0.0000
					finished = True
					#Deallocate_me()

			}	splash_sprite.setColor(sf.Color(255, 255, 255, ((Fader)*255)))
			Title_text.setColor(sf.Color(r, g, b, Fader*(255)))
			Fade_clock.restart()


	if (Fader == 0.000)and(Fading_in == True)and(Fading_out == True)and(Active == True):
	{	std.cout << "Loadscreen Dropout" << std.endl
		Active = False


 



SFML_titlescreen.~SFML_titlescreen()
{	delete Title_text
	delete Title_font
	delete splash_sprite
	delete splash_texture
	delete Fade_clock


# Utility function ##############################
########################################

std.string Input_string(std.string prompt)		# self should go, not
{	data = "";	# a great idea in a project primarily focused
	while(True)				# on gui
	{	std.cout << std.endl
		std.cout << prompt
		while(std.getline(std.cin, data))
		{	if data.size() == 0:
			{	continue;	
			}	
			else:
			{	return data
			}	



