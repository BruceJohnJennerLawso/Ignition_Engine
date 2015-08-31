# mfd.cpp ##################################/
# MFD Multifunctional Displays, we dont have to fly blind anymore #####
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"
#include "mfd.hpp"

def Get_MFD_Name(self):
{	Talkback("Bad call to MFD.Get_MFD_Name()")
	return "FUU"


def Render_MFD(self, &window, &keyCommands, &cursorCommands, double &cameraScale, double dt, &time_acceleration, double sim_time, &current_vessel, &newtonians, &keplerians, &vessels, &Camera_target, double &Camera_rotation):
{	Talkback("Bad call to MFD.Render_MFD(SFML_Window &window, &keyCommands, &cursorCommands, double &cameraScale, double dt, &time_acceleration, double sim_time, &current_vessel, &newtonians, &keplerians, &vessels, &Camera_target, double &Camera_rotation)")
	return False


def Draw_MFD(self, &window, Placard_color):
{	# okay, we go
	sf.RectangleShape placard(sf.Vector2f(Width, Height))
	placard.setFillColor(Placard_color)
	# we set the background to the mfd to the passed colour
	placard.setPosition(self.Get_mfd_position(window))
	# and we set its position in the window appropriately
	window.window.draw(placard)
	# and draw it to the window
	
	
	# now we need to do the same with whatever the MFD spat out
	sf.Sprite mfd_sprite(MFD_image)
	# create a sprite that uses the texture we generated for the mfd
	mfd_sprite.setPosition(self.Get_mfd_position(window))
	# set its position in the window, as with the placard background
	window.window.draw(mfd_sprite)
	# and draw it onscreen


def Set_MFD_orientation(self, new_orientation):
{	MFD_orientation = new_orientation


def Set_MFD_offset(self, new_offset):
{	MFD_offset = new_offset


def Get_mfd_position(self, &window):
{	sf.Vector2f position(MFD_offset.x, MFD_offset.y)
	if MFD_orientation == upper_left:
	{	# the simple case, do as usual
		position = MFD_offset
		# we set it equal to our offset, mode
		return position

	elif MFD_orientation == upper_right:
	{	#a wee bit more complex, time our y axis is normal, the x axis
		# gets interesting
		deltax = (float)window.Width - (MFD_offset.x + Width)
		position.x = deltax
		position.y = MFD_offset.y
		return position

	elif MFD_orientation == lower_left:
	{	# similar situation, 'cept now the y axis is reversed
		deltay = (float)window.Height - (MFD_offset.y + Height)
		position.y = deltay
		position.x = MFD_offset.x
		return position

	elif MFD_orientation == lower_right:
	{	# the last case, to the far bottom side of the window
		deltax = (float)window.Width - (MFD_offset.x + Width)
		deltay = (float)window.Height - (MFD_offset.y + Height)
		position.x = deltax
		position.y = deltay
		return position

	else:
	{	return position
		# dammit MSVC



def Transform_to_mfd(self, &window, input):
{	input -= self.Get_mfd_position(window)
	return input


# Surface MFD ################################/
# The first prototype ############################/
########################################

Surface_MFD.Surface_MFD()
{	Height = 400
	Width = 400
	# these seem like reasonable defaults
	if not canvas.create(Height, Width):
	{	# something screwed up, we got no more memory to play with
		Talkback("Unable to Create MFD canvas")
	}	# also need to check to see whether the order of height/width is
		# correct	
		
	# the canvas.create function proved insanely slow to create every single
	# call to Render MFD, we moved it here (ran at barely 3 fps)
	
	# My best idea would maybe be to make it an optional call in Render MFD
	# to create it if the MFD gets resized
	
	if not Display_font.loadFromFile("./Data/Fonts/orbitron-light.ttf"):
	{	Talkback("Unable to load font for surface MFD")



def Get_MFD_Name(self):
{	type_name = surface_mfd
	return type_name


def Render_MFD(self, &window, &keyCommands, &cursorCommands, double &cameraScale, double dt, &time_acceleration, double sim_time, &current_vessel, &newtonians, &keplerians, &vessels, &Camera_target, double &Camera_rotation):
	# we create a special sfml object that allows us to draw to it just like
	# it is a screen, though its not. Then at the end we can retrieve a
	# texture from it and use that for drawing

	canvas.clear()
	
	std.string altitude; 
	if current_vessel.NewtonianState.Current_state == Flight:
	{	VectorVictor.Vector2 offset(0,0);		
		# if the vessel is in flight, 
		# set the offset to whatever the position of the parent vessel is
		long separation = 0
		std.vector<long double> altitude_list
		for(std.vector<CKeplerian_Object*>it = keplerians.begin(); it != keplerians.end(); ++it)
		{	separation = 0
			# ummm
			offset = current_vessel.NewtonianState.FlightState.Position
			offset -= (*it).Get_position()
			# make our offset vector equal to the relative offset between
			# the planet and the vessel
			separation = offset.Get_vector_magnitude()
			separation -= (*it).Get_radius(0)
			# make separation equivalent to the magnitude of the vector in
			# question and subtract the radius from that so we have altitude 
			# instead of radius
			altitude_list.push_back(separation)
			# now we need to find the smallest value of the separation 

		# having run through the list of keplerian objects we need to find the
		# smallest value in the list
		altitude = std.to_string(Smallest_value(altitude_list))

		# awwwwww YEEHAW


	else:
	{	altitude = "0 m"


	#altitude = std.to_string(current_vessel.NewtonianState.Current_state);	
	
	#altitude = current_vessel.Get_vessel_name();	
	
	
	
	sf.Text Altitude
	Altitude.setFont(Display_font)
	Altitude.setColor(sf.Color(157,226,252, 255))
	
	if cursorCommands.Right_click == True:
	{	offset = Transform_to_mfd(window, cursorCommands.position); 
		std.cout << "mfd click offset: (" << offset.x << "," << offset.y << ")" << std.endl; 
		if (offset.x <= Width)and(offset.y <= Height):
		{	if (offset.x >= 0)and(offset.y >= 0):
				Altitude.setColor(sf.Color(252, 223, 43, 255))
				# self works, not very smoothly, we only get info
				# about clicks every couple milliseconds or so, makes it
				# very sticky
				
				# perhaps we just force a re render every time a click is 
				# clicked on an MFD to get around self



	Altitude.setString(altitude)
	# canvas.draw(stuff)
	canvas.draw(Altitude)
	# on the one hand, could always just write another function that takes
	# our crapload of arguments like before along with the rendertexture canvas
	# by reference and draws it
	
	# but I dont wanna make things more complex just to be idiotproof, so
	# if you are an idiot, GTFOnot !!
	
	# If you are to making mfds and want to learn, read the SFML
	# tutorial docs on the sfml website regarding graphics and windows and
	# you should have no problem understanding self
	
	canvas.display()
	# not a hundred percent sure what self does, I think it is necessary
	
	MFD_image = canvas.getTexture()
	return True
	# we done here, went okay


Surface_MFD.~Surface_MFD()


def Create_MFD(self, type_name, * &new_mfd, mfd_orientation, mfd_offset):
{	if type_name == surface_mfd:
	{	# think self will work...
		new_mfd = Surface_MFD()
		new_mfd.Set_MFD_orientation(mfd_orientation)
		new_mfd.Set_MFD_offset(mfd_offset)
		return True

	else:
	{	return False


