# Vessel.cpp #################################
# The TVessel type used for vessels #####################/
########################################
##include <std_files>
##include "Headers.h"
##include "Headers.hpp"
##include "Source.cpp"
#include "Vessel.hpp"


# TVessel Class ###############################/
########################################



# getting a little more specific at self point, with specific
# functionality that a spacecraft really needs to have


def Translate_left(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(translate_left):
		{	(*it).Throttle_up(dt, RCS_Throttle_constant)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)





# obvious as it gets, adjust throttles based on dt and the k_throttle
# for the given thruster. This should accept a parameter to modify k_throttle
# so we can modify it on the spot instead of passing through stuff with dt
# I smell disaster there, its better not to tempt fate

def Translate_right(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(translate_right):
		{	(*it).Throttle_up(dt, RCS_Throttle_constant)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)





def Translate_backward(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(translate_back):
		{	(*it).Throttle_up(dt, RCS_Throttle_constant)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)





def Translate_forward(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(translate_forward):
		{	(*it).Throttle_up(dt, RCS_Throttle_constant)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)



}	

def Rotate_right(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(rotate_clockwise):
		{	(*it).Throttle_up(dt, RCS_Throttle_constant)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)



}	

def Rotate_right(self, dt, throttle_target):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(rotate_clockwise):
		{	(*it).Throttle_to(dt, RCS_Throttle_constant, throttle_target)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)



}	

def Rotate_left(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(rotate_counterclockwise):
		{	(*it).Throttle_up(dt, RCS_Throttle_constant)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)



}	

def Rotate_left(self, dt, throttle_target):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(rotate_counterclockwise):
		{	(*it).Throttle_to(dt, RCS_Throttle_constant, throttle_target)

		else:
		{	if (*it).Is_RCS == True:
			{	(*it).Throttle_down(dt, RCS_Throttle_constant)



}	

def No_command(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_RCS == True:
		{	(*it).Throttle_down(dt, RCS_Throttle_constant)


}	

def Throttle_up(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(main_engines):
		{	(*it).Throttle_up(dt, Main_throttle_constant)


	


def Throttle_down(self, dt):
{	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(main_engines):
		{	(*it).Throttle_down(dt, Main_throttle_constant)


}	

def Kill_rotation(self, dt):
{	if self.NewtonianState.Rotation.Omega != 0.000000000000:
	{	
		delta_omega = Absolute_value(self.NewtonianState.Rotation.Omega)
		double magnitude_alpha
		double max_alpha
		if self.NewtonianState.Rotation.Omega > 0.000000000000:
		{	max_alpha = self.Get_max_alpha(counterclockwise)
			magnitude_alpha = Absolute_value(max_alpha)
			if self.NewtonianState.Rotation.Alpha > 0.000000000000:
			{	Rotate_left(dt)

			else:
			{	if magnitude_alpha > delta_omega:
				{	percentage = (delta_omega/magnitude_alpha)
					Rotate_left(dt, percentage)

				else:
				{	Rotate_left(dt)



		else 	# the negative case
		{	max_alpha = self.Get_max_alpha(clockwise)
			magnitude_alpha = Absolute_value(max_alpha)
			if self.NewtonianState.Rotation.Alpha < 0.000000000000:
			{	Rotate_right(dt)

			else:
			{	if magnitude_alpha > delta_omega:
				{	percentage = (delta_omega/magnitude_alpha)
					Rotate_right(dt, percentage)

				else:
				{	Rotate_right(dt)


		}	
	}	
	# awwww yeahhhh, what Im talkin about
	else:
	{	No_command(dt)





def Draw_controls(self, * iwindow, Map_status):
{	# should be a warning message here too...


def Init_thruster(self, is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_id):
{	Thruster new_thruster = Monopropellant_thruster(is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, fuel_tank, group_id)
	self.Object_components.insert(self.Object_components.end(), new_thruster.Get_vessel_component_pointer());	
	Thrusters.insert(Thrusters.end(), new_thruster)
	return True

	
def Init_thruster(self, is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_id1, group_id2):
{	Thruster new_thruster = Monopropellant_thruster(is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, fuel_tank, group_id1, group_id2)
	self.Object_components.insert(self.Object_components.end(), new_thruster.Get_vessel_component_pointer());	
	Thrusters.insert(Thrusters.end(), new_thruster)
	return True


def Init_thruster(self, is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, * fuel_tank, group_id1, group_id2, group_id3):
{	Thruster new_thruster = Monopropellant_thruster(is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, fuel_tank, group_id1, group_id2, group_id3)
	self.Object_components.insert(self.Object_components.end(), new_thruster.Get_vessel_component_pointer());	
	Thrusters.insert(Thrusters.end(), new_thruster);	
	return True


long double TVessel.Get_max_alpha(rotation_direction direction)
{	thruster_group rotation_group
	if direction == clockwise:
	{	rotation_group = rotate_clockwise

	else:
	{	rotation_group = rotate_counterclockwise

	
	long max_alpha = 0
	for(std.vector<Thruster*>it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if (*it).Is_in_group(rotation_group):
		{	max_alpha -= (*it).Get_maximum_torque(1)
			# I need to remove self dt thing, isnt right


	max_alpha /= Get_PMI()
	return max_alpha



def In_view(self, * window, zoom_factor):
{	long radius = self.Get_length()
	radius /= 2
	if window.Intersection(self.NewtonianState.FlightState.Position, radius, 1) == True:
	{	# cam scale of 1 for the moment just so we dont have any crazy stuff
		return True

	else:
	{	#std.cout << "No intersection for vessel " << self.Get_vessel_name() << std.endl
		return False

	#return window.Intersection(self.NewtonianState.FlightState.Position, radius)
	# muuuch better
	
	
	#if (NewtonianState.FlightState.Position.Get_x() >= (window.origin.x-(Hull_component.Get_hull_length_squared())))and(NewtonianState.FlightState.Position.Get_x() <= ((window.origin.x + (window.Aperture_width+(Hull_component.Get_hull_length_squared()))))):
	#{	if (NewtonianState.FlightState.Position.Get_y() <= (window.origin.y+(Hull_component.Get_hull_length_squared())))and(NewtonianState.FlightState.Position.Get_y() >= (window.origin.y - ((window.Aperture_height+(Hull_component.Get_hull_length_squared()))))):
	#	{	return True
	#
	#	else:
	#	{	return False
	#
	#}	
	#else:
	#{	return False
	#
	
	
	# just another simple function for determining if the ship should be drawn
	# there was some logic behind squaring the distance, I dont recall


def In_view(self, * window, double cam_scale):
{	long radius = self.Get_length()
	radius /= 2
	if window.Intersection(self.NewtonianState.FlightState.Position, radius, cam_scale) == True:
	{	# cam scale of 1 for the moment just so we dont have any crazy stuff
		return True

	else:
	{	#std.cout << "No intersection for vessel " << self.Get_vessel_name() << std.endl
		return False



def Draw_vessel(self, * iwindow, double cam_scale):
{	scale_factor = pix_length/(Vessel_tex.getSize().y*cam_scale)
	
#	Object_sprite.setScale((pix_length/((Vessel_tex.getSize().y)*cam_scale)),(pix_length/((Vessel_tex.getSize().y)*cam_scale)))

	Object_sprite.setScale(sf.Vector2f(scale_factor,scale_factor))
	# rescale the axes of the texture to match pix_length in the y and the
	# appropriate scale for the x dimension
		
	# not sure why they both use y, I think self was distorted when
	# it used x & y.
	# This area needs to be looked over again
	camera_offset = Get_window_coordinates(NewtonianState.FlightState.Position, iwindow, cam_scale)
	# I just dont get it, should work just fine
	# translation looks okay, the thing goes nuts rotating twice over the
	# full 360 degrees 
	Object_sprite.setPosition(camera_offset)
	Object_sprite.setRotation(NewtonianState.Rotation.Theta - iwindow.Aperture_rotation)
	# that should work just fine and dandy

	Flag_sprite.setPosition(camera_offset + sf.Vector2f((10*Hull_component.Length), -(10*Hull_component.Length)))
	# draws the flag near the vessel for some reason.
	# really just window-dressing here, necessary
	
	# probably should be removed. If self is done again, will
	# be done at a different level
	iwindow.window.draw(*Object_sprite)
	#iwindow.window.draw(*Flag_sprite)
	# and we draw them onto the window, as it sounds


def Draw_flag(self, * iwindow, zoom_factor):
{	#sf.Vector2f offset(Position.Get_x(), Position.Get_y());	
	# aaand we create our sf vector for the flag position
	#sf.Vector2f camera_origin(iwindow.origin.x, iwindow.origin.y)
	# and we create another one for the camera position
	#offset -= camera_origin
	# we get the relative coordinates of the vessels flag relative to the
	# camera origin 
	#offset.y = -offset.y
	# flip the y axis for SFML specific reasons
	#offset.y *= pow(0.1, (zoom_factor))
	#offset.x *= pow(0.1, (zoom_factor))
	
	#VectorVictor.Vector2 offset(Position.Get_x(), Position.Get_y())
	#VectorVictor.Vector2 camera_origin(iwindow.origin.Get_x(), iwindow.origin.Get_y())
	#offset -= camera_origin
	#offset.y *= -1
	#offset.y *= pow(0.1, (zoom_factor))
	#offset.x *= pow(0.1, (zoom_factor))
	
	camera_offset = Get_window_coordinates(NewtonianState.FlightState.Position, iwindow, zoom_factor)
	
	
	# scale the offset back by 1/(10^zoom), the offset in pixels needs
	# to be shrunk to fit the scale of the window
	Flag_sprite.setPosition(camera_offset)
	iwindow.window.draw(Flag_sprite)
	# and we finally locate the sprite in the window, draw it


def Get_vessel_class(self):
{	return Vessel_class
}	# the string of the given class

def Get_vessel_name(self):
{	return Object_name
}	# the string of the objects name

def Render_MFDs(self, &window, &keyCommands, &cursorCommands, double &cameraScale, double dt, &time_acceleration, double sim_time, &newtonians, &keplerians, &vessels, &Camera_target, double &Camera_rotation):
{	
	for(std.vector<MFD*>it = MFD_displays.begin(); it != MFD_displays.end(); ++it)
	{	(*it).Render_MFD(window, keyCommands, cursorCommands, cameraScale, dt, time_acceleration, sim_time, *self, newtonians, keplerians, vessels, Camera_target, Camera_rotation)
		# self looks good for now
		
		# tried changing what exactly gets sent to the MFD for the vessel
		# type, self will fix it
	} 


def Start_MFD(self, mfd_type, mfd_orientation, mfd_offset):
{	MFD * new_mfd
	if Create_MFD(mfd_type, new_mfd, mfd_orientation, mfd_offset) == True:
	{	MFD_displays.insert(MFD_displays.end(), new_mfd)
		return True

	return False


def Draw_MFDs(self, &window):
{	for(std.vector<MFD*>it = MFD_displays.begin(); it != MFD_displays.end(); ++it)
	{	(*it).Draw_MFD(window, sf.Color(255, 255, 255))
		# iterate through all of the MFDs that the vessel has and draw them to
		# the screen



def Get_Vessel_pointer(self):
{	return self

