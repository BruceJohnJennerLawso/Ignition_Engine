#include <string>
#include <iostream>
#include <math.h>
#include "Newtonian_objects.hpp"



// Newtonian Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



// umm, so next step is to actually write the state propagator functions for
// each propagator type, ie PropagateRK4() and so on

// no constructor here, thats done by the child classes

double CNewtonian_Object::Get_omega()
{	return Omega;
	// value in degrees/s
}

double CNewtonian_Object::Get_theta_in_radians()
{	double rad_theta = Theta;
	rad_theta *= 6.283185308;		// 4/3 pau actually ;)
	rad_theta /= 360;
	return rad_theta;
	// in radians/s
}

double CNewtonian_Object::Get_theta_in_degrees()
{	return Theta;
	// in degrees
}

double CNewtonian_Object::Get_length()
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
	}	// just query every part on the vessel for its mass and add it all up
	return net_mass;

}

void CNewtonian_Object::Update_PMI()
{	PMI = 0;
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	PMI += (*it)->Get_component_inertia();
	}	// reset the moment of inertia to zero, then add it up again from the
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

void CNewtonian_Object::Print_data()
{	Talkback("Bad call to CNewtonian_Object::Print_data()");
}

// above 3 should not be called, just error messages

double CNewtonian_Object::Get_PMI()
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
{	// Important method here, many metaphorical wheels are turning

	// at the start of the call, no forces are acting on the vessel
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	(*it)->Update_component(dt, Force_list);	
		// we run through all of the parts on the given vessel, and let them 
		// update themselves. This is where parts that create a force,
		// like thrusters, wings, parts dragging in atmosphere, add a new force
		// on to the parent Newtonian Objects force list
	}
	
	
	//for(std::vector<CKeplerian_Object*>::iterator it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
	//{	(*it)->Gravitate(Get_total_mass(), Get_theta_in_degrees(), NewtonianState.FlightState.Position ,Force_list);
		// we cycle through all of the large bodies in the universe
		// (probably excluding anything asteroid or smaller) and get them to add
		// a gravitational force to the vessel
	//}
	// this cycling is on the way out I think
	
	
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
			{	Net_force += it->Force_vector.Get_rotated_vector(Theta);
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

void CNewtonian_Object::Propagate_Euler1(long double sim_time, long double dt, VectorVictor::Vector2 &net_force, std::vector<CKeplerian_Object*> &ignition_celestials)
{		Acceleration.x = (net_force.x/Get_total_mass()); 
		Acceleration.y = (net_force.y/Get_total_mass());	
		
		// Newtons laws, simply
		
		// F = ma
		// a = F/m
		
		for(std::vector<CKeplerian_Object*>::iterator it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
		{	
			Acceleration += (*it)->Gravity_acceleration(NewtonianState.FlightState.Position, sim_time);
			// we cycle through all of the large bodies in the universe
			// (probably excluding anything asteroid or smaller) and get them to add
			// a gravitational force to the vessel
			
			// this works nicely now, since the 
		}
		// and once we have done that, we get the gravity accelerations from
		// each of the celestial objects in the sim
		
		NewtonianState.FlightState.Velocity.x += ((Acceleration.x)*dt);
		NewtonianState.FlightState.Velocity.y += ((Acceleration.y)*dt);
		NewtonianState.FlightState.Position.x += (((NewtonianState.FlightState.Velocity.x)*dt) + (((0.500)*Acceleration.x)*dt*dt));
		NewtonianState.FlightState.Position.y += (((NewtonianState.FlightState.Velocity.y)*dt) + (((0.500)*Acceleration.y)*dt*dt));
		// Simple constant acceleration equations of motion	
}

void CNewtonian_Object::Propagate_RK4(long double sim_time, long double dt, VectorVictor::Vector2 &net_force, std::vector<CKeplerian_Object*> &ignition_celestials)
{	// ah, so yaa

	Flight_state a, b, c, d;
	
	a = evaluate(NewtonianState.FlightState, sim_time, 0.0f, Flight_state(), ignition_celestials, net_force);
	// close our eyes and hope that works okay...
	
	// I guess.... I think the idea here is that we  get our initial derivative
	// state of a from the initial state that was passed to the function, and
	// dont step forward at all. We're just getting an initial state here
	b = evaluate(NewtonianState.FlightState, sim_time, dt*(0.5f), a, ignition_celestials, net_force);
	// the previous operations insanity notwithstanding, we do the same thing
	// again using whatever we got back from a
	
	// so we are working forward along the 'curvature of the problem', moving
	// forward and sampling acceleration and velocity based on what the initial
	// state obtained for a gave us for a half frame in length
	c = evaluate(NewtonianState.FlightState, sim_time, dt*(0.5f), b, ignition_celestials, net_force);
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
	
	//VectorVictor::Vector2 velocity = ((1.0/6.0)*(a.Position + ((b.Position + c.Position)*2.0) + d.Position));
	
	//Acceleration = ((1.0/6.0)*(a.Velocity + 2.0*(b.Velocity + c.Velocity) + d.Velocity));
	
	Acceleration = (b.Velocity + c.Velocity);
	Acceleration *= 2.0;
	Acceleration += (a.Velocity + d.Velocity);
	Acceleration *= (1.0/6.0);

	//VectorVictor::Vector2 velocity = ((1.0/6.0)*(a.Position + ((b.Position + c.Position)*2.0) + d.Position));
	//Acceleration = ((1.0/6.0)*(a.Velocity + 2.0*(b.Velocity + c.Velocity) + d.Velocity));
	// we do some odd thing here to get a frame-wide accel and velocity?
	// I think? I do recognize these as the RK4 scheme coefficients
	
	// I wanted to do it straight up like this, but my standard operators arent
	// quite up to the challenge yet :(
	
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
	
	// If this works first time, my mind will be blown
}

Flight_state CNewtonian_Object::evaluate(const Flight_state &initial_state, long double simtime, long double dt, const Flight_state &derivative, std::vector<CKeplerian_Object*> &ignition_celestials, VectorVictor::Vector2 &net_force)
{	Flight_state state;
	
	//state.x = initial_state.x + derivative.dx*dt;
	
	//state.Position = initial_state.Position + ((derivative.Position)*dt);
	
	state.Position = derivative.Position;
	state.Position *= dt;	
	state.Position += initial_state.Position;
	
	// I guess the last part is because its actually a*dt^2 or 
	// (dx/dt)*dt^2 = dx*dt
	
	//state.Velocity = initial_state.Velocity + derivative.Velocity)*dt);
	
	state.Velocity = derivative.Velocity;
	state.Position *= dt;	
	state.Velocity += initial_state.Velocity;	
	
	// for some reason we step things forward 1 frame dt seconds using a Euler
	// integrator
	
	//Derivative output;
	Flight_state output;
	
	output.Position = state.Velocity;
	// we set the output derivatives dx equal to the velocity of the state that
	// we stepped forward earlier using the euler step
	
	Acceleration.x = (net_force.x/Get_total_mass()); 
	Acceleration.y = (net_force.y/Get_total_mass());	
	
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


void CNewtonian_Object::Update_rotation(long double simtime, long double dt)
{	
	if(NewtonianState.Current_state != Crashed)
	{	// again if weve crashed, dont bother
		Alpha = 0;
		// where alpha is our angular acceleration, the second derivative
		// of theta with respect to time
		for(std::vector<Force>::iterator it = Force_list.begin(); it != Force_list.end(); ++it)
		{	Alpha -= it->Get_force_torque();
			// work through all of the forces acting on the object, and sum up
			// the torques they cause
			
			// odd that this is subtraction, might be worth another look... 
		}
		Alpha /= PMI;
		// again, from simple 121 physics,
		
		// T = I*Alpha
		// Alpha = T/I
		Theta += ((Omega*dt)+(0.5*((Alpha*dt)*dt)));
		Omega += (Alpha*dt);
		// again, just the equations of rotational motion applied here
		Object_sprite->setRotation(Theta);
		// make sure that the image drawn onscreen is synchronized with what 
		// happened here
		
		if(Theta >= 360)
		{	while(Theta >= 360)
			{	Theta -= 360;
			}	
		}
		else if(Theta < 0)
		{	while(Theta < 0)
			{	Theta += 360;
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

// TVessel Class ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void TVessel::Init_vessel_type()
{	
}

// getting a little more specific at this point, dealing with specific
// functionality that a spacecraft really needs to have

void TVessel::Rotate_left(double dt)
{	std::cout << "Bad call to TVessel::Rotate_left(double dt) " << std::endl;
}

void TVessel::Rotate_right(double dt)
{	std::cout << "Bad call to TVessel::Rotate_right(double dt) " << std::endl;
}

void TVessel::Kill_rotation(double dt)	
{	std::cout << "Bad call to TVessel::Kill_rotation(double dt) " << std::endl;
}

void TVessel::Translate_forward(double dt)
{	std::cout << "Bad call to TVessel::Translate_forward(double dt) " << std::endl;
}

void TVessel::Translate_backward(double dt)
{	std::cout << "Bad call to TVessel::Translate_backward(double dt) " << std::endl;
}

void TVessel::Translate_left(double dt)
{	std::cout << "Bad call to TVessel::Translate_left(double dt) " << std::endl;
}

void TVessel::Translate_right(double dt)
{	std::cout << "Bad call to TVessel::Translate_right(double dt) " << std::endl;
}

void TVessel::Throttle_up(double dt)
{	std::cout << "Bad call to TVessel::Throttle_up(double dt) " << std::endl;
}

void TVessel::Throttle_down(double dt)
{	std::cout << "Bad call to TVessel::Throttle_down(double dt) " << std::endl;
}

void TVessel::No_command(double dt)
{	std::cout << "Bad call to TVessel::No_command(double dt) " << std::endl;
}

// all not handled at this level, but had to be defined anyways

void TVessel::Draw_controls(SFML_Window * iwindow, bool Map_status)
{	// should be a warning message here too...
}

bool TVessel::Init_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_id)
{	Thruster * new_thruster = new Monopropellant_thruster(is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, fuel_tank, group_id);
	this->Object_components.insert(this->Object_components.end(), new_thruster->Get_vessel_component_pointer());	
	Thrusters.insert(Thrusters.end(), new_thruster);
	return true;
}
	
bool TVessel::Init_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_id1, thruster_group group_id2)
{	Thruster * new_thruster = new Monopropellant_thruster(is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, fuel_tank, group_id1, group_id2);
	this->Object_components.insert(this->Object_components.end(), new_thruster->Get_vessel_component_pointer());	
	Thrusters.insert(Thrusters.end(), new_thruster);
	return true;
}

bool TVessel::Init_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_id1, thruster_group group_id2, thruster_group group_id3)
{	Thruster * new_thruster = new Monopropellant_thruster(is_rcs, thruster_mass, vexhaust, max_flow_rate, position_x, position_y, direction_x, direction_y, inner_radius, outer_radius, fuel_tank, group_id1, group_id2, group_id3);
	this->Object_components.insert(this->Object_components.end(), new_thruster->Get_vessel_component_pointer());	
	Thrusters.insert(Thrusters.end(), new_thruster);	
	return true;
}

long double TVessel::Get_max_alpha(rotation_direction direction)
{	thruster_group rotation_group;
	if(direction == clockwise)
	{	rotation_group = rotate_clockwise;
	}
	else
	{	rotation_group = rotate_counterclockwise;
	}
	
	long double max_alpha = 0;
	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotation_group))
		{	max_alpha -= (*it)->Get_maximum_torque(1);
			// I need to remove this dt thing, it isnt right
		}
	}
	max_alpha /= Get_PMI();
	return max_alpha;
}


bool TVessel::In_view(SFML_Window * window, int zoom_factor)
{	if((NewtonianState.FlightState.Position.Get_x() >= (window->origin.x-(Hull_component->Get_hull_length_squared())))&&(NewtonianState.FlightState.Position.Get_x() <= ((window->origin.x + (window->Aperture_width+(Hull_component->Get_hull_length_squared()))))))
	{	if((NewtonianState.FlightState.Position.Get_y() <= (window->origin.y+(Hull_component->Get_hull_length_squared())))&&(NewtonianState.FlightState.Position.Get_y() >= (window->origin.y - ((window->Aperture_height+(Hull_component->Get_hull_length_squared()))))))
		{	return true;
		}
		else
		{	return false;
		}
	}	
	else
	{	return false;
	}
	// just another simple function for determining if the ship should be drawn
	// there was some logic behind squaring the distance, but I dont recall
}

void TVessel::Draw_vessel(SFML_Window * iwindow, double cam_scale)
{	// very simple to the draw flag for TPlanets, save a few minor details
	//sf::Vector2f offset(Position.Get_x(), Position.Get_y());
	// create a sf vector representing the in-universe coords of the vessel
	//sf::Vector2f camera_origin(iwindow->origin.x, iwindow->origin.y);
	// and another sf vector representing the camera origin
	// (in this case the top left corner of the camera)
	//offset -= camera_origin;
	// we find the relative offset from the camera to the object
	//offset.y = -offset.y;
	//offset.x *= 10;
	//offset.y *= 10;
	// flip the y axis for SFML specific reasons
	//Object_sprite->setPosition(offset);
	// we locate the sprite in the right part of the window
	
	//VectorVictor::Vector2 offset(Position.Get_x(), Position.Get_y());
	//VectorVictor::Vector2 camera_origin(iwindow->origin.Get_x(), iwindow->origin.Get_y());
	//offset -= camera_origin;
	//offset.y *= -1;
	//offset.x *= (10.00000000/(long double)cam_scale);
	//offset.y *= (10.00000000/(long double)cam_scale);
	
	Object_sprite->setScale((pix_length/((Vessel_tex->getSize().y)*cam_scale)),(pix_length/((Vessel_tex->getSize().y)*cam_scale)));
	// rescale the axes of the texture to match pix_length in the y and the
	// appropriate scale for the x dimension
		
	// not sure why they both use y, but I think this was distorted when
	// it used x & y.
	// This area needs to be looked over again
		
	
	sf::Vector2f camera_offset = Get_window_coordinates(NewtonianState.FlightState.Position, iwindow, cam_scale);
	Object_sprite->setPosition(camera_offset);
	//k lets see what that did
	
	// exactly what I thought, caused by the use of sf::Vector2f instead of
	// the higher precision long doubles in VV2
	
	// okay so here we replace the old order of operations in drawing the vessel
	// in order to kill the jitterbug
	
	//offset += sf::Vector2f(40, -40);		// never did figure this part out...
	// this, hmmm... I dont reallly know what this is
	// this might have been a really early workaround in the code, should just
	// scrap it
	Flag_sprite->setPosition(camera_offset + sf::Vector2f((10*Hull_component->Length), -(10*Hull_component->Length)));
	// draws the flag near the vessel for some reason.
	// really just window-dressing here, not necessary
	
	// probably should be removed. If this is done again, it will
	// be done at a different level
	iwindow->window->draw(*Object_sprite);
	//iwindow->window->draw(*Flag_sprite);
	// and we draw them onto the window, simple as it sounds
}

void TVessel::Draw_flag(SFML_Window * iwindow, int zoom_factor)
{	//sf::Vector2f offset(Position.Get_x(), Position.Get_y());	
	// aaand we create our sf vector for the flag position
	//sf::Vector2f camera_origin(iwindow->origin.x, iwindow->origin.y);
	// and we create another one for the camera position
	//offset -= camera_origin;
	// we get the relative coordinates of the vessels flag relative to the
	// camera origin 
	//offset.y = -offset.y;
	// flip the y axis for SFML specific reasons
	//offset.y *= pow(0.1, (zoom_factor));
	//offset.x *= pow(0.1, (zoom_factor));
	
	//VectorVictor::Vector2 offset(Position.Get_x(), Position.Get_y());
	//VectorVictor::Vector2 camera_origin(iwindow->origin.Get_x(), iwindow->origin.Get_y());
	//offset -= camera_origin;
	//offset.y *= -1;
	//offset.y *= pow(0.1, (zoom_factor));
	//offset.x *= pow(0.1, (zoom_factor));
	
	sf::Vector2f camera_offset = Get_window_coordinates(NewtonianState.FlightState.Position, iwindow, zoom_factor);
	
	
	// scale the offset back by 1/(10^zoom), since the offset in pixels needs
	// to be shrunk to fit the scale of the window
	Flag_sprite->setPosition(camera_offset);
	iwindow->window->draw(*Flag_sprite);
	// and we finally locate the sprite in the window, and draw it
}

std::string TVessel::Get_vessel_class()
{	return Vessel_class;
}	// the string of the given class

std::string TVessel::Get_vessel_name()
{	return Object_name;
}	// the string of the objects name

TVessel* TVessel::Get_Vessel_pointer()
{	return this;
}


// Current Vessel classes //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -Delta Glider

