// Vessel_components.cpp ///////////////////////////////////////////////////////
// The classes for any kind of part that makes up a Newtonian object ///////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include "Vessel_components.hpp"




// Vessel Component parent class ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

part_id Vessel_component::Component_index = 1;


Vessel_component::Vessel_component(VectorVictor::Vector2 component_position)
{	Component_position = component_position;
}


part_id Vessel_component::Get_new_index()
{	part_id new_id = Component_index;
	Component_index += 1;
	// increment by one
	return new_id;
}

part_id Vessel_component::Get_component_id()
{	return Component_index;
}

void Vessel_component::New_vessel_component(Vessel_component * new_this)
{	Vessel_components.insert(Vessel_components.end(), this->Get_vessel_component_pointer());
	// lookin guud
}

void Vessel_component::Draw_component(SFML_Window * iwindow, bool Map_status)
{	Talkback("Bad call to Vessel_component::Draw_component(SFML_Window * iwindow, bool Map_status)");
	// shouldn't be called at all, but failsafes are good
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
	
double Vessel_component::Get_component_inertia(VectorVictor::Vector2 axis)
{	Talkback("Bad call to Vessel_component::Get_component_inertia(VectorVictor::Vector2 axis)");
	return -1;
}	// the results of this would be... unstable to say the least


long double Vessel_component::Get_length()
{	return Length;
}

Vessel_component* Vessel_component::Get_vessel_component_pointer()
{	return this;
}

Vessel_component::~Vessel_component()
{
}




// Resource Tank Class /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Resource_Tank::Resource_Tank(double initial_tank_resource_mass, double tank_mass, double inner_radius, double outer_radius, double tank_length, VectorVictor::Vector2 PositionVector): Vessel_component(PositionVector)
{	Tank_mass = tank_mass;
	Resource_mass = initial_tank_resource_mass;
	Component_moment = new Hollow_cylinder(inner_radius, outer_radius, tank_length);
	Empty = false;
	// the Empty should be conditional, but whatever
	Component_position = PositionVector;
	
	Length = sqrt((tank_length*tank_length)+(outer_radius*outer_radius));
	// just simple passing of info to the object here.
}

void Resource_Tank::Update_component(double dt, std::vector<Force> &parent_force_list)
{	//...
	// I mean, there could be something here eventually, but currently
	// nothing
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

double Resource_Tank::Get_component_inertia(VectorVictor::Vector2 axis)
{	return Component_moment->Get_moment_about_pivot(axis, Resource_mass, Tank_mass);
}	
	
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

double Thruster::Get_component_inertia(VectorVictor::Vector2 axis)
{	double inertia = Component_moment->Get_moment_about_pivot(axis, 0, Thruster_mass);
	// for now we are back to approximating things as spheres
	// but this can be fixed later
	
	
	//inertia /= 2;		
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

Monopropellant_thruster::Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one): Vessel_component(VectorVictor::Vector2(position_x, position_y))
{	Exhaust_velocity = vexhaust;
	Maximum_flow_rate = max_flow_rate;
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	// Important that it has a normalized direction, otherwise stuff goes nuts
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Fuel_tank = fuel_tank;
	Component_moment = new Solid_sphere(inner_radius);
	Groups.insert(Groups.end(), group_one);
	Is_RCS = is_rcs;
	// all the nice initialization
	//Component_position.Set_values(position_x, position_y);
	// now taken care of by the Vessel component constructor
	
	// it works, but a VV2 would be nicer
}

Monopropellant_thruster::Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one, thruster_group group_two)
{	Exhaust_velocity = vexhaust;
	Maximum_flow_rate = max_flow_rate;
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	// Important that it has a normalized direction, otherwise stuff goes nuts
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Fuel_tank = fuel_tank;
	Component_moment = new Solid_sphere(inner_radius);
	Groups.insert(Groups.end(), group_one);
	Groups.insert(Groups.end(), group_two);
	Is_RCS = is_rcs;
	// all the nice initialization
	Component_position.Set_values(position_x, position_y);
}


Monopropellant_thruster::Monopropellant_thruster(bool is_rcs, double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, thruster_group group_one, thruster_group group_two, thruster_group group_three)
{	Exhaust_velocity = vexhaust;
	Maximum_flow_rate = max_flow_rate;
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	// Important that it has a normalized direction, otherwise stuff goes nuts
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Fuel_tank = fuel_tank;
	Component_moment = new Solid_sphere(inner_radius);
	Groups.insert(Groups.end(), group_one);
	Groups.insert(Groups.end(), group_two);
	Groups.insert(Groups.end(), group_three);
	Is_RCS = is_rcs;
	// all the nice initialization
	Component_position.Set_values(position_x, position_y);	
}

long double Monopropellant_thruster::Get_maximum_torque(double dt)
{	VectorVictor::Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y());
	thruster_force *=(Maximum_flow_rate*Exhaust_velocity);
	thruster_force *= -1;
	Force max_force(Component_position, thruster_force);
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
			Force New_force(Component_position, thruster_force);
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
	Component_position.Set_values(position_x, position_y);
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Oxidizer_tank = oxidizer_tank;
	Fuel_tank = fuel_tank;
	Optimal_mixture_ratio = optimal_mix_ratio;
	Component_moment = new Solid_sphere(inner_radius);
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
			Force New_force(Component_position, thruster_force);
			parent_force_list.insert(parent_force_list.end(), New_force);				
			// this is a lot simpler than the original function, although
			// fuel tanks arent type-checked.
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
	Component_moment = new Inertia_complex(inertia_factor);
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

double Hull::Get_component_inertia(VectorVictor::Vector2 axis)
{	return Component_moment->Get_moment_about_pivot(axis, Hull_mass, 0);
}

Hull::~Hull()
{	delete Component_moment;
}

// not much to comment on in the hull class, pretty straightforward
