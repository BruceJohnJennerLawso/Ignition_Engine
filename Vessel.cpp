// Vessel.cpp //////////////////////////////////////////////////////////////////
// The TVessel type used for vessels ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#include <std_files>
//#include "Headers.h"
//#include "Headers.hpp"
//#include "Source.cpp"
#include "Vessel.hpp"


// TVessel Class ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



// getting a little more specific at this point, dealing with specific
// functionality that a spacecraft really needs to have


void TVessel::Translate_left(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_left))
		{	(*it)->Throttle_up(dt, RCS_Throttle_constant);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}

// obvious as it gets, just adjust throttles based on dt and the k_throttle
// for the given thruster. This should accept a parameter to modify k_throttle
// so we can modify it on the spot instead of passing through stuff with dt
// I smell disaster there, so its better not to tempt fate

void TVessel::Translate_right(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_right))
		{	(*it)->Throttle_up(dt, RCS_Throttle_constant);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}

void TVessel::Translate_backward(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_back))
		{	(*it)->Throttle_up(dt, RCS_Throttle_constant);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}

void TVessel::Translate_forward(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(translate_forward))
		{	(*it)->Throttle_up(dt, RCS_Throttle_constant);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}	

void TVessel::Rotate_right(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_clockwise))
		{	(*it)->Throttle_up(dt, RCS_Throttle_constant);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}	

void TVessel::Rotate_right(double dt, double throttle_target)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_clockwise))
		{	(*it)->Throttle_to(dt, RCS_Throttle_constant, throttle_target);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}	

void TVessel::Rotate_left(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_counterclockwise))
		{	(*it)->Throttle_up(dt, RCS_Throttle_constant);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}	

void TVessel::Rotate_left(double dt, double throttle_target)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(rotate_counterclockwise))
		{	(*it)->Throttle_to(dt, RCS_Throttle_constant, throttle_target);
		}
		else
		{	if((*it)->Is_RCS == true)
			{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
			}
		}
	}
}	

void TVessel::No_command(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_RCS == true)
		{	(*it)->Throttle_down(dt, RCS_Throttle_constant);
		}
	}
}	

void TVessel::Throttle_up(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(main_engines))
		{	(*it)->Throttle_up(dt, Main_throttle_constant);
		}
	}
	
}

void TVessel::Throttle_down(double dt)
{	for(std::vector<Thruster*>::iterator it = Thrusters.begin(); it != Thrusters.end(); ++it)
	{	if((*it)->Is_in_group(main_engines))
		{	(*it)->Throttle_down(dt, Main_throttle_constant);
		}
	}
}	

void TVessel::Kill_rotation(double dt)
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
{	long double radius = this->Get_length();
	radius /= 2;
	if(window->Intersection(this->NewtonianState.FlightState.Position, radius, 1) == true)
	{	// cam scale of 1 for the moment just so we dont have any crazy stuff
		return true;
	}
	else
	{	//std::cout << "No intersection for vessel " << this->Get_vessel_name() << std::endl;
		return false;
	}
	//return window->Intersection(this->NewtonianState.FlightState.Position, radius);
	// muuuch better
	
	
	//if((NewtonianState.FlightState.Position.Get_x() >= (window->origin.x-(Hull_component->Get_hull_length_squared())))&&(NewtonianState.FlightState.Position.Get_x() <= ((window->origin.x + (window->Aperture_width+(Hull_component->Get_hull_length_squared()))))))
	//{	if((NewtonianState.FlightState.Position.Get_y() <= (window->origin.y+(Hull_component->Get_hull_length_squared())))&&(NewtonianState.FlightState.Position.Get_y() >= (window->origin.y - ((window->Aperture_height+(Hull_component->Get_hull_length_squared()))))))
	//	{	return true;
	//	}
	//	else
	//	{	return false;
	//	}
	//}	
	//else
	//{	return false;
	//}
	
	
	// just another simple function for determining if the ship should be drawn
	// there was some logic behind squaring the distance, but I dont recall
}

bool TVessel::In_view(SFML_Window * window, long double cam_scale)
{	long double radius = this->Get_length();
	radius /= 2;
	if(window->Intersection(this->NewtonianState.FlightState.Position, radius, cam_scale) == true)
	{	// cam scale of 1 for the moment just so we dont have any crazy stuff
		return true;
	}
	else
	{	//std::cout << "No intersection for vessel " << this->Get_vessel_name() << std::endl;
		return false;
	}
}

void TVessel::Draw_vessel(SFML_Window * iwindow, long double cam_scale)
{	double scale_factor = pix_length/(Vessel_tex->getSize().y*cam_scale);
	
//	Object_sprite->setScale((pix_length/((Vessel_tex->getSize().y)*cam_scale)),(pix_length/((Vessel_tex->getSize().y)*cam_scale)));

	Object_sprite->setScale(sf::Vector2f(scale_factor,scale_factor));
	// rescale the axes of the texture to match pix_length in the y and the
	// appropriate scale for the x dimension
		
	// not sure why they both use y, but I think this was distorted when
	// it used x & y.
	// This area needs to be looked over again
	sf::Vector2f camera_offset = Get_window_coordinates(NewtonianState.FlightState.Position, iwindow, cam_scale);
	// I just dont get it, this should work just fine
	// translation looks okay, but the thing goes nuts rotating twice over the
	// full 360 degrees 
	Object_sprite->setPosition(camera_offset);
	Object_sprite->setRotation(NewtonianState.Rotation.Theta - iwindow->Aperture_rotation);
	// that should work just fine and dandy

	Flag_sprite.setPosition(camera_offset + sf::Vector2f((10*Hull_component->Length), -(10*Hull_component->Length)));
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
	Flag_sprite.setPosition(camera_offset);
	iwindow->window->draw(Flag_sprite);
	// and we finally locate the sprite in the window, and draw it
}

std::string TVessel::Get_vessel_class()
{	return Vessel_class;
}	// the string of the given class

std::string TVessel::Get_vessel_name()
{	return Object_name;
}	// the string of the objects name

void TVessel::Render_MFDs(SFML_Window &window, key_commands &keyCommands, Cursor_commands &cursorCommands, long double &cameraScale, long double dt, int &time_acceleration, long double sim_time, std::vector<CNewtonian_Object*> &newtonians, std::vector<CKeplerian_Object*> &keplerians, std::vector<TVessel*> &vessels, VectorVictor::Vector2 &Camera_target, long double &Camera_rotation)
{	
	for(std::vector<MFD*>::iterator it = MFD_displays.begin(); it != MFD_displays.end(); ++it)
	{	(*it)->Render_MFD(window, keyCommands, cursorCommands, cameraScale, dt, time_acceleration, sim_time, *this, newtonians, keplerians, vessels, Camera_target, Camera_rotation);
		// this looks good for now
		
		// tried changing what exactly gets sent to the MFD for the vessel
		// type, maybe this will fix it
	} 
}

bool TVessel::Start_MFD(std::string mfd_type, orientation mfd_orientation, sf::Vector2f mfd_offset)
{	MFD * new_mfd;
	if(Create_MFD(mfd_type, new_mfd, mfd_orientation, mfd_offset) == true)
	{	MFD_displays.insert(MFD_displays.end(), new_mfd);
		return true;
	}
	return false;
}

void TVessel::Draw_MFDs(SFML_Window &window)
{	for(std::vector<MFD*>::iterator it = MFD_displays.begin(); it != MFD_displays.end(); ++it)
	{	(*it)->Draw_MFD(window, sf::Color(255, 255, 255));
		// iterate through all of the MFDs that the vessel has and draw them to
		// the screen
	}
}

TVessel* TVessel::Get_Vessel_pointer()
{	return this;
}
