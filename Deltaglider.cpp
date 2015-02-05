// Deltaglider.cpp /////////////////////////////////////////////////////////////
// A testbed for ignition, based off of ////////////////////////////////////////
// the one and only DG, from Orbiter ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include <string>
#include <iostream>
#include <math.h>
#include "Deltaglider.hpp"


// Delta Glider Class //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


DeltaGlider::DeltaGlider(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, std::string panel_path, sf::Font * controls_font, Propagator_type propagator)
{	Talkback("Constructing Delta Glider");
	// we write to the console for feedback while debugging
	NewtonianState.Current_state = Flight;
	Propagator = propagator;
	
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
	NewtonianState.FlightState.Position.Set_values(initial_x_position, initial_y_position);
	NewtonianState.FlightState.Velocity.Set_values(initial_x_velocity, initial_y_velocity);
	
	this->NewtonianState.Rotation.Theta = initial_theta; 
	this->NewtonianState.Rotation.Omega = initial_omega; 
	this->NewtonianState.Rotation.Alpha = 0;
	// we take what we were passed and set our properties to it
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
	
	vessel_id.Init_object(*controls_font, sf::Vector2f(20, 380), "-", *text_colour, 16, false);

	main_fuel_level.Init_object(*controls_font, sf::Vector2f(20, 420), "-", *text_colour, 14, false);
	
	rcs_fuel_level.Init_object(*controls_font, sf::Vector2f(20, 440), "-", *text_colour, 14, false);	
	
	omega_value.Init_object(*controls_font, sf::Vector2f(20, 460), "-", *text_colour, 14, false);		
	
	theta_value.Init_object(*controls_font, sf::Vector2f(20, 480), "-", *text_colour, 14, false);	

	position_values.Init_object(*controls_font, sf::Vector2f(20, 500), "-", *text_colour, 14, false);	
	
	velocity_values.Init_object(*controls_font, sf::Vector2f(20, 520), "-", *text_colour, 14, false);		
	
	vessel_display.Init_object(sf::Vector2f(820, 460), sf::Color(255, 255, 255, 168), false, *Vessel_tex, true);
	vessel_display.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
	// Ill get around to it later
	
	// the pretty semi-transparent image of the ship that appears at bottom
	// right in the screen. Really just eye candy for now, but I have plans for
	// it later on. Its vessel implementation-specific anywayss
	
	display_panel.Init_object(sf::Vector2f(5, 360), sf::Color(255, 255, 255), false, panel_path);
	// Ill get around to it later
	
	// display_panel = new sf::Sprite();
	// display_panel->setTexture(*panel_texture1);
	// display_panel->setPosition(sf::Vector2f(5, 360));	
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
	// ok the sprite definitely gets centered there...
	Object_sprite->setRotation(this->NewtonianState.Rotation.Theta);
	// and we center the origin and rotate it appropriately
	Talkback("...Constructed Delta Glider");
	// feed back that the constructor is now finished to the console
}	


DeltaGlider::DeltaGlider(ObjectState initial_object_state, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, std::string panel_path, sf::Font * controls_font, Propagator_type propagator): CNewtonian_Object(initial_object_state)
{	// the new & improved constructor using the call to CNewtonians constructor
	// so hopefully a fair bit shorter
	// although we still have a long way to go in this area till the whole
	// system is really ready to go. In fact, the current setup probably wont
	// work with dynamic binding at all just yet, needs to only take CNewtonian
	// parameters to get set up
	
	// simplest way to get there would probably be to half implement scenario
	// loading from file first so that the constructor is clean for calling in
	// a dll callback without all of this type specific stuff
	
	// the individual sprites and textures should probably be loaded internally
	// to each object. The whole idea of sharing references between all of the
	// objects was cute, but it just wont work with the way generic vessels will
	// need to work
	Talkback("Constructing Delta Glider");
	// we write to the console for feedback while debugging
	Propagator = propagator;
	
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
	
	vessel_id.Init_object(*controls_font, sf::Vector2f(20, 380), "-", *text_colour, 16, false);
	main_fuel_level.Init_object(*controls_font, sf::Vector2f(20, 420), "-", *text_colour, 14, false);
	rcs_fuel_level.Init_object(*controls_font, sf::Vector2f(20, 440), "-", *text_colour, 14, false);	
	omega_value.Init_object(*controls_font, sf::Vector2f(20, 460), "-", *text_colour, 14, false);		
	theta_value.Init_object(*controls_font, sf::Vector2f(20, 480), "-", *text_colour, 14, false);	
	position_values.Init_object(*controls_font, sf::Vector2f(20, 500), "-", *text_colour, 14, false);	
	velocity_values.Init_object(*controls_font, sf::Vector2f(20, 520), "-", *text_colour, 14, false);		
	vessel_display.Init_object(sf::Vector2f(820, 460), sf::Color(255, 255, 255, 168), false, *Vessel_tex, true);
	vessel_display.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
	display_panel.Init_object(sf::Vector2f(5, 360), sf::Color(255, 255, 255), false, panel_path);
	
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
	Object_sprite->setRotation(this->NewtonianState.Rotation.Theta);
	// and we center the origin and rotate it appropriately
	Talkback("...Constructed Delta Glider");
	// feed back that the constructor is now finished to the console
}

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

long double DeltaGlider::Get_PMI()
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
{	std::string mainfuel = std::to_string((long long int)Main_fuel->Resource_mass);
	mainfuel.append(" kg");
	main_fuel_level.Set_element(mainfuel);
	
	std::string rcsfuel = std::to_string((long long int)RCS_fuel->Resource_mass);
	rcsfuel.append(" kg");
	rcs_fuel_level.Set_element(rcsfuel);	
	
	omega_value.Set_element(std::to_string((long long int)this->NewtonianState.Rotation.Omega));	
	
	theta_value.Set_element(std::to_string((long long int)this->NewtonianState.Rotation.Theta));
	
	std::string pos = "Position ";
	pos.append(NewtonianState.FlightState.Position.Get_vector("m"));	
	position_values.Set_element(pos);	
	
	std::string vel = "Velocity ";
	vel.append(NewtonianState.FlightState.Velocity.Get_vector("m/s"));
	velocity_values.Set_element(vel);	
	
	std::string id = Get_vessel_class();
	id.append(" Class\n");
	id.append(Get_vessel_name());
	vessel_id.Set_element(id);
	// do a hideous transfer of formatted data to the sf texts
	
	vessel_display.Draw_element(iwindow);
	display_panel.Draw_element(iwindow);
	
	main_fuel_level.Draw_element(iwindow);
	rcs_fuel_level.Draw_element(iwindow);	
	
	omega_value.Draw_element(iwindow);
	theta_value.Draw_element(iwindow);
	
	position_values.Draw_element(iwindow);
	velocity_values.Draw_element(iwindow);
	
	vessel_id.Draw_element(iwindow);
	
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
{	if(this->NewtonianState.Rotation.Omega != 0.000000000000)
	{	
		double delta_omega = Absolute_value(this->NewtonianState.Rotation.Omega);
		double magnitude_alpha;
		double max_alpha;
		if(this->NewtonianState.Rotation.Omega > 0.000000000000)
		{	max_alpha = this->Get_max_alpha(counterclockwise);
			magnitude_alpha = Absolute_value(max_alpha);
			if(this->NewtonianState.Rotation.Alpha > 0.000000000000)
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
			if(this->NewtonianState.Rotation.Alpha < 0.000000000000)
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
	else
	{	No_command(dt);
	}
}


DeltaGlider::~DeltaGlider()
{	Thrusters.clear();
	Fuel_tanks.clear();
	delete Object_sprite;
	delete Hull_component;
	delete text_colour;
	delete Hull_component;
}

