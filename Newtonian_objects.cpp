#include <string>
#include <iostream>
#include <math.h>
#include "Newtonian_objects.hpp"


// Vessel Component parent class ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void Vessel_component::Draw_component(SFML_Window * iwindow, bool Map_status)
{	Talkback("Bad call to Vessel_component::Draw_component(SFML_Window * iwindow, bool Map_status)");
	// shouldnt be called at all, but failsafes are good
}

void Vessel_component::Update_component(double dt, std::vector<Force> &parent_force_list)
{	Talkback("Bad call to Vessel_component::Update_component(double dt, std::vector<Force> &parent_force_list)");
	// Yaaaa... hmm
	// just a placeholder cause C++ compilers demand it be defined for reasons
	// I dont recall. Rather annoying :(
}

double Vessel_component::Get_component_mass()
{	Talkback("Bad call to Vessel_component::Get_component_mass()");
	return -1;
}	// this would cause some screwed up stuff, although the physicists would be
	// thrilled. Intended to signal something is wrong.
	
double Vessel_component::Get_component_inertia()
{	Talkback("Bad call to Vessel_component::Get_component_inertia()");
	return -1;
}	// the results of this would be... unstable to say the least

Vessel_component* Vessel_component::Get_vessel_component_pointer()
{	return this;
}	// abstraction, abstraction...

Vessel_component::~Vessel_component()
{
}




// Resource Tank Class /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Resource_Tank::Resource_Tank(double initial_tank_resource_mass, double tank_mass, double inner_radius, double outer_radius, double tank_length, VectorVictor::Vector2 PositionVector)
{	Tank_mass = tank_mass;
	Resource_mass = initial_tank_resource_mass;
	Component_moment = new Inertia_cylinder(inner_radius, outer_radius, tank_length, PositionVector);
	Empty = false;
	// the Empty should be conditional, but whatever
	
	// just simple passing of info to the object here.
}

void Resource_Tank::Update_component(double dt, std::vector<Force> &parent_force_list)
{	//...
	// I mean, there could be something here eventually, but currently
	// nothing
}

double Resource_Tank::Get_tank_inertia()
{	return Component_moment->Get_moment_about_pivot(Resource_mass, Tank_mass);
	// Just query our inertia moment object with the current masses so we can
	// assess the mass
}

double Resource_Tank::Get_resource_mass()
{	return Resource_mass;
}

double Resource_Tank::Get_component_mass()
{	double net_mass = 0;
	net_mass += Tank_mass;
	net_mass += Resource_mass;
	return net_mass;
	// all mass, in kilos
}

double Resource_Tank::Get_component_inertia()
{	return this->Get_tank_inertia();
}	// Not clear on what this actually does
	// Seems dangerous to reveal a pointer, I think this should be removed
	// will need to test the change first
	
Resource_Tank::~Resource_Tank()
{	delete Component_moment;
}

// Thruster Class //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Thruster::Update_component(double dt, std::vector<Force> &parent_force_list)
{	Talkback("Bad call to Thruster::Update_component(double dt, std::vector<Force> &parent_force_list)");
	// Yaaaa... hmm	// this would be a pain in the ass to have happen
	// cant have this, needs to be the child class definition
}

bool Thruster::Is_in_group(thruster_group group)
{	for(std::vector<thruster_group>::iterator it = Groups.begin(); it != Groups.end(); ++it)
	{	if((*it) == group)
		{	return true;
		}
	}
	return false;
}

void Thruster::Throttle_down(double dt, double k_throttle)
{	Thruster_throttle -= (k_throttle*dt);
	if(Thruster_throttle < 0.00000000000)
	{	Thruster_throttle = 0.000000000000;
	}
	// quite simple, just increment the value down, but make sure it stays
	// 0<throttle<1, negative throttles would do some weeeird things
}

void Thruster::Throttle_up(double dt, double k_throttle)
{	Thruster_throttle += (k_throttle*dt);
	if(Thruster_throttle > 1.00000000000)
	{	Thruster_throttle = 1.000000000000;
	}
	// same as throttle down, even more important that we dont exceed one here
	// or physics will break
}

void Thruster::Throttle_to(double dt, double k_throttle, double Throttle_target)
{	if(Throttle_target > 1.000000000000000)
	{	Throttle_target = 1.000000000000000;
	}
	else if(Throttle_target < 0.000000000000000)
	{	Throttle_target = 0.000000000000000;
	}
	
	if(Thruster_throttle != Throttle_target)
	{	double delta = Throttle_target - Thruster_throttle;
		// we get the relative difference between the two throttle levels
		if(delta >= (dt*k_throttle))
		{	Thruster_throttle += (dt*k_throttle);
		}
		else if(delta <= -(dt*k_throttle))
		{	Thruster_throttle -= (dt*k_throttle);
		}
		else
		{	// if we are closer to the target than the magnitude of
			// dt*k_throttle , just jump straight to the value itself
			Thruster_throttle = Throttle_target;
		}
	}
	// otherwise we are already there, so dont need to do anything
}

long double Thruster::Get_maximum_torque(double dt)
{	Talkback("Bad call to long double Thruster::Get_maximum_torque()");
	return -1;
	// this also needs an error warning here
}

double Thruster::Get_component_mass()
{	return Thruster_mass;
}

double Thruster::Get_component_inertia()
{	double inertia = Component_moment->Get_moment_about_pivot(0, (2*Thruster_mass));
	// double the thruster mass for the larger sphere, then its inertia is
	// halved just like it is. The logic works, but I cant easily lay it out
	// in this medium
	inertia /= 2;		
	// so my idea was just to do a half sphere for the inertia, not perfect, 
	// but a decent approximation for an engine bell
	// better to cut these in half than the cows ;)
	return inertia;
	// weird that this is done at this level..., but I guess it does work
}	

Thruster* Thruster::Get_thruster_pointer()
{	return this;
}



// Monopropellant Thrusters ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Monopropellant_thruster::Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one)
{	Exhaust_velocity = vexhaust;
	Maximum_flow_rate = max_flow_rate;
	Thruster_position.Set_values(position_x, position_y);
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	// Important that it has a normalized direction, otherwise stuff goes nuts
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Fuel_tank = fuel_tank;
	Component_moment = new Inertia_sphere(inner_radius, outer_radius, Thruster_position);
	Groups.insert(Groups.end(), group_one);
	Is_RCS = is_rcs;
	// all the nice initialization
}

Monopropellant_thruster::Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one, thruster_group group_two)
{	Exhaust_velocity = vexhaust;
	Maximum_flow_rate = max_flow_rate;
	Thruster_position.Set_values(position_x, position_y);
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	// Important that it has a normalized direction, otherwise stuff goes nuts
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Fuel_tank = fuel_tank;
	Component_moment = new Inertia_sphere(inner_radius, outer_radius, Thruster_position);
	Groups.insert(Groups.end(), group_one);
	Groups.insert(Groups.end(), group_two);
	Is_RCS = is_rcs;
	// all the nice initialization
}

Monopropellant_thruster::Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one, thruster_group group_two, thruster_group group_three)
{	Exhaust_velocity = vexhaust;
	Maximum_flow_rate = max_flow_rate;
	Thruster_position.Set_values(position_x, position_y);
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	// Important that it has a normalized direction, otherwise stuff goes nuts
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Fuel_tank = fuel_tank;
	Component_moment = new Inertia_sphere(inner_radius, outer_radius, Thruster_position);
	Groups.insert(Groups.end(), group_one);
	Groups.insert(Groups.end(), group_two);
	Groups.insert(Groups.end(), group_three);
	Is_RCS = is_rcs;
	// all the nice initialization
}

long double Monopropellant_thruster::Get_maximum_torque(double dt)
{	VectorVictor::Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y());
	thruster_force *=(Maximum_flow_rate*Exhaust_velocity);
	thruster_force *= -1;
	Force max_force(Thruster_position, thruster_force);
	long double max_torque = max_force.Get_force_torque();
	return max_torque;
}

void Monopropellant_thruster::Update_component(double dt, std::vector<Force> &parent_force_list)
{	// and welcome to probably the pickiest code in this project so far
	// this whole method is really just the update, so anything pertaining to
	// the engine as an object (overheats and explodes?) can go in here before
	// the thrust section is run
	if(Fuel_tank->Empty == false)
	{	// ^ self explanatory
		if(Thruster_throttle > 0.0000000000000000)
		{	// ^ also self explanatory
			VectorVictor::Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y());
			// the actual force being exerted by the thruster
			double dm = (Thruster_throttle*Maximum_flow_rate*dt);
			// delta mass, how much fuel we gonna throw this frame
			if(Fuel_tank->Get_resource_mass() >= dm)
			{	Fuel_tank->Resource_mass -= dm;
			}	// if our fuel tank has enough left for dm, decrease its
				// contents by dm
			else if(Fuel_tank->Get_resource_mass() < dm)
			{	dm = Fuel_tank->Get_resource_mass();
				Fuel_tank->Resource_mass = 0.0000000000000000;
				Fuel_tank->Empty = true;
				// If the tank doesnt have enough left to match the dm,
				// give it whatever is left, and signal the tank empty
			}
			else if(Fuel_tank->Resource_mass <= 0)
			{	dm = 0;
				Fuel_tank->Empty = true;
				if(Fuel_tank->Resource_mass < 0)
				{	Fuel_tank->Resource_mass = 0.0000000000000000;
				}
				// just redundant check to make sure weird stuff with negative
				// mass doesnt happen
			}
			thruster_force *= dm;
			// our direction of force with magnitude 1, gets multiplied by dm
			thruster_force *= Exhaust_velocity;
			// and multiplied by the velocity, making it a vector with magnitude
			// the product of dm and velocity
			thruster_force *= (1/dt);
			// something, something... Unit Analysis!
			// I dont know precisely why this works, the usual suggestion is to
			// obtain the force from the other side of the dp/dt equation,
			// but it seems that using the m*(dv/dt) part works too (ie)
			
			// p = m*v
			
			// dp/dt = v*(dm/dt) + m*(dv/dt)
			
			// I guess very vaguely, Im working from the perspective of the
			// mass being ejected, m being our delta mass for the frame, dv
			// being the change in velocity for the mass over the frame
			// (0 to Ve) and dt is the length of the frame.
			// If the framerate were infinite (as is the typical assumption
			// with most of these things) the Ve divided by the dt really would
			// be dv/dt, so yeah...
			
			// More importantly, it fits Tsiolkovsky equation, so it fits
			// reality well			
			thruster_force *= -1;
			// reverse the direction of the force since the original unit vector
			// is opposite the direction the force will be applied in			
			
			// just some feedback on the forces magnitude for some reason
			Force New_force(Thruster_position, thruster_force);
			parent_force_list.insert(parent_force_list.end(), New_force);	
			// construct the Force object and attach it to the parent force list
			// all done here
		}
	}
}

Monopropellant_thruster::~Monopropellant_thruster()
{	delete Component_moment;
}




// Bipropellant Thrusters //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


Bipropellant_thruster::Bipropellant_thruster(double thruster_mass, double vexhaust, double optimal_mix_ratio, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, Resource_Tank * oxidizer_tank)
{	Exhaust_velocity = vexhaust;	// I like verbose constructor definitions and I cannot lie ;)
	Maximum_flow_rate = max_flow_rate;
	Thruster_position.Set_values(position_x, position_y);
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Oxidizer_tank = oxidizer_tank;
	Fuel_tank = fuel_tank;
	Optimal_mixture_ratio = optimal_mix_ratio;
	Component_moment = new Inertia_sphere(inner_radius, outer_radius, Thruster_position);
	// same as for the monopropellant thruster, except we assign an oxidizer
	// tank as well
}

// handy function here /////////////////////////////////////////////////////////

// really should be defined elsewhere, but whatever

inline double Get_absolute_value(double value)
{	// inline makes more sense here I think
	if(value == 0)
	{	return 0;
	}
	else
	{	if(value > 0)
		{	return value;
		}
		else
		{	value = (-value);
			return value;
		}
	}
}

double Bipropellant_thruster::Get_exhaust_velocity(double mixture_ratio)
{	double Drop_off_constant = 42;	
	// dont argue with it. You know it to be true
	if(mixture_ratio == Optimal_mixture_ratio)
	{	return Exhaust_velocity;
	}
	else
	{	double ve = (Exhaust_velocity*(exp(-(Drop_off_constant*(Get_absolute_value((mixture_ratio - Optimal_mixture_ratio)))))));
		return ve;
	}
	// basically, how is our Ve affected by improper mixture ratios
	// could potentially be slow, and the reality might be assymetrical
}

long double Bipropellant_thruster::Get_maximum_torque(double dt)
{	return -1;
}

void Bipropellant_thruster::Update_component(double dt, std::vector<Force> &parent_force_list)
{	if((Fuel_tank->Empty == false)||(Oxidizer_tank->Empty == false))
	{	if(Thruster_throttle > 0.0000000000000000)
		{	VectorVictor::Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y());
			double dm = (Thruster_throttle*Maximum_flow_rate*dt);
			double dm_oxidizer = (dm/(1+(1/Optimal_mixture_ratio)));
			double dm_fuel = (dm/(1+Optimal_mixture_ratio));
			// The Fuel tank drain run /////////////////////////////////////////
			if(Fuel_tank->Get_resource_mass() <= dm_fuel)
			{	Fuel_tank->Resource_mass -= dm_fuel;
			}
			else if(Fuel_tank->Get_resource_mass() > 0.0000000000000000)
			{	dm_fuel = Fuel_tank->Get_resource_mass();
				Fuel_tank->Resource_mass = 0.0000000000000000;
			}
			else
			{	dm_fuel = 0;
				Fuel_tank->Empty = true;
			}
			// The Oxidizer tank drain run /////////////////////////////////////
			if(Oxidizer_tank->Get_resource_mass() <= dm_oxidizer)
			{	Oxidizer_tank->Resource_mass -= dm_oxidizer;
			}
			else if(Oxidizer_tank->Get_resource_mass() > 0.0000000000000000)
			{	dm_oxidizer = Oxidizer_tank->Get_resource_mass();
				Oxidizer_tank->Resource_mass = 0.0000000000000000;
			}
			else
			{	dm_oxidizer = 0;
				Oxidizer_tank->Empty = true;
			}
			dm = dm_oxidizer + dm_fuel;
			double mix = (dm_oxidizer/dm_fuel);
			thruster_force *= dm;
			thruster_force *= Get_exhaust_velocity(mix);
			Force New_force(Thruster_position, thruster_force);
			parent_force_list.insert(parent_force_list.end(), New_force);				// this is a lot simpler than the original function, although fuel tanks arent type-checked.
		}
	}
}

// I dont know if this works at the moment, this will need to be tested later on

Bipropellant_thruster::~Bipropellant_thruster()
{	delete Component_moment;
}


// Hull class //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Hull::Hull(double hull_mass, double inertia_factor, double hull_length, VectorVictor::Vector2 PositionVector)
{	Hull_mass = hull_mass;
	Component_moment = new Inertia_complex(inertia_factor, PositionVector);
	Length = hull_length;	
}

void Hull::Update_component(double dt, std::vector<Force> &parent_force_list)
{	// ...
}

double Hull::Get_hull_length()
{	return Length;
}

double Hull::Get_hull_length_squared()
{	return (Length*Length);
}

double Hull::Get_component_mass()
{	return Hull_mass;
}

double Hull::Get_hull_inertia()
{	return Component_moment->Get_moment_about_pivot(Hull_mass, 0);
}

double Hull::Get_component_inertia()
{	return Get_hull_inertia();
}

Hull::~Hull()
{	delete Component_moment;
}

// not much to comment on in the hull class, pretty straightforward


// Newtonian Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


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
	for(std::vector<CKeplerian_Object*>::iterator it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
	{	(*it)->Gravitate(Get_total_mass(), Get_theta_in_degrees(), Position ,Force_list);
		// we cycle through all of the large bodies in the universe
		// (probably excluding anything asteroid or smaller) and get them to add
		// a gravitational force to the vessel
	}
	this->Update_motion(dt);
	// the vessel uses the forces acting upon it to update its motion...
	this->Update_rotation(dt);
	// ...and then does the same for rotation
	Update_PMI();						
	// we recalculate the vessels total moment of inertia
	// maybe a good candidate to do selectively, since this may not be needed
	// every single frame
	Force_list.clear();
	// set the net force on the object back to zero for the next frame
	if(Crashed == false)
	{	Crashed = Crash_state(simtime, ignition_celestials);
		// if we arent crashed at the moment, check if we are crashed
	}
}

bool CNewtonian_Object::Crash_state(long double sim_time, std::vector<CKeplerian_Object*> &ignition_celestials)
{	long double offset_rad = 0;
	for(std::vector<CKeplerian_Object*>::iterator it = ignition_celestials.begin(); it != ignition_celestials.end(); ++it)
	{	offset_rad = VectorVictor::Get_vector_separation(Position, (*it)->Get_position(sim_time));
		if(offset_rad < (*it)->Get_radius(0))	// important to change this once terrain is a thing
		{	return true;
		}
		// iterate through all of the large bodies in the sim and see if we seem
		// to be 'underground'. If we are, we've probably bought the farm, so we
		// set the Crashed state to true by returning true
	}
	return false;
	// if not, we continue on our merry way
	
}
	
void CNewtonian_Object::Update_motion(long double dt)
{	
	if(Crashed == false)
	{	// If we've made a crater in the planet, dont bother updating
		VectorVictor::Vector2 Net_force(0,0);
		// start with a net force of 0
		for(std::vector<Force>::iterator it = Force_list.begin(); it != Force_list.end(); ++it)
		{	Net_force += it->Force_vector.Get_rotated_vector(Theta);	// Now this makes more sense
			// add up all of the forces acting on the vessel in this frame
		}	
		Acceleration.x = (Net_force.x/Get_total_mass()); 
		Acceleration.y = (Net_force.y/Get_total_mass());	
		// Newtons laws, simply
		
		// F = ma
		// a = F/m
		Velocity.x += ((Acceleration.x)*dt);
		Velocity.y += ((Acceleration.y)*dt);
		Position.x += (((Velocity.x)*dt) + (((0.50000000000000000)*Acceleration.x)*dt*dt));
		Position.y += (((Velocity.y)*dt) + (((0.50000000000000000)*Acceleration.y)*dt*dt));
		// Simple constant acceleration equations of motion	
	}
	// there should be an else statement here so that we keep pace with the
	// keplerians movement and rotation
	
}


void CNewtonian_Object::Update_rotation(long double dt)
{	
	if(Crashed == false)
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
{	if((Position.Get_x() >= (window->origin.x-(Hull_component->Get_hull_length_squared())))&&(Position.Get_x() <= ((window->origin.x + (window->Aperture_width+(Hull_component->Get_hull_length_squared()))))))
	{	if((Position.Get_y() <= (window->origin.y+(Hull_component->Get_hull_length_squared())))&&(Position.Get_y() >= (window->origin.y - ((window->Aperture_height+(Hull_component->Get_hull_length_squared()))))))
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
		
	
	sf::Vector2f camera_offset = Get_window_coordinates(Position, iwindow, cam_scale);
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
	
	sf::Vector2f camera_offset = Get_window_coordinates(Position, iwindow, zoom_factor);
	
	
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


// Delta Glider Class //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


DeltaGlider::DeltaGlider(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, sf::Texture * status_texture, sf::Font * controls_font, sf::Texture * panel_texture1)
{	Talkback("Constructing Delta Glider");
	// we write to the console for feedback while debugging
	
	Init_vessel_type();
	// run the setup function
	
	Flag_sprite = iFlag_sprite;			
	// set our flag affilation here
	// this looks wrongish, I think this needs to be copied by value, definitely
	// not reference, since the same flag may need to be drawn several times per
	// frame
	Flag_sprite->setScale(0.1,0.1);
	// scales it back by 1/10 along each axis, since the orginal was too big
	// this is baddish
	Object_name = ivessel_name;
	Vessel_class = "Delta Glider";
	// this should rather be frozen so it cant be changed in sim, just for
	// safety/not chaos
	Acceleration.Set_values(0, 0);
	// just an call to be safe
	Position.Set_values(initial_x_position, initial_y_position);
	Velocity.Set_values(initial_x_velocity, initial_y_velocity);
	Theta = initial_theta; Omega = initial_omega; 
	// we take what we were passed and set our properties to it
	Crashed = false;
	// important that this gets set explicitly, without it the compiler just 
	// sets it to random values for some godawful reason
	
	VectorVictor::Vector2 origin(0,0);
	Hull_component = new Hull(16000, 0.4, 17.5, origin);
	Object_components.insert(Object_components.end(), Hull_component);
	// we construct the hull component and insert it into the general component
	// list. The vessel can access it by either the Hull component reference
	// that it stores internally, or through its general version in the parts
	// list
			
	// the pointer passin crap didnt work ^^
	// ^ not sure exactly what that was referring to
	Main_fuel = new Resource_Tank(initial_main_propellant, 800, 0.6, 0.8, 2.2, origin);
	Fuel_tanks.insert(Fuel_tanks.end(), Main_fuel);	
	Object_components.insert(Object_components.end(), Main_fuel->Get_vessel_component_pointer());
	// construct the main fuel tank and insert it into both the fuel tank list
	// and the general parts list.
	// Come to think of it, why exactly is there a separate list for fuel tanks?
	// they arent even accessed that way, not yet anyways
	
	Init_thruster(false, 4000, 50000, 290, 0, -6.2000, 0, -1, 0.4, 0.49, Main_fuel, main_engines); 
	// construct the main engine thrsuter, insert it into the thruster vector
	// and the general components vector
	
	// this is at least necessary, since the main thruster vector is used for
	// throttling back all engines each frame unless the throttle up is doing
	// the opposite
	
	VectorVictor::Vector2 rcs_pos(0, 2);
	RCS_fuel = new Resource_Tank(initial_rcs_propellant, 500, 0.6, 0.64, 1,rcs_pos);
	Fuel_tanks.insert(Fuel_tanks.end(), RCS_fuel);		
	Object_components.insert(Object_components.end(), RCS_fuel->Get_vessel_component_pointer());	
	
	Init_thruster(true, 470, 49000, 610, 0, 6.2000, -1, 0, 0.10, 0.12, RCS_fuel, rotate_clockwise, translate_right);
	Init_thruster(true, 470, 49000, 610, 0, 6.2000, 1, 0, 0.10, 0.12, RCS_fuel, rotate_counterclockwise, translate_left);
	Init_thruster(true, 470, 49000, 460, 0, 6.400, 0, 1, 0.10, 0.12, RCS_fuel, translate_back); 
	Init_thruster(true, 470, 49000, 630, -0.20, -6.0000, -1, 0, 0.10, 0.12, RCS_fuel, rotate_counterclockwise, translate_right); 
	Init_thruster(true, 470, 49000, 630, -0.20, -6.0000, 1, 0, 0.10, 0.12, RCS_fuel, rotate_clockwise, translate_left); 
	Init_thruster(true, 470, 49000, 460, -0.2, -6.0000, 0, -1, 0.10, 0.12, RCS_fuel, translate_forward);
		
	// and we construct all of the RCS thrusters in the same way
	
	// this should definitely be a function to minimize mistakes
	
	std::cout << Object_components.size() << " Vessel components" << std::endl;
	// not sure why, but good to have the number of components on the vessel
	// for debugging
	
	k_throttle = 29.9000;
	// we set this up so that the thruster goes from 0 to 2990 % in one second
	// obviously that doesnt work, so it really represents 0 to 100% in 1/29.9 s
	// (0.033 s, roughly 2 frames if fps is a healthy 60)
	Throttle_lock = false;
	// Throttle lock keeps the main engines at their current throttle setting
	// instead of trying to slide back to zero each frame, which is useful
	// during long burns
	
	Update_PMI();
	// once all of the vessel components are in, this should be run to get the
	// initial value
	Object_sprite = new sf::Sprite();
	Object_sprite->setTexture(*XWing_texture);
	// we create it and assign it the texture passed by the constructor
	// (less memory waste per vessel than each one loading a copy of the texture
	// for every instance
	Vessel_tex = XWing_texture;
	// not sure why we need to copy the reference to this...
	
	// this appears to be redundant, and a good candidate to remove
	text_colour = new sf::Color(194, 38, 15);
	// nice red colour for all of the vessel displays
	
	vessel_id = new sf::Text();
	vessel_id->setFont(*controls_font);
	vessel_id->setCharacterSize(16);
	vessel_id->setColor(*text_colour);
	vessel_id->setPosition(20, 380);
	
	main_fuel_level = new sf::Text();
	main_fuel_level->setFont(*controls_font);
	main_fuel_level->setCharacterSize(14);
	main_fuel_level->setColor(*text_colour);
	main_fuel_level->setString(std::to_string((float)Main_fuel->Resource_mass));
	main_fuel_level->setPosition(20, 420);
	
	rcs_fuel_level = new sf::Text();
	rcs_fuel_level->setFont(*controls_font);	
	rcs_fuel_level->setCharacterSize(14);
	rcs_fuel_level->setColor(*text_colour);
	rcs_fuel_level->setString(std::to_string((float)RCS_fuel->Resource_mass));	
	rcs_fuel_level->setPosition(20, 440);
	
	omega_value = new sf::Text();
	omega_value->setFont(*controls_font);	
	omega_value->setCharacterSize(14);
	omega_value->setColor(*text_colour);
	omega_value->setString(std::to_string((float)Omega));	
	omega_value->setPosition(20, 460);
	
	theta_value = new sf::Text();
	theta_value->setFont(*controls_font);	
	theta_value->setCharacterSize(14);
	theta_value->setColor(*text_colour);
	theta_value->setString(std::to_string((float)Theta));	
	theta_value->setPosition(20, 480);
	
	position_values = new sf::Text();
	position_values->setFont(*controls_font);	
	position_values->setCharacterSize(14);
	position_values->setColor(*text_colour);
	position_values->setString("(-,-)");	
	position_values->setPosition(20, 500);	
	
	velocity_values = new sf::Text();
	velocity_values->setFont(*controls_font);	
	velocity_values->setCharacterSize(14);
	velocity_values->setColor(*text_colour);
	velocity_values->setString("(-,-)");	
	velocity_values->setPosition(20, 520);		
	// hideously long set of operations to set up the text feedback displays
	// onscreen. This is a big part of why it needs to be generalized as an
	// object	
	
	vessel_display = new sf::Sprite();
	vessel_display->setTexture(*status_texture);
	vessel_display->setPosition(sf::Vector2f(820, 460));	
	vessel_display->setColor(sf::Color(255, 255, 255, 168));
	vessel_display->setScale(sf::Vector2f(0.5f, 0.5f));
	// the pretty semi-transparent image of the ship that appears at bottom
	// right in the screen. Really just eye candy for now, but I have plans for
	// it later on. Its vessel implementation-specific anyways
	
	display_panel = new sf::Sprite();
	display_panel->setTexture(*panel_texture1);
	display_panel->setPosition(sf::Vector2f(5, 360));	
	// the metal looking thing at bottom right. Only there because the onscreen
	// displays proved hard to read against the background
	
	pix_length = Hull_component->Length*10;
	// get the length of the vessels long axis in meters
	// the multiple of ten thing is fine, but the current window size was
	// accidentally made 10x too large, so vessels appeared 10x larger than
	// they really were
	
	// not terribly important, since this will change when the window view
	// has multiple scales to be a lot like the one for planets, but it is
	// important to remember that the base view has 1m = 10 pixels, for reasons
	// of easiest scaling
	if(Vessel_tex->getSize().y != pix_length)
	{	Object_sprite->setScale((pix_length/(Vessel_tex->getSize().y)),(pix_length/(Vessel_tex->getSize().y)));
		// rescale the axes of the texture to match pix_length in the y and the
		// appropriate scale for the x dimension
		
		// not sure why they both use y, but I think this was distorted when
		// it used x & y.
		// This area needs to be looked over again
	}	
	Object_sprite->setOrigin((Vessel_tex->getSize().x/2), (Vessel_tex->getSize().y/2));
	Object_sprite->setRotation(Theta);
	// and we center the origin and rotate it appropriately
	Talkback("...Constructed Delta Glider");
	// feed back that the constructor is now finished to the console
}	

	// most constructor calls will be completely off now. Needs to be updated to reflect the new changes
	// the component pointers will also need to be fixed in order to access them in specific ways as well as through the generic polymorphic formats

	// ^ this is odd, not sure what changes it referred to, although the 
	// second line is probably fixed now
	// I think this is safe to remove

void DeltaGlider::Receive_inputs(key_commands * current_inputs, double dt)
{	if(current_inputs->z == true)
	{	Translate_left(dt);
	}
	else if(current_inputs->c == true)
	{	Translate_right(dt);
	}	
	else if(current_inputs->x == true)
	{	Translate_backward(dt);
	}
	else if(current_inputs->w == true)
	{	Translate_forward(dt);
	}
	else if(current_inputs->d == true)
	{	Rotate_right(dt);
	}
	else if(current_inputs->a == true)
	{	Rotate_left(dt);
	}	
	else if(current_inputs->s == true)
	{	Kill_rotation(dt);
	}	// map the rotational and translational commands to the WASD side of
		// the keyboard. It works very similar to the numpad in Orbiter, but I
		// used the keyboard for it since numpad inputs dont seem to be received
		// on linux for some reason
		
		// It can always work on both, but I think using the WASD keys makes
		// more sense, since every keyboard will have that, if not necessarily
		// a numpad
	else
	{	No_command(dt);
	}	// the throttle back of every thruster since no inputs are being sent
		// intended for RCS thrusters, but also applies to main/retro/hover,
		// not exactly a perfect setup here
		
	if(current_inputs->up == true)
	{	Throttle_up(dt*5);
	}
	else if(current_inputs->down == true)
	{	Throttle_down(dt*5);
	}
}

double DeltaGlider::Get_total_mass()
{	double net_mass = 0;
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	net_mass += (*it)->Get_component_mass();
	}	// iterate through all components and get the mass. I think this was
		// defined already in TVessel or Newtonian, doesnt need definition here	
	return net_mass;
}

double DeltaGlider::Get_PMI()
{	return PMI;
}

void DeltaGlider::Print_data()
{	std::cout << "RCS Fuel: " << RCS_fuel->Resource_mass << std::endl;
	std::cout << "Crash state: " << Crashed << std::endl;
	// general useful data
	
	// should probably reformat this a wee bit
}

void DeltaGlider::Toggle_throttle_lock()
{	if(Throttle_lock == true)
	{	Throttle_lock = false;
	}
	else
	{	Throttle_lock = true;
	}
	// simple bit flippin
}

void DeltaGlider::Draw_controls(SFML_Window * iwindow, bool Map_status)
{	std::string mainfuel = std::to_string((int)Main_fuel->Resource_mass);
	mainfuel.append(" kg");
	main_fuel_level->setString(mainfuel);
	std::string rcsfuel = std::to_string((int)RCS_fuel->Resource_mass);
	rcsfuel.append(" kg");
	rcs_fuel_level->setString(rcsfuel);	
	omega_value->setString(std::to_string((int)Omega));	
	theta_value->setString(std::to_string((int)Theta));
	std::string pos = "Position ";
	pos.append(Position.Get_vector("m"));	
	position_values->setString(pos);	
	std::string vel = "Velocity ";
	vel.append(Velocity.Get_vector("m/s"));
	velocity_values->setString(vel);	
	std::string id = Get_vessel_class();
	id.append(" Class\n");
	id.append(Get_vessel_name());
	vessel_id->setString(id);
	// do a hideous transfer of formatted data to the sf texts
	switch(Map_status)
	{	case true:
		{	iwindow->window->draw(*display_panel);
			iwindow->window->draw(*vessel_id);
			iwindow->window->draw(*main_fuel_level);
			iwindow->window->draw(*rcs_fuel_level);
			iwindow->window->draw(*omega_value);
			iwindow->window->draw(*theta_value);
			iwindow->window->draw(*position_values);
			iwindow->window->draw(*velocity_values);	
			iwindow->window->draw(*vessel_display);	
			break;
		}
		case false:
		{	iwindow->window->draw(*display_panel);
			iwindow->window->draw(*vessel_id);
			iwindow->window->draw(*main_fuel_level);
			iwindow->window->draw(*rcs_fuel_level);
			iwindow->window->draw(*omega_value);
			iwindow->window->draw(*theta_value);
			iwindow->window->draw(*position_values);
			iwindow->window->draw(*velocity_values);	
			iwindow->window->draw(*vessel_display);	
			break;
		}
		//and then draw em up onscreen.
		// this happens after most in universe stuff gets drawn I think, since
		// displays should *usually* be on top
	}		
}

void DeltaGlider::Receive_cursor_inputs(Cursor_commands * cursor_action, long double dt)
{	// this way we can check if the cursor is on us, and whether that merits
	// any changes on the vessels end of things
}

void DeltaGlider::Translate_left(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_left))
		{	(*it)->Throttle_up(dt, k_throttle);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}

// obvious as it gets, just adjust throttles based on dt and the k_throttle
// for the given thruster. This should accept a parameter to modify k_throttle
// so we can modify it on the spot instead of passing through stuff with dt
// I smell disaster there, so its better not to tempt fate

void DeltaGlider::Translate_right(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_right))
		{	(*it)->Throttle_up(dt, k_throttle);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}

void DeltaGlider::Translate_backward(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_back))
		{	(*it)->Throttle_up(dt, k_throttle);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}

void DeltaGlider::Translate_forward(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_forward))
		{	(*it)->Throttle_up(dt, k_throttle);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}	

void DeltaGlider::Rotate_right(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_clockwise))
		{	(*it)->Throttle_up(dt, k_throttle);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}	

void DeltaGlider::Rotate_right(double dt, double throttle_target)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_clockwise))
		{	(*it)->Throttle_to(dt, k_throttle, throttle_target);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}	

void DeltaGlider::Rotate_left(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_counterclockwise))
		{	(*it)->Throttle_up(dt, k_throttle);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}	

void DeltaGlider::Rotate_left(double dt, double throttle_target)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_counterclockwise))
		{	(*it)->Throttle_to(dt, k_throttle, throttle_target);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, k_throttle);
			}
		}
	}
}	

void DeltaGlider::No_command(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_RCS == true)
		{	(*it)->Throttle_down(dt, k_throttle);
		}
	}
}	

void DeltaGlider::Throttle_up(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(main_engines))
		{	(*it)->Throttle_up(dt, k_throttle);
		}
	}
	
}

void DeltaGlider::Throttle_down(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(main_engines))
		{	(*it)->Throttle_down(dt, k_throttle);
		}
	}
}	

void DeltaGlider::Kill_rotation(double dt)
{	if(Omega != 0.000000000000)
	{	
		double delta_omega = Absolute_value(Omega);
		double magnitude_alpha;
		double max_alpha;
		if(Omega > 0.000000000000)
		{	max_alpha = this->Get_max_alpha(counterclockwise);
			magnitude_alpha = Absolute_value(max_alpha);
			if(Alpha > 0.000000000000)
			{	Rotate_left(dt);
			}
			else
			{	if(magnitude_alpha > delta_omega)
				{	double percentage = (delta_omega/magnitude_alpha);
					Rotate_left(dt, percentage);
				}
				else
				{	Rotate_left(dt);
				}
			}
		}
		else 	// the negative case
		{	max_alpha = this->Get_max_alpha(clockwise);
			magnitude_alpha = Absolute_value(max_alpha);
			if(Alpha < 0.000000000000)
			{	Rotate_right(dt);
			}
			else
			{	if(magnitude_alpha > delta_omega)
				{	double percentage = (delta_omega/magnitude_alpha);
					Rotate_right(dt, percentage);
				}
				else
				{	Rotate_right(dt);
				}
			}
		}	
	}	
	// awwww yeahhhh, thats what Im talkin about
	
	// lets see what happens now
	else
	{	No_command(dt);
	}
}


DeltaGlider::~DeltaGlider()
{	Thrusters.clear();
	Fuel_tanks.clear();
	delete Object_sprite;
	delete main_fuel_level;
	delete rcs_fuel_level;
	delete omega_value;
	delete theta_value;
	delete position_values;
	delete velocity_values;	
	delete vessel_display;
	delete vessel_id;
	delete display_panel;
	delete Hull_component;
	delete text_colour;
	delete Hull_component;
}

