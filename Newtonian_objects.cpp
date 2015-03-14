#include <string>
#include <iostream>
#include <math.h>
#include "Newtonian_objects.hpp"



// Rotation State //////////////////////////////////////////////////////////////
// Properties of the object pertaining to rotation that we dont want to slave //
// to position & velocity if we dont have to ///////////////////////////////////
// Although the question of why does come to mind //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Rotation_state::Rotation_state()
{	Theta = 0;
	Omega = 0;
	Alpha = 0;
}

Rotation_state::Rotation_state(long double theta, long double omega, long double alpha)
{	Theta = theta;
	Omega = omega;
	Alpha = alpha;
}

Rotation_state Rotation_state::operator= (const Rotation_state r)
{
	Theta = r.Theta;
	Omega = r.Omega;
	Alpha = r.Alpha;	
	return (*this);		
	// looks good enough
}

Rotation_state::~Rotation_state()
{
}

// Flight state ////////////////////////////////////////////////////////////////
// the properties of the object when we are in that wild blue yonder... ////////
// just vector values for position and velocity ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Flight_state::Flight_state()
{	this->Init_flight_state(VectorVictor::Vector2(), VectorVictor::Vector2());
}

Flight_state::Flight_state(VectorVictor::Vector2 initial_position, VectorVictor::Vector2 initial_velocity)
{	this->Init_flight_state(initial_position, initial_velocity);
}

bool Flight_state::Init_flight_state(VectorVictor::Vector2 initial_position, VectorVictor::Vector2 initial_velocity)
{	Position = initial_position;
	Velocity = initial_velocity;
	return true;
}

Flight_state Flight_state::operator= (const Flight_state s)
{	Position = s.Position;
	Velocity = s.Velocity;
	return (*this);		
}

Flight_state::~Flight_state()
{
}

// Surface state ///////////////////////////////////////////////////////////////
// Really basic info about a ship when it is on the ground, either landed //////
// or crashed, including what planet and the longitude that the object is at ///
// on said planet //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Surface_state::Surface_state()
{	Planet_name = "NOWHERE";
	Longitude = 0;
}

Surface_state::Surface_state(std::string planet_name, long double longitude)
{	Planet_name = planet_name;
	Longitude = longitude;
}

Surface_state Surface_state::operator= (const Surface_state s)
{	Planet_name = s.Planet_name;
	Longitude = s.Longitude;
	return (*this);		
}

Surface_state::~Surface_state()
{	
}


// ObjectState /////////////////////////////////////////////////////////////////
// All of the physical info that a Newtonian object needs to keep ticking //////
////////////////////////////////////////////////////////////////////////////////

ObjectState::ObjectState()
{	Rotation = Rotation_state();

	FlightState.Position = VectorVictor::Vector2();
	// umm, I think this works
	FlightState.Velocity = VectorVictor::Vector2();

	LandedState.Longitude = 0;
	LandedState.Planet_name = "NOWHERE";

	Current_state = Flight;
	// for lack of better information we say that our current state is in flight
	// at the origin with zero velocity
}

ObjectState::ObjectState(Flight_state initial_flight_state, Rotation_state rotation)
{	FlightState = initial_flight_state;
	Rotation = rotation;
	Current_state = Flight;
	// implicitly based on the data we provided the state is in flight
	LandedState.Longitude = 0;
	LandedState.Planet_name = "NOWHERE";
	// and we give the Landed state some default parameters so it uhh, has
	// something
}

ObjectState::ObjectState(Surface_state initial_landed_state, Rotation_state rotation)
{	LandedState = initial_landed_state;
	Rotation = rotation;
	Current_state = Landed;
	// set it to landed, and if we truly must have a starting crash state,
	// we can just call Set_status with Crashed after constructing the object
	FlightState.Position = VectorVictor::Vector2();
	// umm, I think this works
	FlightState.Velocity = VectorVictor::Vector2();
}

ObjectState ObjectState::operator = (const ObjectState o)
{	Current_state = o.Current_state;
	LandedState = o.LandedState;
	FlightState = o.FlightState;
	Rotation = o.Rotation;
	return (*this);
}

void ObjectState::Set_status(Object_status new_status)
{	Current_state = new_status;
}

ObjectState::~ObjectState()
{
}

// Newtonian Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CNewtonian_Object::CNewtonian_Object()
{	NewtonianState = ObjectState();
	// just stuff the default definition into it
}

CNewtonian_Object::CNewtonian_Object(ObjectState initial_newtonian_state)
{	NewtonianState = initial_newtonian_state;
}

double CNewtonian_Object::Get_omega()
{	return this->NewtonianState.Rotation.Omega;
	// value in degrees/s
}

double CNewtonian_Object::Get_theta_in_radians()
{	double rad_theta = this->NewtonianState.Rotation.Theta;
	rad_theta *= 6.283185308;		// 4/3 pau actually ;)
	rad_theta /= 360;
	return rad_theta;
	// in radians
}

double CNewtonian_Object::Get_theta_in_degrees()
{	return this->NewtonianState.Rotation.Theta;
	// in degrees
}

long double CNewtonian_Object::Get_length()
{	return Length;
	// in meters
}

double CNewtonian_Object::Get_hull_mass()
{	return -1;
	// this is not supposed to be called at this level, so it gets a error code
	// needs a talkback error message as well I think
}

double CNewtonian_Object::Get_total_mass()
{	double net_mass = 0;
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	net_mass += (*it)->Get_component_mass();
	}	
	// just query every part on the vessel for its mass and add it all up
	return net_mass;

}

void CNewtonian_Object::Update_PMI()
{	PMI = 0;
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	PMI += (*it)->Get_component_inertia((*it)->Component_position);
		// once shifting cog is a thing, we will need to shift the passed vector
		// by whatever offset the offset center of mass has
		
		// but we'll worry about that later
	}	
	// reset the moment of inertia to zero, then add it up again from the
	// inertias contributed by each vessel part 
}

void CNewtonian_Object::Receive_inputs(key_commands * current_inputs, double dt)
{	Talkback("Bad call to CNewtonian_Object::Receive_inputs(key_commands * current_inputs, double dt)");
}

void CNewtonian_Object::Receive_cursor_inputs(Cursor_commands * cursor_action, long double dt)
{	Talkback("Bad call to CNewtonian_Object::Receive_cursor_inputs(Cursor_commands * cursor_action, long double dt)");
}

bool CNewtonian_Object::In_view(SFML_Window * window, int zoom_factor)
{	Talkback("Bad call to CNewtonian_Object::In_view(SFML_Window * window, int zoom_factor)");
	return false;
}

bool CNewtonian_Object::In_view(SFML_Window * window, long double cam_scale)
{	Talkback("Bad call to CNewtonian_Object::In_view(SFML_Window * window, long double cam_scale)");
}

void CNewtonian_Object::Print_data()
{	Talkback("Bad call to CNewtonian_Object::Print_data()");
}

long double CNewtonian_Object::Get_PMI()
{	return PMI;
}

void CNewtonian_Object::Add_force(double attack_point_x, double attack_point_y, double force_x, double force_y)
{	VectorVictor::Vector2 vattack(attack_point_x, attack_point_y);
	VectorVictor::Vector2 vforce(force_x, force_y);
	// create VV2s for each component
	Force new_force(vattack, vforce);
	// use them to construct a force object
	Force_list.insert(Force_list.end(), new_force);
	// and push it back into the vector
}

void CNewtonian_Object::Add_force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force_vector)
{	Force new_force(attack_point, force_vector);
	// create it
	Force_list.insert(Force_list.end(), new_force);
	// and insert it
}
	
void CNewtonian_Object::Frame(long double dt, long double simtime, std::vector<CKeplerian_Object*> &ignition_celestials)
{	
	// at the start of the call, no forces are acting on the vessel
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	(*it)->Update_component(dt, Force_list);	
		// we run through all of the parts on the given vessel, and let them 
		// update themselves. This is where parts that create a force,
		// like thrusters, wings, parts dragging in atmosphere, add a new force
		// on to the parent Newtonian Objects force list
	}
	
	this->Update_motion(simtime, dt, ignition_celestials);
	// the vessel uses the forces acting upon it to update its motion...
	this->Update_rotation(simtime, dt);
	// ...and then does the same for rotation
	Update_PMI();						
	// we recalculate the vessels total moment of inertia
	// maybe a good candidate to do selectively, since this may not be needed
	// every single frame
	Force_list.clear();
	// set the net force on the object back to zero for the next frame
	
	if(NewtonianState.Current_state != Crashed)
	{	// if we arent crashed at the moment, check if we are crashed
		if(Crash_state(simtime, ignition_celestials) == true)
		{	// If we are crashed, set our status to that
			std::cout << "Goodbye cruel world" << std::endl;
			NewtonianState.Current_state = Crashed;
		}
	}
	Collision_checked = false;
	// set our flag so that the object will be checked for any collisions
}

void CNewtonian_Object::Collision_detect(std::vector<CNewtonian_Object*> &collidable_objects)
{	// this is gonna be a big one...
	VectorVictor::Vector2 offset(0,0);
	for(std::vector<CNewtonian_Object*>::iterator it = collidable_objects.begin(); it != collidable_objects.end(); ++it)
	{	if((*it)->Collision_checked == false)
		{	// check and see if we are collided
			
			if(this->Get_collision_bound().Intersection((*it)->Get_collision_bound(), offset)) == true)
			{	// ladies and gentlemen, we have a collision
			
				// now what are we gonna do about it? :P
				
				// in pseudocode description
				
				// VV2 V1 (*this object) is proj of (*this).velocity onto the offset
				
				// VV2 V2 (*it object) is proj of (*it).velocity onto the offset
				
				// we subtract the component velocities normal to the collision
				// plane from the original velocity vector so we are only left
				// with the velocity component that wont be affected by the
				// collision (ie is parallel to the collision plane)
				
				// and for simplicity, we just get the velocity vectors that
				// will be involved in the collision as their magnitudes in
				// long double format
				
				// long double v1 = V1.Get_vector_magnitude()
				// long double v2 = V2.Get_vector_magnitude()
				
				// we normalize V1 & V2 to just get absolute directions of the
				// relative velocities, then we do comparisons of whether the
				// two vectors are both 0, antiparallel pointing in,
				// antiparallel pointing out, parallel, etc.
				// since each case will kinda need its own solution depending
				// on the relative directions of V1 & V2
				
				// we solve whatever case popped up using the proper pair of
				// energy conserved and momentum conserved equations
				// (potentially energy could not be conserved based on some
				// coefficient that each vessel supplies)
				
				// finally, we apply the new values of 
				
				
			}
		}
	}
}

Circle CNewtonian_Object::Get_collision_bound()
{	Circle output(this->NewtonianState.FlightState.Position, this->Get_radius());
	return output;
}

long double CNewtonian_Object::Get_radius()
{	Talkback("Bad call to CNewtonian_Object::Get_radius()");
	return -1;
}

bool CNewtonian_Object::Crash_state(long double sim_time, std::vector<CKeplerian_Object*> &ignition_celestials)
{	long double offset_rad = 0;
	for(std::vector<CKeplerian_Object*>::iterator it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
	{	offset_rad = VectorVictor::Get_vector_separation(NewtonianState.FlightState.Position, (*it)->Get_position(sim_time));
		if(offset_rad < (*it)->Get_radius(0))	// important to change this once terrain is a thing
		{	return true;
		}
		// iterate through all of the large bodies in the sim and see if we seem
		// to be 'underground'. If we are, we've probably bought the farm, so we
		// set the Crashed state to true by returning true
		
		// this should eventually become something where we sweep out distances
		// over the course of the frame by using the previous frames position as
		// the other endpoint of the line
	}
	return false;
	// if not, we continue on our merry way
	
}
	
void CNewtonian_Object::Update_motion(long double simtime, long double dt, std::vector<CKeplerian_Object*> &ignition_celestials)
{	
	switch(NewtonianState.Current_state)
	{	case Crashed:
		{	// do some stuffz
			// If we've made a crater in the planet, run a surface update *TODO*
			// must write the functions preciousssss
			break;
		}
		case Landed:
		{	// do some other stuffz
		
			// roughly speaking, just run a normal surface update, and check if
			// our state is sufficient to get off the ground. If it is, we set
			// the Current state to Flight, and the next frame can take things
			// from there in the Flight propagation.
			break;
		}
		case Flight:
		{	// and do some really cool stuffz here
			
			VectorVictor::Vector2 Net_force(0,0);
			// start with a net force of 0
			for(std::vector<Force>::iterator it = Force_list.begin(); it != Force_list.end(); ++it)
			{	Net_force += it->Force_vector.Get_rotated_vector(this->NewtonianState.Rotation.Theta);
				// add up all of the forces acting on the vessel in this frame
				// * and very important, we are rotating them back into the
				// global reference frame instead of the vessel local one that
				// they were created in *
				
				// SUPER TURBO CINTRAFALAGULOUS IMPORTANT
			}
			
			if(Propagator == RK4)
			{	this->Propagate_RK4(simtime, dt, Net_force, ignition_celestials);
			}
			else
			{	// we go to the forever alone brute force Euler
				// because nothing else was specified
				// this way is just so nothing is left uncaught
				this->Propagate_Euler1(simtime, dt, Net_force, ignition_celestials);
			}
			break;
		}
	}
}

// Euler Propagator ////////////////////////////////////////////////////////////

void CNewtonian_Object::Propagate_Euler1(long double sim_time, long double dt, VectorVictor::Vector2 &net_force, std::vector<CKeplerian_Object*> &ignition_celestials)
{		Acceleration.x = (net_force.x/Get_total_mass()); 
		Acceleration.y = (net_force.y/Get_total_mass());	
		// Newtons laws,
		
		// F = ma
		// a = F/m
		for(std::vector<CKeplerian_Object*>::iterator it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
		{	
			Acceleration += (*it)->Gravity_acceleration(NewtonianState.FlightState.Position, sim_time);
			// we cycle through all of the large bodies in the universe
			// (probably excluding anything asteroid or smaller) and get them to add
			// a gravitational force to the vessel
		}
		// we get the gravity accelerations from each of the celestial objects
		// in the sim
		NewtonianState.FlightState.Velocity.x += ((Acceleration.x)*dt);
		NewtonianState.FlightState.Velocity.y += ((Acceleration.y)*dt);
		// update velocity based on position
		NewtonianState.FlightState.Position.x += (((NewtonianState.FlightState.Velocity.x)*dt));
		NewtonianState.FlightState.Position.y += (((NewtonianState.FlightState.Velocity.y)*dt));
		// update position based on velocity
}









// Runge-Kutta 4 Propagator ////////////////////////////////////////////////////

void CNewtonian_Object::Propagate_RK4(long double sim_time, long double dt, VectorVictor::Vector2 &net_force, std::vector<CKeplerian_Object*> &ignition_celestials)
{	
	Flight_state a, b, c, d;
	Flight_state initial_derivative;
	initial_derivative.Position.Set_values(0,0);
	initial_derivative.Velocity.Set_values(0,0);
	
	a = evaluate(NewtonianState.FlightState, sim_time, 0.0, initial_derivative, ignition_celestials, net_force);
	// close our eyes and hope that works okay...
	
	// I guess.... I think the idea here is that we  get our initial derivative
	// state of a from the initial state that was passed to the function, and
	// dont step forward at all. We're just getting an initial state here
	b = evaluate(NewtonianState.FlightState, sim_time, dt*(0.5), a, ignition_celestials, net_force);
	// the previous operations insanity notwithstanding, we do the same thing
	// again using whatever we got back from a
	
	// so we are working forward along the 'curvature of the problem', moving
	// forward and sampling acceleration and velocity based on what the initial
	// state obtained for a gave us for a half frame in length
	c = evaluate(NewtonianState.FlightState, sim_time, dt*(0.5), b, ignition_celestials, net_force);
	// and again with the results from b to get a.
	// evaluating with the half frame velocities and accels here?
	// I guess we equally weight the start and halfway vels and accels here???
	d = evaluate(NewtonianState.FlightState, sim_time, dt, c, ignition_celestials, net_force);
	// and... one last time with c
	
	//and at last, we get the vels and accels at the end of the frame, now this
	// makes sense
	
	VectorVictor::Vector2 velocity = (b.Position + c.Position);
	velocity *= 2.0;
	velocity += (a.Position + d.Position);
	velocity *= (1.0/6.0);
	
	
	Acceleration = (b.Velocity + c.Velocity);
	Acceleration *= 2.0;
	Acceleration += (a.Velocity + d.Velocity);
	Acceleration *= (1.0/6.0);
	
	// looks like we weight them according to some set of coefficients that the
	// given order of Runge Kutta specifies, and the velocity and accel that
	// result are applied to the frame with a regular euler integration step,
	// minus the position term for constant acceleration
	// (already handled by the weighted velocity changes to the state I guess)   
	
	NewtonianState.FlightState.Position += (velocity*dt);
	NewtonianState.FlightState.Velocity += (Acceleration*dt);
	// and we lastly step ahead the basic state of the system by the values
	// we calculated as velocity and accel for the frame? Yeah, that sounds
	// right
}

Flight_state CNewtonian_Object::evaluate(const Flight_state &initial_state, long double simtime, long double dt, const Flight_state &derivative, std::vector<CKeplerian_Object*> &ignition_celestials, VectorVictor::Vector2 &net_force)
{	bool nanState;
		
	Flight_state state;
	
	state.Position = derivative.Position;
	state.Position *= dt;	
	state.Position += initial_state.Position;
	state.Velocity = derivative.Velocity;
	state.Velocity *= dt;	
	state.Velocity += initial_state.Velocity;	
	// I guess the last part is because its actually a*dt^2 or 
	// (dx/dt)*dt^2 = dx*dt	
	
	// for some reason we step things forward 1 frame dt seconds using a Euler
	// integrator
	Flight_state output;
	output.Position = state.Velocity;
	// we set the output derivatives dx equal to the velocity of the state that
	// we stepped forward earlier using the euler step
	Acceleration.x = (net_force.x/Get_total_mass()); 
	Acceleration.y = (net_force.y/Get_total_mass());	
	// get acceleration using Newtons laws
	for(std::vector<CKeplerian_Object*>::iterator it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
	{	
		Acceleration += (*it)->Gravity_acceleration(state.Position, simtime);
		// we cycle through all of the large bodies in the universe
		// (probably excluding anything asteroid or smaller) and get them to add
		// a gravitational force to the vessel
	}
	output.Velocity = Acceleration;
	// and lastly we set dv equal to whatever the current acceleration is should
	// really be 
	return output;
}

// Rotation update /////////////////////////////////////////////////////////////

void CNewtonian_Object::Update_rotation(long double simtime, long double dt)
{	
	if(NewtonianState.Current_state != Crashed)
	{	// again if weve crashed, dont bother
		this->NewtonianState.Rotation.Alpha = 0;
		// where alpha is our angular acceleration, the second derivative
		// of theta with respect to time
		for(std::vector<Force>::iterator it = Force_list.begin(); it != Force_list.end(); ++it)
		{	this->NewtonianState.Rotation.Alpha -= it->Get_force_torque();
			// work through all of the forces acting on the object, and sum up
			// the torques they cause
			
			// odd that this is subtraction, might be worth another look... 
		}
		this->NewtonianState.Rotation.Alpha /= PMI;
		// again, from simple 121 physics,
		
		// T = I*Alpha
		// Alpha = T/I
		//this->NewtonianState.Rotation.Theta += ((this->NewtonianState.Rotation.Omega*dt)+(0.5*((this->Rotation.Alpha*dt)*dt)));	
		this->NewtonianState.Rotation.Omega += (this->NewtonianState.Rotation.Alpha*dt);
		this->NewtonianState.Rotation.Theta += ((this->NewtonianState.Rotation.Omega*dt));			
		// again, just the equations of rotational motion applied here
		Object_sprite->setRotation(this->NewtonianState.Rotation.Theta);
		// make sure that the image drawn onscreen is synchronized with what 
		// happened here
		
		if(this->NewtonianState.Rotation.Theta >= 360)
		{	while(this->NewtonianState.Rotation.Theta >= 360)
			{	this->NewtonianState.Rotation.Theta -= 360;
			}	
		}
		else if(this->NewtonianState.Rotation.Theta < 0)
		{	while(this->NewtonianState.Rotation.Theta < 0)
			{	this->NewtonianState.Rotation.Theta += 360;
			}
		}
		// if we run over the bounds of zero or 360, nothing is technically
		// wrong (I think), but this is better for understanding
		
		// this caused an occasional problem before, might be best to
		// remove/relocate this so that its only done when saving state to
		// a scn file
	}
	
}

CNewtonian_Object* CNewtonian_Object::Get_Newtonian_pointer()
{	return this;
}


