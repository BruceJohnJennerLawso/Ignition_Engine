#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "Inertia_moment.h"


class Force
{	public:
	Force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force);
	VectorVictor::Vector2 Attack_point;
	VectorVictor::Vector2 Force_vector;
	double Get_force_torque();
	double Get_force_torque(VectorVictor::Vector2 pivot_point);
	VectorVictor::Vector2 Get_force_vector();
	VectorVictor::Vector2 Get_force_vector(double angle);
	~Force();
};

class Vessel_component
{	public:
	virtual void Update_component(double dt, std::vector<Force> &parent_force_list);
	virtual void Draw_component(SFML_Window * iwindow, bool Map_status);
	Inertia_moment * Component_moment;
	Vessel_component* Get_vessel_component_pointer();
	virtual double Get_component_mass();
	virtual double Get_component_inertia();
	~Vessel_component();
	friend class CNewtonian_Object;		// Didnt fix it anyways
};

class Resource_Tank: public Vessel_component
{	public:
	Resource_Tank(double initial_tank_resource_mass, double tank_mass, double inner_radius, double outer_radius, double tank_length, VectorVictor::Vector2 PositionVector);	// everything is cylinders now
	virtual void Update_component(double dt, std::vector<Force> &parent_force_list);
	double Tank_mass, Resource_mass;
	bool Empty;
	double Get_tank_inertia();
	double Get_resource_mass();
	double Get_component_mass();
	double Get_component_inertia();	
	~Resource_Tank();
};

class Thruster: public Vessel_component
{	public:
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	double Exhaust_velocity, Maximum_flow_rate, Thruster_throttle, Thruster_mass;		// mix ratio is (mass oxidizer/mass fuel) 
	VectorVictor::Vector2 Thruster_position;						// normalized along with direction at construction if necessary
	VectorVictor::Vector2 Thruster_direction;
	void Throttle_down(double dt, double k_throttle);
	void Throttle_up(double dt, double k_throttle);
	bool empty_tank;		// this is completely temporary	// I swear, totally	// actually this would be good as a quick "precheck before continuing" sort of thing // Ill at least make it uppercase
	double Get_component_mass();
	double Get_component_inertia();		
	Thruster* Get_thruster_pointer();
};

class Monopropellant_thruster: public Thruster
{	public:
	Monopropellant_thruster(double thruster_mass, double vexhaust, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank);
	Resource_Tank * Fuel_tank;
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	~Monopropellant_thruster();
};

class Bipropellant_thruster: public Thruster
{	public:
	Bipropellant_thruster(double thruster_mass, double vexhaust, double optimal_mix_ratio, double max_flow_rate, double position_x, double position_y, double direction_x, double direction_y, double inner_radius, double outer_radius, Resource_Tank * fuel_tank, Resource_Tank * oxidizer_tank);	
	double Optimal_mixture_ratio;
	double Get_exhaust_velocity(double mix_ratio);
	Resource_Tank * Fuel_tank, * Oxidizer_tank;
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	~Bipropellant_thruster();
};


class Hull: public Vessel_component
{	public:
	Hull(double hull_mass, double inertia_factor, double hull_length, VectorVictor::Vector2 PositionVector);
	void Update_component(double dt, std::vector<Force> &parent_force_list);
	double Hull_mass, Hull_inertia, Length;		// hull distance just assumed to be zero
	double Get_hull_length();
	double Get_hull_length_squared();
	double Get_hull_inertia();
	double Get_component_mass();
	double Get_component_inertia();		
	~Hull();
};

// Celestial Bodies ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

class CKeplerian_Object
{	public:
	virtual void Frame(double dt, long double simtime);
	long double Theta, Omega;
	long double Get_omega();
	long double Get_theta_in_degrees();		
	long double Get_theta_in_radians();
	
	long double Simulation_time;
	
	long double Radius;
	long double Get_radius(double longitude);
	
	long double Mass;
	long double Get_mass();
		
	VectorVictor::Vector2 Get_position(long double sim_time);
	
	void Gravitate(long double satellite_mass, long double satellite_rotation, VectorVictor::Vector2 satellite_position, std::vector<Force> &parent_force_list);
	
	std::string Object_name;
	std::string Get_object_name();
	
	sf::Texture * Object_texture;
	
	virtual bool In_view(SFML_Window * window, int zoom_factor);	
	virtual void Draw_flag(SFML_Window * iwindow, int zoom_factor);
};

std::vector<CKeplerian_Object*> Celestial_list;

class TPlanet: public CKeplerian_Object
{	public:
	TPlanet(long double initial_theta, long double omega, long double radius, long double atmosphere_height, long double mass, std::string planet_texture_path);
	void Frame(double dt, long double simtime);
	VectorVictor::Vector2 Get_position(long double sim_time);
	std::vector<sf::Sprite*> Planet_sprites;
	bool In_view(SFML_Window * window, int zoom_factor);
	void Draw_flag(SFML_Window * iwindow, int zoom_factor);	
	~TPlanet();
};

// Newtonian Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

class CNewtonian_Object
{	public:
	VectorVictor::Vector2 Position;
	VectorVictor::Vector2 Velocity;
	VectorVictor::Vector2 Acceleration;	// another shitter to get rid of. Need to look up all references		// maybe
	
	bool Crashed;
	bool Crash_state(long double sim_time);
	
	double Theta, Omega;
	double Get_omega();
	double Get_theta_in_degrees();		
	double Get_theta_in_radians();
	
	double Length, PMI;
	double Get_length();
	
	double Get_hull_mass();
	virtual double Get_total_mass();
	virtual double Get_PMI();		

	std::vector<Resource_Tank*> Fuel_tanks;
	std::vector<Vessel_component*> Object_components;
	
	void Add_force(double attack_point_x, double attack_point_y, double force_x, double force_y);
	void Add_force(VectorVictor::Vector2 attack_point, VectorVictor::Vector2 force_vector);
	std::vector<Force> Force_list;	
	
	void Frame(long double dt, long double simtime);
	void Update_motion(long double dt);
	void Update_rotation(long double dt);
	
	bool Update_Flag;
	bool Update_flag();
	void Update_PMI();	
	
	virtual void Print_data();	
	virtual void Receive_inputs(key_commands * current_inputs, double dt);	
	
	virtual bool In_view(SFML_Window * window, int zoom_factor);	
	
	sf::Sprite * Object_sprite;	
	sf::Sprite * Flag_sprite;	
	void Draw_flag(SFML_Window * iwindow, int zoom_factor);
	std::string Object_name;	
	
	CNewtonian_Object* Get_Newtonian_pointer();
};

std::vector<CNewtonian_Object*> Newtonian_list;

// TVessel Class /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

class TVessel: public CNewtonian_Object
{	public:
	
	Hull * Hull_component;
	
	virtual void Rotate_left(double dt);
	virtual void Rotate_right(double dt);
	virtual void Kill_rotation(double dt);					// this seems at first to conflict with rot left/right, but since input commands come in one by one, they shouldnt. I hope
	virtual void Translate_forward(double dt);
	virtual void Translate_backward(double dt);
	virtual void Translate_left(double dt);
	virtual void Translate_right(double dt);
	virtual void Throttle_up(double dt);
	virtual void Throttle_down(double dt);
	virtual void No_command(double dt);
	
	std::vector<Thruster*> Thrusters;
	

	sf::Texture * Vessel_tex;

	bool In_view(SFML_Window * window, int zoom_factor);
	void Draw_flag(SFML_Window * window, int zoom_factor);	

	void Draw_vessel(SFML_Window * iwindow);												// this should eventually become a vertex array with the thruster visuals thrown in alongside it
	virtual void Draw_controls(SFML_Window * iwindow, bool Map_status);
	
	std::string Vessel_class;
	std::string Get_vessel_class();
	std::string Get_vessel_name();
	
	TVessel* Get_Vessel_pointer();
};

std::vector<TVessel*> Vessel_list;

class XWing: public TVessel
{	public:
	XWing(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, sf::Texture * status_texture, sf::Font * controls_font, sf::Texture * panel_texture1);
	// Inherited functions
	double k_throttle;
	sf::Text * main_fuel_level, * rcs_fuel_level, * omega_value, * theta_value, * position_values, * velocity_values, * vessel_id;
	sf::Sprite * vessel_display, * display_panel;
	bool Throttle_lock;
	void Receive_inputs(key_commands * current_inputs, double dt);
	double Get_total_mass();
	double Get_PMI();			
	void Drain_fuel(double dt);
	void Print_data();
	void Toggle_throttle_lock();
	void Draw_controls(SFML_Window * iwindow, bool Map_status);	
	void Rotate_left(double dt);
	void Rotate_right(double dt);
	void Translate_forward(double dt);
	void Translate_backward(double dt);
	void Translate_left(double dt);
	void Translate_right(double dt);
	void Throttle_up(double dt);
	void Throttle_down(double dt);	
	void No_command(double dt);
	void Kill_rotation(double dt);					// this seems at first to conflict with rot left/right, but since input commands come in one by one, they shouldnt. I hope
	Resource_Tank * Main_fuel, * RCS_fuel;
	Thruster * Main_engines, * Bow_left, * Bow_right, * Bow_fore, * Stern_left, * Stern_right, * Stern_aft;
	sf::Color * text_colour;
	~XWing();
};



