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


DeltaGlider::DeltaGlider(double initial_x_position, double initial_y_position, double initial_x_velocity, double initial_y_velocity, double initial_theta, double initial_omega, double initial_main_propellant, double initial_rcs_propellant,  sf::Sprite * iFlag_sprite, sf::Texture * XWing_texture, std::string ivessel_name, sf::Texture * status_texture, sf::Font * controls_font, sf::Texture * panel_texture1)
{	Talkback("Constructing Delta Glider");
	// we write to the console for feedback while debugging
	NewtonianState.Current_state = Flight;
	Propagator = Euler1;
	
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
	Theta = initial_theta; Omega = initial_omega; 
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
	
	vessel_id = new sf::Text();
	vessel_id->setFont(*controls_font);
	vessel_id->setCharacterSize(16);
	vessel_id->setColor(*text_colour);
	vessel_id->setPosition(20, 380);
	
	main_fuel_level = new sf::Text();
	main_fuel_level->setFont(*controls_font);
	main_fuel_level->setCharacterSize(14);
	main_fuel_level->setColor(*text_colour);
	main_fuel_level->setString(std::to_string((long double)Main_fuel->Resource_mass));
	// $%@#$% MICROSOFT
	main_fuel_level->setPosition(20, 420);
	
	rcs_fuel_level = new sf::Text();
	rcs_fuel_level->setFont(*controls_font);	
	rcs_fuel_level->setCharacterSize(14);
	rcs_fuel_level->setColor(*text_colour);
	rcs_fuel_level->setString(std::to_string((long double)RCS_fuel->Resource_mass));	
	rcs_fuel_level->setPosition(20, 440);
	
	omega_value = new sf::Text();
	omega_value->setFont(*controls_font);	
	omega_value->setCharacterSize(14);
	omega_value->setColor(*text_colour);
	omega_value->setString(std::to_string((long double)Omega));	
	omega_value->setPosition(20, 460);
	
	theta_value = new sf::Text();
	theta_value->setFont(*controls_font);	
	theta_value->setCharacterSize(14);
	theta_value->setColor(*text_colour);
	theta_value->setString(std::to_string((long double)Theta));	
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
{	std::string mainfuel = std::to_string((long long int)Main_fuel->Resource_mass);
	mainfuel.append(" kg");
	main_fuel_level->setString(mainfuel);
	std::string rcsfuel = std::to_string((long long int)RCS_fuel->Resource_mass);
	rcsfuel.append(" kg");
	rcs_fuel_level->setString(rcsfuel);	
	omega_value->setString(std::to_string((long long int)Omega));	
	theta_value->setString(std::to_string((long long int)Theta));
	std::string pos = "Position ";
	pos.append(NewtonianState.FlightState.Position.Get_vector("m"));	
	position_values->setString(pos);	
	std::string vel = "Velocity ";
	vel.append(NewtonianState.FlightState.Velocity.Get_vector("m/s"));
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

