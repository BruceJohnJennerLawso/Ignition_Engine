#include <string>
#include <iostream>
#include <math.h>

// Force handling class ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Force::Force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force)
{	Attack_point = attack_point;
	Force_vector = force;
}

//double Force::Get_force_torque(VectorVictor::Vector2 pivot_point)
//{	double torque = 0;
//	Attack_point -= pivot_point;
//}

double Force::Get_force_torque()
{	double torque = 0;
	torque += VectorVictor::Get_cross_product(Attack_point, Force_vector);
	return torque;
}

VectorVictor::Vector2 Force::Get_force_vector()
{	return Force_vector;
}

VectorVictor::Vector2 Force::Get_force_vector(double angle)
{	return Force_vector.Get_rotated_vector(angle);
}

Force::~Force()
{
}

// Vessel Component parent class ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Vessel_component::Draw_component(SFML_Window * iwindow, bool Map_status)
{	Talkback("Bad call to Vessel_component::Draw_component(SFML_Window * iwindow, bool Map_status)");
	//std::cout << "Bad call to Vessel_component::Draw_component(SFML_Window * iwindow, bool Map_status)" << std::endl;
}

void Vessel_component::Update_component(double dt, std::vector<Force> &parent_force_list)
{	// Yaaaa... hmm
}

double Vessel_component::Get_component_mass()
{	return -1;
}	// this would cause some screwed up stuff
	
double Vessel_component::Get_component_inertia()
{	return -1;
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

Resource_Tank::Resource_Tank(double initial_tank_resource_mass, double tank_mass, double inner_radius, double outer_radius, double tank_length, VectorVictor::Vector2 PositionVector)
{	Tank_mass = tank_mass;
	Resource_mass = initial_tank_resource_mass;
	Component_moment = new Inertia_cylinder(inner_radius, outer_radius, tank_length, PositionVector);
	Empty = false;
}

void Resource_Tank::Update_component(double dt, std::vector<Force> &parent_force_list)
{	//...
}

double Resource_Tank::Get_tank_inertia()
{	return Component_moment->Get_moment_about_pivot(Resource_mass, Tank_mass);
	
	//double inertia = (Tank_mass*Tank_inertia);
	//inertia *= (Tank_distance*Tank_distance);
	//return inertia;
}

double Resource_Tank::Get_resource_mass()
{	return Resource_mass;
}

double Resource_Tank::Get_component_mass()
{	double net_mass = 0;
	net_mass += Tank_mass;
	net_mass += Resource_mass;
	return net_mass;
}

double Resource_Tank::Get_component_inertia()
{	return this->Get_tank_inertia();
}	// I guess this could be used

Resource_Tank::~Resource_Tank()
{	delete Component_moment;
}

// Thruster Class //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Thruster::Update_component(double dt, std::vector<Force> &parent_force_list)
{	// Yaaaa... hmm	// this would be a pain in the ass to have happen
}

void Thruster::Throttle_down(double dt, double k_throttle)
{	Thruster_throttle -= (k_throttle*dt);
	if(Thruster_throttle < 0.00000000000)
	{	Thruster_throttle = 0.000000000000;
	}
}

double Thruster::Get_component_mass()
{	return Thruster_mass;
}

double Thruster::Get_component_inertia()
{	double inertia = Component_moment->Get_moment_about_pivot(0, (2*Thruster_mass));
	inertia /= 2;		// so my idea was just to do a half sphere for the inertia, not perfect, but a decent approximation for an engine bell
	return inertia;
}	

void Thruster::Throttle_up(double dt, double k_throttle)
{	Thruster_throttle += (k_throttle*dt);
	if(Thruster_throttle > 1.00000000000)
	{	Thruster_throttle = 1.000000000000;
	}
}

Thruster* Thruster::Get_thruster_pointer()
{	return this;
}


// Monopropellant Thrusters ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Monopropellant_thruster::Monopropellant_thruster(double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank)
{	Exhaust_velocity = vexhaust;
	Maximum_flow_rate = max_flow_rate;
	Thruster_position.Set_values(position_x, position_y);
	Thruster_direction.Set_values(direction_x, direction_y);
	Thruster_direction.Normalize();
	Thruster_throttle = 0.0000000000000000;
	empty_tank = false;
	Thruster_mass = thruster_mass;
	Fuel_tank = fuel_tank;
	Component_moment = new Inertia_sphere(inner_radius, outer_radius, Thruster_position);
}

void Monopropellant_thruster::Update_component(double dt, std::vector<Force> &parent_force_list)
{	if(Fuel_tank->Empty == false)
	{	if(Thruster_throttle > 0.0000000000000000)
		{	Spillback("Fuel non-empty & thruster throttle non-zero at " , Thruster_throttle);
			Spillback("Beginning force run");
			VectorVictor::Vector2 thruster_force(Thruster_direction.Get_x(), Thruster_direction.Get_y());
			double dm = (Thruster_throttle*Maximum_flow_rate*dt);
			if(Fuel_tank->Get_resource_mass() >= dm)
			{	Fuel_tank->Resource_mass -= dm;
			}
			else if(Fuel_tank->Get_resource_mass() < dm)
			{	dm = Fuel_tank->Get_resource_mass();
				Fuel_tank->Resource_mass = 0.0000000000000000;
				Fuel_tank->Empty = true;
			}
			else if(Fuel_tank->Resource_mass <= 0)
			{	dm = 0;
				Fuel_tank->Empty = true;
				if(Fuel_tank->Resource_mass < 0)
				{	Fuel_tank->Resource_mass = 0.0000000000000000;
				}
			}
			thruster_force *= dm;
			thruster_force *= Exhaust_velocity;
			thruster_force *= (1/dt);
			thruster_force *= -1;			
			std::string force_info = "Thruster force: ";
			force_info.append(thruster_force.Get_vector("N"));	// arg forgot this was in vessel local coordinates
			Spillback(force_info);
			Force New_force(Thruster_position, thruster_force);
			parent_force_list.insert(parent_force_list.end(), New_force);				// this is a lot simpler than the original draw function, although thruster types 
		}
	}
}

Monopropellant_thruster::~Monopropellant_thruster()
{	delete Component_moment;
}


// Bipropellant Thrusters ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
}

// handy function here ////////////////////////////////////

double Get_absolute_value(double value)
{	if(value == 0)
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
{	double Drop_off_constant = 42;		// dont argue with it. You know it to be true
	if(mixture_ratio == Optimal_mixture_ratio)
	{	return Exhaust_velocity;
	}
	else
	{	double ve = (Exhaust_velocity*(exp(-(Drop_off_constant*(Get_absolute_value((mixture_ratio - Optimal_mixture_ratio)))))));
		return ve;
	}
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
	Spillback("...Hull component updated...");
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
	
	//double inertia = (Hull_mass*Hull_inertia);
	//inertia *= ((Length*Length)/4);	// ie r^2 == ((L/2)^2) == ((L^2)*(1/4))
	//return inertia;
}

double Hull::Get_component_inertia()
{	return Get_hull_inertia();
}

Hull::~Hull()
{	delete Component_moment;
}

// Celestial Bodies ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void CKeplerian_Object::Frame(double dt, long double simtime)
{	std::cout << "Bad call to CKeplerian_Object::Frame(double dt)" << std::endl;
}

long double CKeplerian_Object::Get_theta_in_degrees()
{	return Theta;
}

long double CKeplerian_Object::Get_theta_in_radians()
{	long double rad_theta = Theta;
	rad_theta *= 6.283185308;		// 4/3 pau actually ;)
	rad_theta /= 360;
	return rad_theta;	
}

long double CKeplerian_Object::Get_omega()
{	return Omega;
}

long double CKeplerian_Object::Get_radius(double longitude)
{	return Radius;
}

long double CKeplerian_Object::Get_mass()	// In kilograms, bitch
{	return Mass;							
}

VectorVictor::Vector2 CKeplerian_Object::Get_position(long double sim_time)
{	//std::cout << "Bad call to CKeplerian_Object::Get_position(double sim_time)" << std::endl;
	VectorVictor::Vector2 origin(0.000000000000000000000,0.000000000000000000000);
	return origin;
}

void CKeplerian_Object::Gravitate(long double satellite_mass, long double satellite_rotation,VectorVictor::Vector2 satellite_position, std::vector<Force> &parent_force_list)
{	// a whole #$%^ year later... finally I get to write this
	VectorVictor::Vector2 origin(0.000000000000000000,0.000000000000000);
	long double G = 6.673e-11; // universal gravitational constant
	long double M = this->Get_mass();
	VectorVictor::Vector2 Vf(0,0);	 //this really needs to be fixed to the relative keplerian position
	Vf -=  satellite_position;
	Spillback("\nGetting the relative position vector: ");
	Spillback(Vf.Get_vector("m relative"));	
	long double radial_distance_squared = Vf.Get_vector_magnitude_squared();
	Spillback("Current radius squared: ", radial_distance_squared);
	if(radial_distance_squared >100000000)
	{	Vf.Normalize();	// reduce our direction vector so we have direction & magnitude 1
		
		Spillback("\nCurrent unit vector: ");
		Spillback(Vf.Get_vector(" "));
		
		long double Fmag = ((G*M)*satellite_mass);
		Fmag /= radial_distance_squared;
		Vf *= Fmag;	// 
		
		Spillback("\nFinal Gravity Force Vector: ");
		Spillback(Vf.Get_vector("N"));
		
		long double rotation = (-satellite_rotation);
		//Vf.Rotate_vector(rotation);
		//Vf.Rotate_vector(90);
		Force New_force(origin, Vf.Get_rotated_vector(rotation));
		parent_force_list.insert(parent_force_list.end(), New_force); 
	}
}

std::string CKeplerian_Object::Get_object_name()
{	return Object_name;
}	

bool CKeplerian_Object::In_view(SFML_Window * window, int zoom_factor)
{	std::cout << "Bad call to CKeplerian_Object::In_view(SFML_Window * window, int zoom_factor, VectorVictor::Rectangle * view_frame)" << std::endl;
	return false;
}
void CKeplerian_Object::Draw_flag(SFML_Window * iwindow, int zoom_factor)
{	std::cout << "Bad call to CKeplerian_Object::Draw_flag(SFML_Window * iwindow, int zoom_factor, VectorVictor::Rectangle * view_frame)" << std::endl;
}

// TPlanet ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TPlanet::TPlanet(long double initial_theta, long double omega, long double radius, long double atmosphere_height, long double mass, std::string planet_texture_path)
{	Theta = initial_theta;
	Omega = omega;
	Radius = radius;
	Mass = mass;
	Object_texture = new sf::Texture();
	if(!Object_texture->loadFromFile(planet_texture_path))
	{	std::cout << "Planet " << Get_object_name() << " unable to load texture at " << planet_texture_path << std::endl;
	}
	else
	{	long double pix_length = Get_radius(0) + atmosphere_height;
		pix_length /= 5;		// I forget why its 20 here..., wait oh, I get it now
		float sprite_x, sprite_y;
		for(unsigned int cy = 1; cy != 16; ++cy)
		{	sf::Sprite * planet_sprite;
			planet_sprite = new sf::Sprite();
			planet_sprite->setTexture(*Object_texture);
			sprite_x = Object_texture->getSize().x;	sprite_x *= planet_sprite->getScale().x;
			sprite_y = Object_texture->getSize().y;  sprite_y *= planet_sprite->getScale().y;
			std::cout << "sprite dimensions: " << sprite_x << " , " << sprite_y << std::endl;
			planet_sprite->setOrigin((sprite_x/2), (sprite_y/2));
			if(Object_texture->getSize().y != pix_length)
			{	planet_sprite->setScale((pix_length/(Object_texture->getSize().x)),(pix_length/(Object_texture->getSize().y)));			
			}
			planet_sprite->setRotation(Theta);
			Planet_sprites.insert(Planet_sprites.end(), planet_sprite);
			pix_length /= 10;
		}
	}
}	// once more, my code works and I dont know why ^^
// I need to ask Vaughan or someone about this

void TPlanet::Frame(double dt, long double simtime)
{	Theta += (Omega*dt);
	for(std::vector<sf::Sprite*>::iterator it = Planet_sprites.begin(); it != Planet_sprites.end(); ++it)
	{	(*it)->setRotation(Theta);
	}
	Simulation_time = simtime;
}

VectorVictor::Vector2 TPlanet::Get_position(long double sim_time)
{	VectorVictor::Vector2 origin(0,0);
	return origin;
}


bool TPlanet::In_view(SFML_Window * window, int zoom_factor)
{	if((Get_position(Simulation_time).Get_x() >= (window->origin.x-(Get_radius(0))))&&(Get_position(Simulation_time).Get_x() <= ((window->origin.x + (window->Aperture_width+(Get_radius(0)))))))
	{	if((Get_position(Simulation_time).Get_y() <= (window->origin.y+(Get_radius(0))))&&(Get_position(Simulation_time).Get_y() >= (window->origin.y - ((window->Aperture_height+(Get_radius(0)))))))
		{	return true;
		}
		else
		{	return false;
		}
	}	
	else
	{	return false;
	}
}

void TPlanet::Draw_flag(SFML_Window * iwindow, int zoom_factor)
{	sf::Vector2f offset(Get_position(Simulation_time).Get_x(), Get_position(Simulation_time).Get_y());	sf::Vector2f camera_origin(iwindow->origin.x, iwindow->origin.y);
	offset -= camera_origin;
	offset.y = -offset.y;	// Dont ask ^^
	offset.y *= pow(0.1, (zoom_factor));
	offset.x *= pow(0.1, (zoom_factor));		// next step is rewriting this to take advantage of the new relative position function located in VV::Rectangle...
	for(std::vector<sf::Sprite*>::iterator it = Planet_sprites.begin(); it != Planet_sprites.end(); ++it)
	{	(*it)->setPosition(offset);
	}	// eh, just easier to do it this way. I should probably change it eventually
	iwindow->window->draw(*(Planet_sprites.at(zoom_factor-1)));
}

TPlanet::~TPlanet()
{	delete Object_texture;
}

// Newtonian Class ///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double CNewtonian_Object::Get_omega()
{	return Omega;
}

double CNewtonian_Object::Get_theta_in_radians()
{	double rad_theta = Theta;
	rad_theta *= 6.283185308;		// 4/3 pau actually ;)
	rad_theta /= 360;
	return rad_theta;
}

double CNewtonian_Object::Get_theta_in_degrees()
{	return Theta;
}

double CNewtonian_Object::Get_length()
{	return Length;
}

double CNewtonian_Object::Get_hull_mass()
{	return -1;
}

double CNewtonian_Object::Get_total_mass()
{	double net_mass = 0;
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	net_mass += (*it)->Get_component_mass();
	}	return net_mass;
}

void CNewtonian_Object::Update_PMI()
{	PMI = 0;
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	PMI += (*it)->Get_component_inertia();
	}
}

void CNewtonian_Object::Receive_inputs(key_commands * current_inputs, double dt)
{	Talkback("Bad call to CNewtonian_Object::Receive_inputs(key_commands * current_inputs, double dt)");
}

bool CNewtonian_Object::In_view(SFML_Window * window, int zoom_factor)
{	Talkback("Bad call to CNewtonian_Object::In_view(SFML_Window * window, int zoom_factor)");
	return false;
}

void CNewtonian_Object::Print_data()
{	Talkback("Bad call to CNewtonian_Object::Print_data()");
}

double CNewtonian_Object::Get_PMI()
{	return PMI;
}

void CNewtonian_Object::Add_force(double attack_point_x, double attack_point_y, double force_x, double force_y)
{	VectorVictor::Vector2 vattack(attack_point_x, attack_point_y);
	VectorVictor::Vector2 vforce(force_x, force_y);
	Force new_force(vattack, vforce);
	Force_list.insert(Force_list.end(), new_force);
}

void CNewtonian_Object::Add_force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force_vector)
{	Force new_force(attack_point, force_vector);
	Force_list.insert(Force_list.end(), new_force);
}
	
void CNewtonian_Object::Frame(long double dt, long double simtime)
{	std::string intro = "...Frame(double dt) beginning for ";
	intro.append(Object_name);
	Spillback(intro);
	
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	(*it)->Update_component(dt, Force_list);	// this is where component forces get added
	}
	for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
	{	(*it)->Gravitate(Get_total_mass(), Get_theta_in_degrees(), Position ,Force_list);
	}
	this->Update_motion(dt);
	this->Update_rotation(dt);
	Update_PMI();						// this needs to be selected for computatation later on. Too many operations to be running needlessly each frame
	Force_list.clear();
	if(Crashed == false)
	{	Crashed = Crash_state(simtime);
	}
	Spillback("Current Coordinates ");
	Spillback("x: ", Position.x);
	Spillback("y: ", Position.y);
	Spillback("Frame(double dt) finished...\n\n");
}

bool CNewtonian_Object::Crash_state(long double sim_time)
{	long double offset_rad = 0;
	for(std::vector<CKeplerian_Object*>::iterator it = Celestial_list.begin(); it != Celestial_list.end(); ++it)
	{	offset_rad = VectorVictor::Get_vector_separation(Position, (*it)->Get_position(sim_time));
		if(offset_rad < (*it)->Get_radius(0))	// important to change this once terrain is a thing
		{	return true;
		}
	}
	return false;
	
}
	
void CNewtonian_Object::Update_motion(long double dt)
{	Spillback("Start of CNewtonian_Object::Update_motion(double dt)");
	
	if(Crashed == false)
	{	VectorVictor::Vector2 Net_force(0,0);
		for(std::vector<Force>::iterator it = Force_list.begin(); it != Force_list.end(); ++it)
		{	Net_force += it->Force_vector.Get_rotated_vector(Theta);	// Now this makes more sense
		}	
		Spillback(Net_force.Get_vector("N Total Force"));
		Acceleration.x = (Net_force.x/Get_total_mass()); 
		Acceleration.y = (Net_force.y/Get_total_mass());	
		Velocity.x += ((Acceleration.x)*dt);
		Velocity.y += ((Acceleration.y)*dt);	
		Position.x += (((Velocity.x)*dt) + (((0.50000000000000000)*Acceleration.x)*dt*dt));
		Position.y += (((Velocity.y)*dt) + (((0.50000000000000000)*Acceleration.y)*dt*dt));
	}
	
	Spillback("End of CNewtonian_Object::Update_motion(double dt)\n");
}


void CNewtonian_Object::Update_rotation(long double dt)
{	Spillback("Start of CNewtonian_Object::Update_rotation(double dt)");
	
	if(Crashed == false)
	{	double alpha = 0;
		for(std::vector<Force>::iterator it = Force_list.begin(); it != Force_list.end(); ++it)
		{	alpha -= it->Get_force_torque();
		}
		alpha /= PMI;
		Theta += ((Omega*dt)+(0.5*((alpha*dt)*dt)));
		Omega += (alpha*dt);
		Object_sprite->setRotation(Theta);
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
	}
	
	Spillback("End of CNewtonian_Object::Update_rotation(double dt)\n");
}

CNewtonian_Object* CNewtonian_Object::Get_Newtonian_pointer()
{	return this;
}

// TVessel Class ///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TVessel::Rotate_left(double dt)
{	std::cout << "Bad call to TVessel::Rotate_left(double dt) " << std::endl;
}

void TVessel::Rotate_right(double dt)
{	std::cout << "Bad call to TVessel::Rotate_right(double dt) " << std::endl;
}

void TVessel::Kill_rotation(double dt)					// this seems at first to conflict with rot left/right, but since input commands come in one by one, they shouldnt. I hope
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

void TVessel::Draw_controls(SFML_Window * iwindow, bool Map_status)
{
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
}

void TVessel::Draw_vessel(SFML_Window * iwindow)
{	sf::Vector2f offset(Position.Get_x(), Position.Get_y());	sf::Vector2f camera_origin(iwindow->origin.x, iwindow->origin.y);
	offset -= camera_origin;
	offset.y = -offset.y;	// Dont ask ^^
	Object_sprite->setPosition(offset);
	//offset += sf::Vector2f(40, -40);		// never did figure this part out...
	Flag_sprite->setPosition(offset + sf::Vector2f((10*Hull_component->Length), -(10*Hull_component->Length)));
	iwindow->window->draw(*Object_sprite);
	iwindow->window->draw(*Flag_sprite);
}

void TVessel::Draw_flag(SFML_Window * iwindow, int zoom_factor)
{	sf::Vector2f offset(Position.Get_x(), Position.Get_y());	sf::Vector2f camera_origin(iwindow->origin.x, iwindow->origin.y);
	offset -= camera_origin;
	offset.y = -offset.y;	// Dont ask ^^
	offset.y *= pow(0.1, (zoom_factor));
	offset.x *= pow(0.1, (zoom_factor));
	Flag_sprite->setPosition(offset);
	iwindow->window->draw(*Flag_sprite);
}

std::string TVessel::Get_vessel_class()
{	return Vessel_class;
}

std::string TVessel::Get_vessel_name()
{	return Object_name;
}

TVessel* TVessel::Get_Vessel_pointer()
{	return this;
}


// Current Vessel classes //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// -XWing
// @line 430











// X-Wing Class /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


XWing::XWing(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, sf::Texture * status_texture, sf::Font * controls_font, sf::Texture * panel_texture1)
{	Talkback("Constructing XWing");
	Flag_sprite = iFlag_sprite;			// set our flag affilation here
	Flag_sprite->setScale(0.1,0.1);
	Object_name = ivessel_name;
	Vessel_class = "Delta Glider";
	Acceleration.Set_values(0, 0);
	Position.Set_values(initial_x_position, initial_y_position);
	Velocity.Set_values(initial_x_velocity, initial_y_velocity);
	Theta = initial_theta; Omega = initial_omega; 
	
	VectorVictor::Vector2 origin(0,0);
	
	Hull_component = new Hull(16000, 0.4, 17.5, origin);
	Object_components.insert(Object_components.end(), Hull_component);		// the pointer passin crap didnt work ^^
	
	Main_fuel = new Resource_Tank(initial_main_propellant, 800, 0.6, 0.8, 2.2, origin);
	Fuel_tanks.insert(Fuel_tanks.end(), Main_fuel);	
	Object_components.insert(Object_components.end(), Main_fuel->Get_vessel_component_pointer());
	
	Main_engines = new Monopropellant_thruster(4000, 50000, 290, 0, -6.2000, 0, -1, 0.4, 0.49, Main_fuel); 
	Thrusters.insert(Thrusters.end(), Main_engines);
	Object_components.insert(Object_components.end(), Main_engines->Get_vessel_component_pointer());
	
	VectorVictor::Vector2 rcs_pos(0, 2);
	RCS_fuel = new Resource_Tank(initial_rcs_propellant, 500, 0.6, 0.64, 1,rcs_pos);
	Fuel_tanks.insert(Fuel_tanks.end(), RCS_fuel);		
	Object_components.insert(Object_components.end(), RCS_fuel->Get_vessel_component_pointer());	
	
	Bow_left = new Monopropellant_thruster(470, 49000, 610, 0, 6.2000, -1, 0, 0.10, 0.12, RCS_fuel);
	Thrusters.insert(Thrusters.end(), Bow_left);
	Object_components.insert(Object_components.end(), Bow_left->Get_vessel_component_pointer());
		
	Bow_right = new Monopropellant_thruster(470, 49000, 610, 0, 6.2000, 1, 0, 0.10, 0.12, RCS_fuel);
	Thrusters.insert(Thrusters.end(), Bow_right);
	Object_components.insert(Object_components.end(), Bow_right->Get_vessel_component_pointer());	
	
	Bow_fore = new Monopropellant_thruster(470, 49000, 460, 0, 6.400, 0, 1, 0.10, 0.12, RCS_fuel); 
	Thrusters.insert(Thrusters.end(), Bow_fore);
	Object_components.insert(Object_components.end(), Bow_fore->Get_vessel_component_pointer());		
	
	Stern_left = new Monopropellant_thruster(470, 49000, 630, -0.20, -6.0000, -1, 0, 0.10, 0.12, RCS_fuel); 
	Thrusters.insert(Thrusters.end(), Stern_left);
	Object_components.insert(Object_components.end(), Stern_left->Get_vessel_component_pointer());		
	
	Stern_right = new Monopropellant_thruster(470, 49000, 630, -0.20, -6.0000, 1, 0, 0.10, 0.12, RCS_fuel); 
	Thrusters.insert(Thrusters.end(), Stern_right);
	Object_components.insert(Object_components.end(), Stern_right->Get_vessel_component_pointer());		
	
	Stern_aft = new Monopropellant_thruster(470, 49000, 460, -0.2, -6.0000, 0, -1, 0.10, 0.12, RCS_fuel);
	Thrusters.insert(Thrusters.end(), Stern_aft);
	Object_components.insert(Object_components.end(), Stern_aft->Get_vessel_component_pointer());		
	
	std::cout << Object_components.size() << " Vessel components" << std::endl;
	
	k_throttle = 29.9000;
	Throttle_lock = false;
	
	Update_PMI();
	Object_sprite = new sf::Sprite();
	Object_sprite->setTexture(*XWing_texture);
	Vessel_tex = XWing_texture;
	
	text_colour = new sf::Color(194, 38, 15);
	
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
	
	
	vessel_display = new sf::Sprite();
	vessel_display->setTexture(*status_texture);
	vessel_display->setPosition(sf::Vector2f(820, 460));	
	vessel_display->setColor(sf::Color(255, 255, 255, 168));
	vessel_display->setScale(sf::Vector2f(0.5f, 0.5f));
	
	display_panel = new sf::Sprite();
	display_panel->setTexture(*panel_texture1);
	display_panel->setPosition(sf::Vector2f(5, 360));	
	
	double pix_length = Hull_component->Length*10;
	if(Vessel_tex->getSize().y != pix_length)
	{	Object_sprite->setScale((pix_length/(Vessel_tex->getSize().y)),(pix_length/(Vessel_tex->getSize().y)));
	}	
	Object_sprite->setOrigin((Vessel_tex->getSize().x/2), (Vessel_tex->getSize().y/2));
	Object_sprite->setRotation(Theta);
	Talkback("...Constructed XWing");
}	// most constructor calls will be completely off now. Needs to be updated to reflect the new changes
	// the component pointers will also need to be fixed in order to access them in specific ways as well as through the generic polymorphic formats


void XWing::Receive_inputs(key_commands * current_inputs, double dt)
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
	}			
	else
	{	No_command(dt);
	}
	
	if(current_inputs->l == true)
	{	Toggle_throttle_lock();
	}
	
	switch(Throttle_lock)
	{	case true:
		{	if(current_inputs->up == true)
			{	Throttle_up(dt);
			}
			else if(current_inputs->down == true)
			{	Throttle_down(dt);
			}
			break;
		}
		case false:
		{	if(current_inputs->up == true)
			{	Throttle_up(50*dt);
			}
			else
			{	Throttle_down(50*dt);
			}
			break;
		}
	}
}	// this needs to have the new key inputs

double XWing::Get_total_mass()
{	//double net_mass = Get_hull_mass();
	double net_mass = 0;
	for(std::vector<Vessel_component*>::iterator it = Object_components.begin(); it != Object_components.end(); ++it)
	{	net_mass += (*it)->Get_component_mass();
	}	return net_mass;
}

double XWing::Get_PMI()
{	return PMI;
}

void XWing::Print_data()
{	std::cout << "Engines Main: " << Main_engines->Thruster_throttle << "		" << Main_fuel->Resource_mass << std::endl;
	std::cout << "RCS Engines " << std::endl;
	std::cout << Bow_fore ->Thruster_throttle << std::endl;
	std::cout << Bow_left->Thruster_throttle <<  std::endl;
	std::cout << Bow_right->Thruster_throttle <<  std::endl;
	std::cout << Stern_aft->Thruster_throttle << std::endl;
	std::cout << Stern_left->Thruster_throttle << std::endl;
	std::cout << Stern_right->Thruster_throttle << std::endl;
	std::cout << "RCS Fuel: " << RCS_fuel->Resource_mass << std::endl;
	double acceleration;
	acceleration = (Main_engines->Exhaust_velocity*Main_engines->Maximum_flow_rate);
	acceleration /= Get_total_mass();
	std::cout << "top accel " << acceleration << " - " << acceleration/9.81 << " g" <<  std::endl; 
	std::cout << "Main engine direction: " << (Main_engines->Thruster_direction.Get_rotated_vector(Theta)).Get_vector(" ") << std::endl;
}

void XWing::Toggle_throttle_lock()
{	if(Throttle_lock == true)
	{	Throttle_lock = false;
	}
	else
	{	Throttle_lock = true;
	}
}

void XWing::Draw_controls(SFML_Window * iwindow, bool Map_status)
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
	switch(Map_status)
	{	case true:
		{	iwindow->window->draw(*display_panel);
			iwindow->window->draw(*vessel_id);
			iwindow->window->draw(*omega_value);
			iwindow->window->draw(*theta_value);
			iwindow->window->draw(*position_values);
			iwindow->window->draw(*velocity_values);	
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
	}		
}

void XWing::Translate_left(double dt)
{	// Throttles up ///////////////////////////////////////////////////////////
	Bow_right->Throttle_up(dt, k_throttle);
	Stern_right->Throttle_up(dt, k_throttle);
	// Throttles down //////////////////////////////////////////////////////
	Bow_left->Throttle_down(dt, k_throttle);
	Stern_left->Throttle_down(dt, k_throttle);
	Bow_fore->Throttle_down(dt, k_throttle);
	Stern_aft->Throttle_down(dt, k_throttle);
}

void XWing::Translate_right(double dt)
{	// Throttles up ///////////////////////////////////////////////////////////
	Bow_left->Throttle_up(dt, k_throttle);
	Stern_left->Throttle_up(dt, k_throttle);
	// Throttles down //////////////////////////////////////////////////////
	Bow_right->Throttle_down(dt, k_throttle);
	Stern_right->Throttle_down(dt, k_throttle);
	Bow_fore->Throttle_down(dt, k_throttle);
	Stern_aft->Throttle_down(dt, k_throttle);
}

void XWing::Translate_backward(double dt)
{	// Throttles up ///////////////////////////////////////////////////////////
	Bow_fore->Throttle_up(dt, k_throttle);
	// Throttles down //////////////////////////////////////////////////////
	Bow_right->Throttle_down(dt, k_throttle);
	Bow_left->Throttle_down(dt, k_throttle);
	Stern_aft->Throttle_down(dt, k_throttle);
	Stern_left->Throttle_down(dt, k_throttle);
	Stern_right->Throttle_down(dt, k_throttle);
}	// next is translate forwards

void XWing::Translate_forward(double dt)
{	// Throttles up ///////////////////////////////////////////////////////////
	Stern_aft->Throttle_up(dt, k_throttle);
	// Throttles down //////////////////////////////////////////////////////
	Bow_right->Throttle_down(dt, k_throttle);
	Bow_left->Throttle_down(dt, k_throttle);
	Bow_fore->Throttle_down(dt, k_throttle);
	Stern_left->Throttle_down(dt, k_throttle);
	Stern_right->Throttle_down(dt, k_throttle);
}	

void XWing::Rotate_right(double dt)
{	// Throttles up ///////////////////////////////////////////////////////////
	Bow_left->Throttle_up(dt, k_throttle);
	Stern_right->Throttle_up(dt, k_throttle);
	// Throttles down //////////////////////////////////////////////////////
	Bow_right->Throttle_down(dt, k_throttle);
	Stern_aft->Throttle_down(dt, k_throttle);
	Stern_left->Throttle_down(dt, k_throttle);
	Bow_fore->Throttle_down(dt, k_throttle);
}	

void XWing::Rotate_left(double dt)
{	// Throttles up ///////////////////////////////////////////////////////////
	Bow_right->Throttle_up(dt, k_throttle);
	Stern_left->Throttle_up(dt, k_throttle);
	// Throttles down //////////////////////////////////////////////////////
	Bow_left->Throttle_down(dt, k_throttle);
	Stern_aft->Throttle_down(dt, k_throttle);
	Stern_right->Throttle_down(dt, k_throttle);
	Bow_fore->Throttle_down(dt, k_throttle);
}	

void XWing::No_command(double dt)
{	// Throttles down //////////////////////////////////////////////////////
	 Bow_right->Throttle_down(dt, k_throttle);
	 Bow_left->Throttle_down(dt, k_throttle);
	 Bow_fore->Throttle_down(dt, k_throttle);
	 Stern_aft->Throttle_down(dt, k_throttle);
	 Stern_left->Throttle_down(dt, k_throttle);
	 Stern_right->Throttle_down(dt, k_throttle);
}	

void XWing::Throttle_up(double dt)
{	// Throttles up ///////////////////////////////////////////////////////////
	Main_engines->Throttle_up(dt, k_throttle);
}

void XWing::Throttle_down(double dt)
{	// Throttles down ///////////////////////////////////////////////////////
	Main_engines->Throttle_down(dt, k_throttle);
}	

void XWing::Kill_rotation(double dt)
{	if(Omega != 0.000000000000)
	{	if(Omega > 0.000000000000)
		{	Rotate_left(dt);
		}
		else 	// the negative case
		{	Rotate_right(dt);
		}	// well... that was easier than expected
	}
	else
	{	No_command(dt);
	}
}

XWing::~XWing()
{	Thrusters.clear();
	Fuel_tanks.clear();
	delete Main_engines;
	delete Bow_left;
	delete Bow_right;
	delete Bow_fore;
	delete Stern_left;
	delete Stern_right;
	delete Stern_aft;
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


// Tie Fighter //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

