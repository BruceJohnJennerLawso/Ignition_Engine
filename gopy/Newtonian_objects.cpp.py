#include <string>
#include <iostream>
#include <math.h>
#include "Newtonian_objects.hpp"



# Rotation State ###############################
# Properties of the object pertaining to rotation that we dont want to slave #
# to position & velocity if we dont have to #################/
# Although the question of why does come to mind ###############
########################################

Rotation_state.Rotation_state()
{	Theta = 0
	Omega = 0
	Alpha = 0


Rotation_state.Rotation_state(long double theta, double omega, double alpha)
{	Theta = theta
	Omega = omega
	Alpha = alpha


Rotation_state Rotation_state.operator= ( Rotation_state r)
	Theta = r.Theta
	Omega = r.Omega
	Alpha = r.Alpha;	
	return (*self);		
	# looks good enough


Rotation_state.~Rotation_state()


# Flight state ################################
# the properties of the object when we are in that wild blue yonder... ####
# just vector values for position and velocity ################
########################################

Flight_state.Flight_state()
{	self.Init_flight_state(VectorVictor.Vector2(), VectorVictor.Vector2())


Flight_state.Flight_state(VectorVictor.Vector2 initial_position, initial_velocity)
{	self.Init_flight_state(initial_position, initial_velocity)


def Init_flight_state(self, initial_position, initial_velocity):
{	Position = initial_position
	Velocity = initial_velocity
	return True


Flight_state Flight_state.operator= ( Flight_state s)
{	Position = s.Position
	Velocity = s.Velocity
	return (*self);		


Flight_state.~Flight_state()


# Surface state ###############################/
# Really basic info about a ship when it is on the ground, landed ###
# or crashed, what planet and the longitude that the object is at #/
# on said planet ###############################
########################################

Surface_state.Surface_state()
{	Planet_name = "NOWHERE"
	Longitude = 0


Surface_state.Surface_state(std.string planet_name, double longitude)
{	Planet_name = planet_name
	Longitude = longitude


Surface_state Surface_state.operator= ( Surface_state s)
{	Planet_name = s.Planet_name
	Longitude = s.Longitude
	return (*self);		


Surface_state.~Surface_state()
{	



# ObjectState ################################/
# All of the physical info that a Newtonian object needs to keep ticking ###
########################################

ObjectState.ObjectState()
{	Rotation = Rotation_state()

	FlightState.Position = VectorVictor.Vector2()
	# umm, think self works
	FlightState.Velocity = VectorVictor.Vector2()

	LandedState.Longitude = 0
	LandedState.Planet_name = "NOWHERE"

	Current_state = Flight
	# for lack of better information we say that our current state is in flight
	# at the origin with zero velocity


ObjectState.ObjectState(Flight_state initial_flight_state, rotation)
{	FlightState = initial_flight_state
	Rotation = rotation
	Current_state = Flight
	# implicitly based on the data we provided the state is in flight
	LandedState.Longitude = 0
	LandedState.Planet_name = "NOWHERE"
	# and we give the Landed state some default parameters so it uhh, has
	# something


ObjectState.ObjectState(Surface_state initial_landed_state, rotation)
{	LandedState = initial_landed_state
	Rotation = rotation
	Current_state = Landed
	# set it to landed, if we truly must have a starting crash state,
	# we can just call Set_status with Crashed after constructing the object
	FlightState.Position = VectorVictor.Vector2()
	# umm, think self works
	FlightState.Velocity = VectorVictor.Vector2()


ObjectState ObjectState.operator = ( ObjectState o)
{	Current_state = o.Current_state
	LandedState = o.LandedState
	FlightState = o.FlightState
	Rotation = o.Rotation
	return (*self)


def Set_status(self, new_status):
{	Current_state = new_status


ObjectState.~ObjectState()


# Newtonian Class ##############################/
########################################

CNewtonian_Object.CNewtonian_Object()
{	NewtonianState = ObjectState()
	# just stuff the default definition into it


CNewtonian_Object.CNewtonian_Object(ObjectState initial_newtonian_state)
{	NewtonianState = initial_newtonian_state


def Get_omega(self):
{	return self.NewtonianState.Rotation.Omega
	# value in degrees/s


def Get_theta_in_radians(self):
{	rad_theta = self.NewtonianState.Rotation.Theta
	rad_theta *= 6.283185308;		# 4/3 pau actually ;)
	rad_theta /= 360
	return rad_theta
	# in radians


def Get_theta_in_degrees(self):
{	return self.NewtonianState.Rotation.Theta
	# in degrees


long double CNewtonian_Object.Get_length()
{	return Length
	# in meters


def Get_hull_mass(self):
{	return -1
	# self is not supposed to be called at self level, it gets a error code
	# needs a talkback error message as well I think


def Get_total_mass(self):
{	net_mass = 0
	for(std.vector<Vessel_component*>it = Object_components.begin(); it != Object_components.end(); ++it)
	{	net_mass += (*it).Get_component_mass()
	}	
	# just query every part on the vessel for its mass and add it all up
	return net_mass



def Update_PMI(self):
{	PMI = 0
	for(std.vector<Vessel_component*>it = Object_components.begin(); it != Object_components.end(); ++it)
	{	PMI += (*it).Get_component_inertia((*it).Component_position)
		# once shifting cog is a thing, will need to shift the passed vector
		# by whatever offset the offset center of mass has
		
		# but we'll worry about that later
	}	
	# reset the moment of inertia to zero, add it up again from the
	# inertias contributed by each vessel part 


def Receive_inputs(self, * current_inputs, dt):
{	Talkback("Bad call to CNewtonian_Object.Receive_inputs(key_commands * current_inputs, dt)")


def Receive_cursor_inputs(self, * cursor_action, double dt):
{	Talkback("Bad call to CNewtonian_Object.Receive_cursor_inputs(Cursor_commands * cursor_action, double dt)")


def In_view(self, * window, zoom_factor):
{	Talkback("Bad call to CNewtonian_Object.In_view(SFML_Window * window, zoom_factor)")
	return False


def In_view(self, * window, double cam_scale):
{	Talkback("Bad call to CNewtonian_Object.In_view(SFML_Window * window, double cam_scale)")
	return False


def Print_data(self):
{	Talkback("Bad call to CNewtonian_Object.Print_data()")


long double CNewtonian_Object.Get_PMI()
{	return PMI


def Add_force(self, attack_point_x, attack_point_y, force_x, force_y):
{	VectorVictor.Vector2 vattack(attack_point_x, attack_point_y)
	VectorVictor.Vector2 vforce(force_x, force_y)
	# create VV2s for each component
	Force new_force(vattack, vforce)
	# use them to construct a force object
	Force_list.insert(Force_list.end(), new_force)
	# and push it back into the vector


def Add_force(self, attack_point, force_vector):
{	Force new_force(attack_point, force_vector)
	# create it
	Force_list.insert(Force_list.end(), new_force)
	# and insert it

	
def Frame(self, double dt, double simtime, &ignition_celestials):
{	
	# at the start of the call, forces are acting on the vessel
	for(std.vector<Vessel_component*>it = Object_components.begin(); it != Object_components.end(); ++it)
	{	(*it).Update_component(dt, Force_list);	
		# we run through all of the parts on the given vessel, let them 
		# update themselves. This is where parts that create a force,
		# like thrusters, wings, dragging in atmosphere, a force
		# on to the parent Newtonian Objects force list

	
	self.Update_motion(simtime, dt, ignition_celestials)
	# the vessel uses the forces acting upon it to update its motion...
	self.Update_rotation(simtime, dt)
	# ...and then does the same for rotation
	Update_PMI();						
	# we recalculate the vessels total moment of inertia
	# maybe a good candidate to do selectively, self may not be needed
	# every single frame
	Force_list.clear()
	# set the net force on the object back to zero for the next frame
	if NewtonianState.Current_state != Crashed:
	{	# if we arent crashed at the moment, if we are crashed
		if Crash_state(simtime, ignition_celestials) == True:
		{	# If we are crashed, our status to that
			std.cout << "Goodbye cruel world" << std.endl
			NewtonianState.Current_state = Crashed




def Crash_state(self, double sim_time, &ignition_celestials):
{	long offset_rad = 0
	for(std.vector<CKeplerian_Object*>it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
	{	offset_rad = VectorVictor.Get_vector_separation(NewtonianState.FlightState.Position, (*it).Get_position(sim_time))
		if(offset_rad < (*it).Get_radius(0))	# important to change self once terrain is a thing
		{	return True

		# iterate through all of the large bodies in the sim and see if we seem
		# to be 'underground'. If we are, we've probably bought the farm, we
		# set the Crashed state to True by returning True
		
		# self should eventually become something where we sweep out distances
		# over the course of the frame by using the previous frames position as
		# the other endpoint of the line

	return False
	# if not, continue on our merry way
	

	
def Update_motion(self, double simtime, double dt, &ignition_celestials):
{	
	switch(NewtonianState.Current_state)
	{	case Crashed:
		{	# do some stuffz
			# If we've made a crater in the planet, a surface update *TODO*
			# must write the functions preciousssss
			break

		case Landed:
		{	# do some other stuffz
		
			# roughly speaking, run a normal surface update, check if
			# our state is sufficient to get off the ground. If it is, set
			# the Current state to Flight, the next frame can take things
			# from there in the Flight propagation.
			break

		case Flight:
		{	# and do some really cool stuffz here
			
			VectorVictor.Vector2 Net_force(0,0)
			# start with a net force of 0
			for(std.vector<Force>it = Force_list.begin(); it != Force_list.end(); ++it)
			{	Net_force += it.Force_vector.Get_rotated_vector(self.NewtonianState.Rotation.Theta)
				# add up all of the forces acting on the vessel in self frame
				# * and very important, are rotating them back into the
				# global reference frame instead of the vessel local one that
				# they were created in *
				
				# SUPER TURBO CINTRAFALAGULOUS IMPORTANT

			
			if Propagator == RK4:
			{	self.Propagate_RK4(simtime, dt, Net_force, ignition_celestials)

			else:
			{	# we go to the forever alone brute force Euler
				# because nothing else was specified
				# self way is just so nothing is left uncaught
				self.Propagate_Euler1(simtime, dt, Net_force, ignition_celestials)

			break




# Euler Propagator ##############################

def Propagate_Euler1(self, double sim_time, double dt, &net_force, &ignition_celestials):
{		Acceleration.x = (net_force.x/Get_total_mass()); 
		Acceleration.y = (net_force.y/Get_total_mass());	
		# Newtons laws,
		
		# F = ma
		# a = F/m
		for(std.vector<CKeplerian_Object*>it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
		{	
			Acceleration += (*it).Gravity_acceleration(NewtonianState.FlightState.Position, sim_time)
			# we cycle through all of the large bodies in the universe
			# (probably excluding anything asteroid or smaller) and get them to add
			# a gravitational force to the vessel

		# we get the gravity accelerations from each of the celestial objects
		# in the sim
		NewtonianState.FlightState.Velocity.x += ((Acceleration.x)*dt)
		NewtonianState.FlightState.Velocity.y += ((Acceleration.y)*dt)
		# update velocity based on position
		NewtonianState.FlightState.Position.x += (((NewtonianState.FlightState.Velocity.x)*dt))
		NewtonianState.FlightState.Position.y += (((NewtonianState.FlightState.Velocity.y)*dt))
		# update position based on velocity










# Runge-Kutta 4 Propagator ##########################

def Propagate_RK4(self, double sim_time, double dt, &net_force, &ignition_celestials):
{	
	Flight_state a, b, c, d
	Flight_state initial_derivative
	initial_derivative.Position.Set_values(0,0)
	initial_derivative.Velocity.Set_values(0,0)
	
	a = evaluate(NewtonianState.FlightState, sim_time, 0.0, initial_derivative, ignition_celestials, net_force)
	# close our eyes and hope that works okay...
	
	# I guess.... I think the idea here is that we  get our initial derivative
	# state of a from the initial state that was passed to the function, and
	# dont step forward at all. We're just getting an initial state here
	b = evaluate(NewtonianState.FlightState, sim_time, dt*(0.5), a, ignition_celestials, net_force)
	# the previous operations insanity notwithstanding, do the same thing
	# again using whatever we got back from a
	
	# so we are working forward along the 'curvature of the problem', moving
	# forward and sampling acceleration and velocity based on what the initial
	# state obtained for a gave us for a half frame in length
	c = evaluate(NewtonianState.FlightState, sim_time, dt*(0.5), b, ignition_celestials, net_force)
	# and again with the results from b to get a.
	# evaluating with the half frame velocities and accels here?
	# I guess we equally weight the start and halfway vels and accels here???
	d = evaluate(NewtonianState.FlightState, sim_time, dt, c, ignition_celestials, net_force)
	# and... one last time with c
	
	#and at last, get the vels and accels at the end of the frame, self
	# makes sense
	
	velocity = (b.Position + c.Position)
	velocity *= 2.0
	velocity += (a.Position + d.Position)
	velocity *= (1.0/6.0)
	
	
	Acceleration = (b.Velocity + c.Velocity)
	Acceleration *= 2.0
	Acceleration += (a.Velocity + d.Velocity)
	Acceleration *= (1.0/6.0)
	
	# looks like we weight them according to some set of coefficients that the
	# given order of Runge Kutta specifies, the velocity and accel that
	# result are applied to the frame with a regular euler integration step,
	# minus the position term for constant acceleration
	# (already handled by the weighted velocity changes to the state I guess)   
	
	NewtonianState.FlightState.Position += (velocity*dt)
	NewtonianState.FlightState.Velocity += (Acceleration*dt)
	# and we lastly step ahead the basic state of the system by the values
	# we calculated as velocity and accel for the frame? Yeah, sounds
	# right


def evaluate(self, &initial_state, double simtime, double dt, &derivative, &ignition_celestials, &net_force):
{	bool nanState
		
	Flight_state state
	
	state.Position = derivative.Position
	state.Position *= dt;	
	state.Position += initial_state.Position
	state.Velocity = derivative.Velocity
	state.Velocity *= dt;	
	state.Velocity += initial_state.Velocity;	
	# I guess the last part is because its actually a*dt^2 or 
	# (dx/dt)*dt^2 = dx*dt	
	
	# for some reason we step things forward 1 frame dt seconds using a Euler
	# integrator
	Flight_state output
	output.Position = state.Velocity
	# we set the output derivatives dx equal to the velocity of the state that
	# we stepped forward earlier using the euler step
	Acceleration.x = (net_force.x/Get_total_mass()); 
	Acceleration.y = (net_force.y/Get_total_mass());	
	# get acceleration using Newtons laws
	for(std.vector<CKeplerian_Object*>it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
	{	
		Acceleration += (*it).Gravity_acceleration(state.Position, simtime)
		# we cycle through all of the large bodies in the universe
		# (probably excluding anything asteroid or smaller) and get them to add
		# a gravitational force to the vessel

	output.Velocity = Acceleration
	# and lastly we set dv equal to whatever the current acceleration is should
	# really be 
	return output


# Rotation update ##############################/

def Update_rotation(self, double simtime, double dt):
{	
	if NewtonianState.Current_state != Crashed:
	{	# again if weve crashed, bother
		self.NewtonianState.Rotation.Alpha = 0
		# where alpha is our angular acceleration, second derivative
		# of theta with respect to time
		for(std.vector<Force>it = Force_list.begin(); it != Force_list.end(); ++it)
		{	self.NewtonianState.Rotation.Alpha -= it.Get_force_torque()
			# work through all of the forces acting on the object, sum up
			# the torques they cause
			
			# odd that self is subtraction, be worth another look... 

		self.NewtonianState.Rotation.Alpha /= PMI
		# again, simple 121 physics,
		
		# T = I*Alpha
		# Alpha = T/I
		#self.NewtonianState.Rotation.Theta += ((self.NewtonianState.Rotation.Omega*dt)+(0.5*((self.Rotation.Alpha*dt)*dt)));	
		self.NewtonianState.Rotation.Omega += (self.NewtonianState.Rotation.Alpha*dt)
		self.NewtonianState.Rotation.Theta += ((self.NewtonianState.Rotation.Omega*dt));			
		# again, the equations of rotational motion applied here
		Object_sprite.setRotation(self.NewtonianState.Rotation.Theta)
		# make sure that the image drawn onscreen is synchronized with what 
		# happened here
		
		if self.NewtonianState.Rotation.Theta >= 360:
		{	while(self.NewtonianState.Rotation.Theta >= 360)
			{	self.NewtonianState.Rotation.Theta -= 360
			}	

		elif self.NewtonianState.Rotation.Theta < 0:
		{	while(self.NewtonianState.Rotation.Theta < 0)
			{	self.NewtonianState.Rotation.Theta += 360


		# if we run over the bounds of zero or 360, is technically
		# wrong (I think), self is better for understanding
		
		# self caused an occasional problem before, be best to
		# remove/relocate self so that its only done when saving state to
		# a scn file

	


def Get_Newtonian_pointer(self):
{	return self



